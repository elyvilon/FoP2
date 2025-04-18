#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

hd44780_I2Cexp lcd;

// copy character from progmem to temporary buffer
void loadCharFromProgmem(byte slot, const byte *charArray) {
  byte buffer[8];
  memcpy_P(buffer, charArray, 8);
  lcd.createChar(slot, buffer);
}

// clear display buffer
void clearDisplayBuffer() {
  for (byte row = 0; row < 4; row++) {
    for (byte col = 0; col < 20; col++) {
      displayBuffer[col][row] = ' ';
    }
  }
}

// update display buffer
void updateToDisplayBuffer(byte col, byte row, char newChar) {
  if (displayBuffer[col][row] != newChar) {
    lcd.setCursor(col, row);
    lcd.write(newChar);
    displayBuffer[col][row] = newChar;
  }
}


// reset character sprites to default values
void resetDefaultObjects() {
  loadCharFromProgmem(0, characterArmsDown[0]);
  loadCharFromProgmem(1, birch);
  loadCharFromProgmem(2, spruce);
  loadCharFromProgmem(3, boulder);
  loadCharFromProgmem(4, hole);
  loadCharFromProgmem(5, fire[0]);
  loadCharFromProgmem(6, mushroom);
  loadCharFromProgmem(7, flag[0]);
}


// draw main menu to indicate current selection
void drawMenu(byte selectedIndex, byte topMenuIndex) {

  lcd.clear();  

  for (byte i = 0; i < 4; i++) {
    byte menuItemIndex = topMenuIndex + i;
    if (menuItemIndex < totalMenuItems) {
      lcd.setCursor(0, i);
      if (menuItemIndex == selectedIndex) {
        lcd.print("> ");
      } else {
        lcd.print("  ");
      }
      lcd.print(menuItems[menuItemIndex]);
    }
  }
}


// draw name entry screen
void drawPlayerName(byte currentCharIndex){

  lcd.clear();
  lcd.setCursor(5,1);
  lcd.print("Enter name:");
  lcd.setCursor(8, 2);
  lcd.print(playerName);

  lcd.setCursor(currentCharIndex+8, 3);
  lcd.print("^");
}


// clear the tile given as parameter
void clearTile(byte col, byte row) {
  updateToDisplayBuffer(col, row, ' ');
}


// function to redraw screen
void updateScreen(bool drawPlayerCharacter) {
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 20; col++) {
      if (col == currentCharacterCol && row == currentCharacterRow) { // player character drawn last so that the object behind the player gets replaced
        continue;
      }
      switch (objectMap[col][row]) {
        case BIRCH:
          updateToDisplayBuffer(col, row, byte(1));
          break;
        case SPRUCE:
          updateToDisplayBuffer(col, row, byte(2));
          break;
        case BOULDER:
          updateToDisplayBuffer(col, row, byte(3));
          break;
        case HOLE:
          updateToDisplayBuffer(col, row, byte(4));
          break;
        case FIRE:
          updateToDisplayBuffer(col, row, byte(5));
          break;
        case MUSHROOM:
          updateToDisplayBuffer(col, row, byte(6));
          break;
        case GOAL:
          updateToDisplayBuffer(col, row, byte(7));
          break;
      }
    }
  }
  // draw player if not at the beginning of level
  if (drawPlayerCharacter) {
    updateToDisplayBuffer(currentCharacterCol, currentCharacterRow, byte(0));
  }
}


// animate moving objects
void animateObjects() {
  static unsigned long previousObjectAnimationTime = 0;
  static byte objectAnimationFrameCount = 0;

  unsigned long currentTime = millis();

  if (currentTime - previousObjectAnimationTime >= objectAnimationInterval) {
    previousObjectAnimationTime = currentTime;

    // 8 frame animation
    objectAnimationFrameCount = (objectAnimationFrameCount + 1) % 8;
    loadCharFromProgmem(5, fire[objectAnimationFrameCount]);
    loadCharFromProgmem(7, flag[objectAnimationFrameCount]);

    // redraw screen
    updateScreen(true);
  }
}


// animate player character movement
bool animateCharacterMove(bool climbingMode, byte currentCharacterCol, byte currentCharacterRow) {
  static unsigned long previousCharacterAnimationTime = 0;
  static byte animationFrame = 0;

  unsigned long currentTime = millis();

  if (currentTime - previousCharacterAnimationTime >= characterAnimationInterval) {
    previousCharacterAnimationTime = currentTime;

    // select correct animation frame
    loadCharFromProgmem(0, climbingMode ? characterArmsUp[animationFrame] : characterArmsDown[animationFrame]);

    updateToDisplayBuffer(currentCharacterCol, currentCharacterRow, byte(0));

    // move to next frame
    animationFrame++;

    // check if the animation has finished
    if (animationFrame >= characterAnimationFrameCount) {
      animationFrame = 0;
      return true;
    }
  }
    return false; // animation is still on going
}


// function for blocking animation
void blockingAnimation(const byte blockingAnimation[][8], byte blockingAnimationFrameCount) {

  // at the beginning of game do not clear previous tile, otherwise clear it
  if (currentLevel != 1 || (currentLevel == 1 && currentCharacterCol > 0)) {
    clearTile(prevCharacterCol, prevCharacterRow);
  }

  for (int i = 0; i < blockingAnimationFrameCount; i++) {
    loadCharFromProgmem(0, blockingAnimation[i]);
    updateScreen(true);
    delay(blockingAnimationInterval);
  }
}


// message screen single animation
void messageAnimation(byte messageAnimation[][8], byte messageAnimationFrameCount, byte col, byte row) {
  for (int i = 0; i < messageAnimationFrameCount; i++) {
    loadCharFromProgmem(0, messageAnimation[i]);
    lcd.setCursor(col,row);
    lcd.write(byte(0));
    delay(messageAnimationInterval);
  }
}


// message screen two animations
void messageAnimation(byte messageAnimation[][8], byte messageAnimationFrameCount, byte col, byte row, byte col2, byte row2) {
  for (int i = 0; i < messageAnimationFrameCount; i++) {
    loadCharFromProgmem(0, messageAnimation[i]);
    lcd.setCursor(col,row);
    lcd.write(byte(0));
    lcd.setCursor(col2,row2);
    lcd.write(byte(0));
    delay(messageAnimationInterval);
  }
}


// level clear time count down to zero
void timeCountDownAnimation() {
    for (int i = levelTime; i >= 0; i--) {
      lcd.setCursor(12,2);
      lcd.print("       ");
      lcd.setCursor(12,2);
      lcd.print(i);
      lcd.print(" s");
      playSoundEffect(COUNTDOWN);
      delay(countDownInterval);
  }
  playSoundEffect(COUNTDOWNFINISHED);
}
