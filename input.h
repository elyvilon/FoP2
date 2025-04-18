#include <ezButton.h>

ezButton buttonArray[numButtons] = {
	ezButton(upButtonPin), 
	ezButton(downButtonPin), 
	ezButton(leftButtonPin), 
	ezButton(rightButtonPin), 
	ezButton(actionButtonPin),
  ezButton(startButtonPin)
};

// set debounce delay for buttons
void setButtonDebounceTime() {
  for (byte i = 0; i < numButtons; i++) {
    buttonArray[i].setDebounceTime(debounceDelay);
  }
}


// read state of buttons
void checkButtons() {
  for(byte i = 0; i < numButtons; i++) {
    buttonArray[i].loop();
    
    if (buttonArray[i].isPressed() && !previousButtonStatus[i]) {
      buttonStatus[i] = true;
    } else {
      buttonStatus[i] = false;
    }

    previousButtonStatus[i] = buttonArray[i].isPressed();
  }
}


// prototypes required for menuSelectionin
void instructionsMessage();
void highScoreMessage();
void creditsMessage();


// navigation of main menu
void menuSelection() {
  byte currentMenuIndex = 0;
  byte topMenuIndex = 0;
  
  drawMenu(currentMenuIndex, topMenuIndex);

  while (true) {
    checkButtons();

    // if up is pressed, move up
    if (buttonStatus[0] && currentMenuIndex > 0) {
      currentMenuIndex--;
      // except when already at top
      if (currentMenuIndex < topMenuIndex) {
        topMenuIndex = currentMenuIndex;
      }
      playSoundEffect(PAUSED);
      drawMenu(currentMenuIndex, topMenuIndex);
      delay(200);
    }
    else if (buttonStatus[1] && currentMenuIndex < totalMenuItems - 1) {
      currentMenuIndex++;
      if (currentMenuIndex > topMenuIndex + 3) {
        topMenuIndex = currentMenuIndex - 3;
      }
      playSoundEffect(PAUSED);
      drawMenu(currentMenuIndex, topMenuIndex);
      delay(200);
    }

    if (buttonStatus[4]) {
      playSoundEffect(ITEM);
      switch(currentMenuIndex) {
        case 1:
          instructionsMessage();
          break;
        case 2:
          highScoreMessage();
          break;
        case 3:
          creditsMessage();
          break;
      }

      if (currentMenuIndex == 0) break;
      drawMenu(currentMenuIndex, topMenuIndex);
    }
  }
}


// player name entry
void inputPlayerName() {
byte currentCharIndex = 0;

  // show starting state of 'AAAAAA'
  drawPlayerName(currentCharIndex);

  while (true) {

    checkButtons();

    if (buttonStatus[2] && currentCharIndex > 0) {
      currentCharIndex--;
      playSoundEffect(MOVE);
      drawPlayerName(currentCharIndex);
      delay(200);
    }
    else if (buttonStatus[3] && currentCharIndex < maxNameLength - 2) {
      currentCharIndex++;
      playSoundEffect(MOVE);
      drawPlayerName(currentCharIndex);
      delay(200);
    }


    if (buttonStatus[0]) {
      if (playerName[currentCharIndex] == ' ') {
        playerName[currentCharIndex] = 'A';
      } else if (playerName[currentCharIndex] == 'Z') {
        playerName[currentCharIndex] = ' ';
      } else {
        playerName[currentCharIndex]++;
      }
      playSoundEffect(PAUSED);
      drawPlayerName(currentCharIndex);
      delay(200);
    }
    else if (buttonStatus[1]) {
      if (playerName[currentCharIndex] == ' ') {
        playerName[currentCharIndex] = 'Z';
      } else if (playerName[currentCharIndex] == 'A') {
        playerName[currentCharIndex] = ' ';
      } else {
        playerName[currentCharIndex]--;
      }
      playSoundEffect(PAUSED);
      drawPlayerName(currentCharIndex);
      delay(200);
    }

    if (buttonStatus[4]) {
      playSoundEffect(ITEM);
      break;
    }
  }

  playerName[maxNameLength] = '\0';
}
