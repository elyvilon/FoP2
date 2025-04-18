// macro to count the length of an array
#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

// level score message screen
void levelScoreMessage() {
  lcd.clear();

  lcd.setCursor(1, 0);
  lcd.print("Mushrooms: ");
  lcd.print(itemsCollected);
  lcd.print(" x 100");
  lcd.setCursor(3,1);
  lcd.print("Time Bonus: ");
  lcd.print(timeBonus);  
  lcd.setCursor(4, 2);
  lcd.print("Level score:");
  lcd.setCursor(8,3);
  lcd.print(levelScore);  

  delay(6000);

}

// total game score message screen
void totalScoreMessage() {
  
  lcd.clear();
  
  lcd.setCursor(1,0);
  lcd.print("Total time:  ");
  lcd.print(totalTime);
  lcd.print(" s");
  lcd.setCursor(1,1);
  lcd.print("Total items: ");
  lcd.print(totalItems);

  lcd.setCursor(1,2);
  lcd.print("Total score: ");
  lcd.print(totalScore);
  delay(4000);

  // check for high score
  if (totalScore > highScore) {
    writeHighScore();
    for (byte i = 0; i < 8; i++) {
      lcd.setCursor(0,3);
      lcd.print("  New High Score!   ");
      delay(1000);
      lcd.setCursor(0,3);
      lcd.print("                    ");
      delay(1000);
    }
  }
  delay(4000);
}


// GAME OVER MAN
void defeatMessage() {
  lcd.clear();
  loadCharFromProgmem(0, frame[0]);
  loadCharFromProgmem(1, frame[1]);
  loadCharFromProgmem(2, frame[2]);
  loadCharFromProgmem(3, frame[3]);
  loadCharFromProgmem(4, frame[4]);
  loadCharFromProgmem(5, frame[5]);
  loadCharFromProgmem(6, skull);
  loadCharFromProgmem(7, tombstone);

  lcd.setCursor(0,0);
  lcd.write(byte(0));
  for (byte i = 0; i < 18; i++) {
    lcd.write(byte(1));
  }
  lcd.write(byte(2));

  lcd.setCursor(0,1);lcd.write(byte(3));lcd.setCursor(19,1);lcd.write(byte(3));
  lcd.setCursor(0,2);lcd.write(byte(3));lcd.setCursor(19,2);lcd.write(byte(3));
  
  lcd.setCursor(0,3);
  lcd.write(byte(4));
  for (byte i = 0; i < 18; i++) {
    lcd.write(byte(1));
  }
  lcd.write(byte(5));

  lcd.setCursor(8,1);
  lcd.print("GAME");
  lcd.setCursor(8,2);
  lcd.print("OVER");

  lcd.setCursor(5,1);lcd.write(byte(6));lcd.setCursor(14,1);lcd.write(byte(6));
  lcd.setCursor(5,2);lcd.write(byte(7));lcd.setCursor(14,2);lcd.write(byte(7));

  playMelody(DEFEAT);
  delay(2000);

  totalScoreMessage();
}


// game completion message screen
void gameCompleteMessage() {

  lcd.clear();
  
  loadCharFromProgmem(0, characterArmsUp[0]);
  loadCharFromProgmem(1, smiley);
  loadCharFromProgmem(2, heart);

  lcd.setCursor(0,0); lcd.write(byte(2)); lcd.setCursor(19,0); lcd.write(byte(2));
  lcd.setCursor(0,3); lcd.write(byte(1)); lcd.setCursor(19,3); lcd.write(byte(1));

  lcd.setCursor(2, 1);
  lcd.print("Congratulations");
  lcd.setCursor(7, 2);
  lcd.print(playerName);
  lcd.print("!");

  playMelody(GAMECOMPLETE);
  messageAnimationFrameCount = ARRAY_LENGTH(characterCelebrating);
  messageAnimation(characterCelebrating, messageAnimationFrameCount,2, 2, 16, 2);
  messageAnimation(characterCelebrating, messageAnimationFrameCount,2, 2, 16, 2);
  messageAnimationFrameCount = ARRAY_LENGTH(characterDancing);
  messageAnimation(characterDancing, messageAnimationFrameCount,2, 2, 16, 2);
  messageAnimation(characterDancing, messageAnimationFrameCount,2, 2, 16, 2);
  messageAnimationFrameCount = ARRAY_LENGTH(characterJumping);
  messageAnimation(characterJumping, messageAnimationFrameCount,2, 2, 16, 2);
  messageAnimation(characterJumping, messageAnimationFrameCount,2, 2, 16, 2);

  delay(2000);
  totalScoreMessage();

}


// level completion message screen
void levelCompleteMessage() {

  lcd.clear();
  loadCharFromProgmem(1, smiley);
  loadCharFromProgmem(2, heart);

  lcd.setCursor(0,0); lcd.write(byte(1)); lcd.setCursor(19,0); lcd.write(byte(1));
  lcd.setCursor(0,3); lcd.write(byte(2)); lcd.setCursor(19,3); lcd.write(byte(2));


  lcd.setCursor(4, 1);
  lcd.print("Level ");
  lcd.print(currentLevel);
  lcd.print(" clear");  

  lcd.setCursor(6,2);
  lcd.print("Time: ");
  lcd.print(levelTime);
  lcd.print(" s");

  playMelody(LEVELCOMPLETE);
  messageAnimationFrameCount = ARRAY_LENGTH(characterDancing);
  messageAnimation(characterDancing, messageAnimationFrameCount,3, 2, 17, 2);
  messageAnimationFrameCount = ARRAY_LENGTH(characterCelebrating);
  messageAnimation(characterCelebrating, messageAnimationFrameCount,3, 2, 17, 2);

  timeCountDownAnimation();
  delay(1000);

  levelScoreMessage();
}


// title screen
void startScreen() {
  lcd.clear();
  loadCharFromProgmem(0, startSpruce[0]);
  loadCharFromProgmem(1, startSpruce[1]);
  loadCharFromProgmem(2, startSpruce[2]);
  loadCharFromProgmem(3, startSpruce[3]);
  loadCharFromProgmem(4, startBirch[0]);
  loadCharFromProgmem(5, startBirch[1]);
  loadCharFromProgmem(6, startBirch[2]);
  loadCharFromProgmem(7, startBirch[3]);

  lcd.setCursor(0,1); lcd.write(byte(0)); lcd.setCursor(1,1); lcd.write(byte(1)); lcd.setCursor(0,2); lcd.write(byte(2)); lcd.setCursor(1,2); lcd.write(byte(3));

  lcd.setCursor(5,1);
  lcd.print("Forest of");
  lcd.setCursor(7,2);
  lcd.print("Peril");
  lcd.setCursor(8,3);
  lcd.print("II");

  lcd.setCursor(18,1); lcd.write(byte(4)); lcd.setCursor(19,1); lcd.write(byte(5)); lcd.setCursor(18,2); lcd.write(byte(6)); lcd.setCursor(19,2); lcd.write(byte(7));

  playMelody(START);
 
unsigned long previousBlink = 0;

while (true) {
    checkButtons();

    unsigned long currentTime = millis();

    if (currentTime - previousBlink >= blinkInterval) {
        previousBlink = currentTime;

        lcd.setCursor(5,1);
        lcd.print("          ");
        lcd.setCursor(6,2);
        lcd.print("        ");
        lcd.setCursor(7,3);
        lcd.print("        ");

        static bool showText = false;
        if (showText) {
            lcd.setCursor(7,1);
            lcd.print("Press");
            lcd.setCursor(7,2);
            lcd.print("START");
        }
        showText = !showText;
    }

    if (buttonStatus[5]){
      playSoundEffect(ITEM);
      break;
    }
  }
 
}

// game instructions message screen
void instructionsMessage() {
  loadCharFromProgmem(0, birch);
  loadCharFromProgmem(1, spruce);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" ");lcd.write(byte(0));lcd.print(" ");lcd.write(byte(1));lcd.print(" ");
  lcd.write(byte(0));lcd.print(" ");lcd.write(byte(1));lcd.print(" ");lcd.write(byte(0));
  lcd.print(" ");lcd.write(byte(1));lcd.print(" ");lcd.write(byte(0));lcd.print(" ");
  lcd.write(byte(1));lcd.print(" ");lcd.write(byte(0));lcd.print(" ");lcd.write(byte(1));

  lcd.setCursor(0,1);lcd.write(byte(1));lcd.setCursor(1,2);lcd.write(byte(0));lcd.setCursor(0,3);lcd.write(byte(1));
  lcd.setCursor(18,1);lcd.write(byte(0));lcd.setCursor(19,2);lcd.write(byte(1));lcd.setCursor(18,3);lcd.write(byte(0));

  lcd.setCursor(5,1);
  lcd.print("Climb trees");  
  lcd.setCursor(4,2);
  lcd.print("Climbing mode");
  lcd.setCursor(4,3);
  lcd.print("with A-button");
  
  delay(6000);

  loadCharFromProgmem(0, mushroom);
  loadCharFromProgmem(1, boulder);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Collect mushrooms");
  lcd.setCursor(2,1);
  lcd.write(byte(0));lcd.print(" ");lcd.write(byte(0));lcd.print(" ");lcd.write(byte(0));
  lcd.print(" ");lcd.write(byte(0));lcd.print(" ");lcd.write(byte(0));lcd.print(" ");
  lcd.write(byte(0));lcd.print(" ");lcd.write(byte(0));lcd.print(" ");lcd.write(byte(0));
  lcd.print(" ");lcd.write(byte(0));
  lcd.setCursor(1,2);
  lcd.print("Go around boulders");
  lcd.setCursor(2,3);
  lcd.write(byte(1));lcd.print(" ");lcd.write(byte(1));lcd.print(" ");lcd.write(byte(1));
  lcd.print(" ");lcd.write(byte(1));lcd.print(" ");lcd.write(byte(1));lcd.print(" ");
  lcd.write(byte(1));lcd.print(" ");lcd.write(byte(1));lcd.print(" ");lcd.write(byte(1));
  lcd.print(" ");lcd.write(byte(1));
  
  delay(6000);

  loadCharFromProgmem(1, hole);
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.write(byte(1));
  lcd.setCursor(5,1);
  lcd.print("Avoid pits");
  lcd.setCursor(17,1);
  lcd.write(byte(1));
  lcd.setCursor(6,2);
  lcd.print("and fire");
  
  messageAnimationFrameCount = ARRAY_LENGTH(fire);
  for (byte i = 0; i < 6; i++) {
    messageAnimation(fire, messageAnimationFrameCount,2, 2, 17, 2);
  }

  lcd.clear();
  lcd.setCursor(5,1);
  lcd.print("Reach the");
  lcd.setCursor(8,2);
  lcd.print("GOAL");
  
  messageAnimationFrameCount = ARRAY_LENGTH(flag);
  for (byte i = 0; i < 6; i++) {
    messageAnimation(flag, messageAnimationFrameCount,3, 2, 15, 2);
  }
}


// high score message screen
void highScoreMessage() {
  
  loadCharFromProgmem(0, characterArmsUp[0]);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Highest score by:");
  lcd.setCursor(7,1);
  lcd.print(highScoreName);
  lcd.setCursor(7,2);
  lcd.print(highScore);

  messageAnimationFrameCount = ARRAY_LENGTH(characterCelebrating);
  messageAnimation(characterCelebrating, messageAnimationFrameCount,5, 3, 14, 3);
  messageAnimation(characterCelebrating, messageAnimationFrameCount,5, 3, 14, 3);
  messageAnimationFrameCount = ARRAY_LENGTH(characterDancing);
  messageAnimation(characterDancing, messageAnimationFrameCount,5, 3, 14, 3);
  messageAnimation(characterDancing, messageAnimationFrameCount,5, 3, 14, 3);
  messageAnimationFrameCount = ARRAY_LENGTH(characterJumping);
  messageAnimation(characterJumping, messageAnimationFrameCount,5, 3, 14, 3);
  messageAnimation(characterJumping, messageAnimationFrameCount,5, 3, 14, 3);


  delay(2000);
}

// game credits message screen
void creditsMessage() {
  loadCharFromProgmem(2, smiley);
  loadCharFromProgmem(3, heart);
  
  lcd.clear();
  lcd.setCursor(3,1);
  lcd.print("Code, gfx, sfx:");
  lcd.setCursor(3,2);
  lcd.print("Oskari Nyk");
  lcd.print(char(225));
  lcd.print("nen");
  lcd.setCursor(8,3);
  lcd.print("2024");
  lcd.setCursor(0,0); lcd.write(byte(2)); lcd.setCursor(19,0); lcd.write(byte(2));
  lcd.setCursor(0,1);lcd.write(byte(3)); lcd.setCursor(19,1); lcd.write(byte(3));
  lcd.setCursor(0,2); lcd.write(byte(2)); lcd.setCursor(19,2); lcd.write(byte(2));
  lcd.setCursor(0,3);lcd.write(byte(3)); lcd.setCursor(19,3); lcd.write(byte(3));

  messageAnimationFrameCount = ARRAY_LENGTH(characterCelebrating);
  messageAnimation(characterCelebrating, messageAnimationFrameCount,4, 3, 15, 3);
  messageAnimation(characterCelebrating, messageAnimationFrameCount,4, 3, 15, 3);
  messageAnimationFrameCount = ARRAY_LENGTH(characterDancing);
  messageAnimation(characterDancing, messageAnimationFrameCount,4, 3, 15, 3);
  messageAnimation(characterDancing, messageAnimationFrameCount,4, 3, 15, 3);
  messageAnimationFrameCount = ARRAY_LENGTH(characterJumping);
  messageAnimation(characterJumping, messageAnimationFrameCount,4, 3, 15, 3);
  messageAnimation(characterJumping, messageAnimationFrameCount,4, 3, 15, 3);

  delay(6000);

}
