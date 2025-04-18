#include <EEPROM.h>

void readHighScore() {

  // read highscore
  EEPROM.get(scoreAddress, highScore);

  if (highScore == 65535) {
    highScore = 0; // set to 0 if EEPROM is empty
  }

  // read highscore name
  for (int i = 0; i < maxNameLength; i++) {
    highScoreName[i] = EEPROM.read(nameAddress + i);
  }
  highScoreName[maxNameLength] = '\0';
}


void writeHighScore() {

  highScore = totalScore;
  strncpy(highScoreName, playerName, maxNameLength);

  // write high score values to EEPROM
  EEPROM.put(scoreAddress, highScore);

  for (int i = 0; i < maxNameLength; i++) {
    EEPROM.write(nameAddress + i, highScoreName[i]);
  }
}
