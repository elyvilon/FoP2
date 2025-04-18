#include "globalvar.h"
#include "highscore.h"
#include "map.h"
#include "sound.h"
#include "character.h"
#include "lcd.h"
#include "input.h"
#include "message.h"


// macro to count the length of an array
#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

// funktion to reset the arduino
void(* resetFunc) (void) = 0;


void setup() {

  // seed for randomness from an empty analog pin
  randomSeed(analogRead(7));

  // debouncing time for buttons
  setButtonDebounceTime();

  // fast mode enabled
  Wire.setClock(400000);

  // LCD on
  lcd.begin(20,4);
  readHighScore();
  // start game, delay to let the arduino properly start
  delay(1000);

  startScreen();
  menuSelection();
  inputPlayerName();
  startLevel();
}


void loop() {

  // read the state of buttons
  checkButtons();

  // if start pressed and not currently paused, pause the game
  if (buttonStatus[5] && !isPaused) {
    isPaused = true;
    buttonStatus[5] = false;
    millisInPausedStart = millis(); // start time of pause to later calculate total time spent in paused state
    playSoundEffect(PAUSED);
  }
  // if start pressed and currently paused, return to normal state
  else if (buttonStatus[5] && isPaused) {
    isPaused = false;
    buttonStatus[5] = false;
    millisInPaused = millis() - millisInPausedStart + millisInPaused; // total time spent in paused state
    playSoundEffect(UNPAUSED);
  }

  // if not currently paused, continue
  if (!isPaused) {

    // check if character animation is still ongoing
    if (isAnimating) {
      if (animateCharacterMove(climbingMode, currentCharacterCol, currentCharacterRow)) {
          isAnimating = false;
      }
    }

    // if A-button pressed, switch the state of climbing mode
    if (buttonStatus[4]) {
      climbingMode = !climbingMode;
      buttonStatus[4] = false;
        if (climbingMode) {
          climbingModeOnSound();
          loadCharFromProgmem(0, characterArmsUp[0]);
        }
        else {
          climbingModeOffSound();
          loadCharFromProgmem(0, characterArmsDown[0]);
        }
    }

    if (buttonStatus[0] || buttonStatus[1] || buttonStatus[2] || buttonStatus[3]) {
      isAnimating = true;
      updatePosition();
    }

    // animoidaan objektit
    animateObjects();
  }
}


void updatePosition() {

  // save previous location
  prevCharacterCol = currentCharacterCol;
  prevCharacterRow = currentCharacterRow;

  bool outOfBounds = false;

  // update location based on directional button pressed
  if (buttonStatus[0] && currentCharacterRow > 0) currentCharacterRow--; // up
  else if (buttonStatus[1] && currentCharacterRow < 3) currentCharacterRow++; // down
  else if (buttonStatus[2] && currentCharacterCol > 0) currentCharacterCol--; // left
  else if (buttonStatus[3] && currentCharacterCol < 19) currentCharacterCol++; // right
  else outOfBounds = true;

  // deny movement if target out of bounds
  if (outOfBounds) {
    playSoundEffect(OUTOFBOUNDS);
    currentCharacterCol = prevCharacterCol;
    currentCharacterRow = prevCharacterRow;
  }
  else {
    // fetch object type in target location
    ObjectType object = objectMap[currentCharacterCol][currentCharacterRow];
    if (object == NONE) {
      // no obstacle, move character
      playSoundEffect(MOVE);
      clearTile(prevCharacterCol, prevCharacterRow);
    }
    else if (object == BIRCH || object == SPRUCE) {
      // if object is tree
      if (climbingMode) {
        // if climbing mode engaged, move character
        playSoundEffect(CLIMB);
        clearTile(prevCharacterCol, prevCharacterRow);
        climbingMode = false;
        loadCharFromProgmem(0, characterArmsDown[0]);
        updateScreen(true);
      }
        else {
          // if climbing mode disengaged, deny movement
          playSoundEffect(BLOCKED);
          currentCharacterCol = prevCharacterCol;
          currentCharacterRow = prevCharacterRow;
        }
    }
    else if (object == MUSHROOM) {
      playSoundEffect(ITEM);
      clearTile(prevCharacterCol, prevCharacterRow);
      itemsCollected++;
      objectMap[currentCharacterCol][currentCharacterRow] = NONE;
      updateScreen(true);
    }
    else if (object == BOULDER) {
      // if boulder, deny movement
      playSoundEffect(BLOCKED);
      currentCharacterCol = prevCharacterCol;
      currentCharacterRow = prevCharacterRow;
    }
    else if (object == FIRE) {
      // if fire, run death by fire animation and reset
      deathByFire();
    }
    else if (object == HOLE) {
      // if hole, run death by falling animation and reset
      deathByFall();
    }
    else if (object == GOAL) {
      // if goal flag, end level
      endLevel();
    }
    clearTile(prevCharacterCol, prevCharacterRow);
    animateCharacterMove(climbingMode, currentCharacterCol, currentCharacterRow);

    // update contents of screen
    updateScreen(true);
  }
}


unsigned int calculateTimeBonus(unsigned int t) {

    if (t >= maxTime) {
        return 0; // no bonus if time longer than max time
    }
    return maxTimeBonus * (maxTime - t) / maxTime;
}


void startLevel() {

  // clear the display buffer
  clearDisplayBuffer();

  // generate level
  lcd.clear();
  generateMap();
  randomizeObstacles(currentLevel);
  resetDefaultObjects
  ();

  // reset level variables
  climbingMode = false;
  itemsCollected = 0;
  timeBonus = 0;
  itemsBonus = 0;
  levelScore = 0;
  millisInPaused = 0;
  currentCharacterCol = 0;
  currentCharacterRow = 0;

  // update the screen without player character
  updateScreen(false);

  // play level melody according to the difficulty
  if (currentLevel <= 10) {
    playMelody(EASY);
  } else if (currentLevel > 10 && currentLevel <= 20) {
    playMelody(MEDIUM);
  } else if (currentLevel > 21) {
    playMelody(HARD);
  }

  delay(1000);

  playSoundEffect(EMERGE);
    blockingAnimationFrameCount = ARRAY_LENGTH(characterEntering);
    blockingAnimation(characterEntering, blockingAnimationFrameCount);
  
  // level begins
  updateScreen(true);
  playSoundEffect(MOVE);
  levelStartMillis = millis();
}


void endLevel() {
  byte blockingAnimationFrameCount;
    // level time in millis
    levelEndMillis = millis();
    // calculate total level time in seconds
    levelTime = (levelEndMillis - levelStartMillis - millisInPaused)/1000;
    // total time of game
    totalTime += levelTime;
    // calculate time bonus
    timeBonus = calculateTimeBonus(levelTime);
    itemsBonus = itemsCollected*100;
    levelScore = timeBonus+itemsBonus;
    totalItems += itemsCollected;
    totalScore += levelScore;

    playSoundEffect(EMERGE);
    blockingAnimationFrameCount = ARRAY_LENGTH(characterReachedGoal);
    blockingAnimation(characterReachedGoal, blockingAnimationFrameCount);

    // check if level was last level
    if (currentLevel == 30) {
      gameCompleteMessage();
      resetFunc();
    } else {
    
    // move the player character back to beginning
    currentCharacterCol = 0, currentCharacterRow = 0;
    levelCompleteMessage();

    // move to the next level
    currentLevel++;
    startLevel();
    }
}

// function to animate death by fire animation and reset
void deathByFire() {
  playSoundEffect(BURN);
  int blockingAnimationFrameCount = ARRAY_LENGTH(characterBurning);
  blockingAnimation(characterBurning, blockingAnimationFrameCount);
  playSoundEffect(DUST);
  blockingAnimationFrameCount = ARRAY_LENGTH(characterDust);
  blockingAnimation(characterDust, blockingAnimationFrameCount);
  delay(2000);
  defeatMessage();
  resetFunc();
}

// function to animate death by falling animation and reset
void deathByFall() {
  int blockingAnimationFrameCount = ARRAY_LENGTH(characterMovingOverHole);
  blockingAnimation(characterMovingOverHole, blockingAnimationFrameCount);
  playSoundEffect(FALL);
  blockingAnimationFrameCount = ARRAY_LENGTH(characterFalling);
  blockingAnimation(characterFalling, blockingAnimationFrameCount);
  delay(500);
  playSoundEffect(IMPACT);
  blockingAnimationFrameCount = ARRAY_LENGTH(characterImpact);
  blockingAnimation(characterImpact, blockingAnimationFrameCount);
  delay(2000);
  defeatMessage();
  resetFunc();
}
