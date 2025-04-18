// pin variables for buttons
const byte upButtonPin = 2;
const byte downButtonPin = 3;
const byte leftButtonPin = 4;
const byte rightButtonPin = 5;
const byte startButtonPin = 6;
const byte actionButtonPin = 7;

// buzzer pin
const byte buzzerPin = 8;

// state variables
bool isPaused = false;
bool climbingMode = false;
bool isAnimating = false;

// main menu
const char* menuItems[] = {
  "Start Game",
  "Instructions",
  "High Score",
  "Credits"
};
const int totalMenuItems = sizeof(menuItems) / sizeof(menuItems[0]);

// variables for buttons
const byte numButtons = 6;
bool buttonStatus[numButtons] = {false, false, false, false, false, false};
bool previousButtonStatus[numButtons] = {false, false, false, false, false, false};
const byte debounceDelay = 20;

// variables for player name
const int maxNameLength = 6;
char playerName[maxNameLength + 1] = "AAAAA";
char highScoreName[maxNameLength + 1];

// LCD variables
const byte objectAnimationInterval = 100;
const byte characterAnimationInterval = 40;
const byte characterAnimationFrameCount = 5;
const byte blockingAnimationInterval = 200;
const byte messageAnimationInterval = 200;
const byte countDownInterval = 40;
const int blinkInterval = 800;
char displayBuffer[20][4];

// misc variables
byte messageAnimationFrameCount;
byte blockingAnimationFrameCount;
byte currentCharacterRow = 0;
byte currentCharacterCol = 0;
byte prevCharacterRow = 0;
byte prevCharacterCol = 0;

// scoring variables
unsigned int maxTime = 30; // maksimiaika, josta saa vielä pisteitä
unsigned int maxTimeBonus = 800; // maksimi aikabonus 

// level variables
unsigned int highScore = 0;
byte currentLevel = 1;
byte itemsCollected = 0;
byte totalItems = 0;
unsigned int itemsBonus = 0;
unsigned int timeBonus = 0;
unsigned int levelScore = 0;
unsigned int totalScore = 0;
unsigned int levelTime = 0;
unsigned int totalTime = 0;
unsigned long levelStartMillis = 0;
unsigned long levelEndMillis;
unsigned long millisInPaused = 0;
unsigned long millisInPausedStart;

// EEPROM variables
const int scoreAddress = 0;
const int nameAddress = 1;
