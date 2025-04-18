#include <avr/pgmspace.h>

// macro to count the length of an array
#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

// types of melodies
enum MelodyType {
  START,
  EASY,
  MEDIUM,
  HARD,
  DEFEAT,
  LEVELCOMPLETE,
  GAMECOMPLETE
};

// notes and note durations for melodies
const int startMelody[] PROGMEM = { 262, 294, 311, 349, 311, 294, 262, 196, 220, 233, 262, 233, 220, 196 };
const int startDurations[] PROGMEM = { 300, 300, 300, 400, 300, 300, 600, 300, 300, 300, 400, 300, 300, 600 };

const int easyMelody[] PROGMEM = { 392, 440, 494, 523, 494, 440, 392 };
const int easyDurations[] PROGMEM = { 250, 250, 250, 400, 250, 250, 500 };

const int mediumMelody[] PROGMEM = { 523, 587, 659, 784, 659, 587, 523 };
const int mediumDurations[] PROGMEM = { 200, 200, 200, 300, 200, 200, 400 };

const int hardMelody[] PROGMEM = { 659, 784, 880, 1046, 880, 784, 659 };
const int hardDurations[] PROGMEM = { 150, 150, 150, 250, 150, 150, 300 };

const int defeatMelody[] PROGMEM = { 880, 830, 780, 740, 700, 660, 620 };
const int defeatDurations[] PROGMEM = { 300, 300, 300, 300, 300, 300, 600 };

const int levelCompleteMelody[] PROGMEM = { 523, 587, 659, 523, 659, 587, 523, 392, 440, 494, 392, 523 };
const int levelCompleteDurations[] PROGMEM = { 200, 200, 200, 200, 200, 200, 400, 400, 400, 400, 400, 600 };

const int gameCompleteMelody[] PROGMEM = { 523, 587, 659, 784, 659, 587, 523, 523, 587, 659, 784, 880, 784, 659, 784, 659, 587, 523, 587, 659, 784, 659, 523, 392, 440, 494, 523 };
const int gameCompleteDurations[] PROGMEM = { 200, 200, 200, 300, 200, 200, 400, 200, 200, 200, 300, 200, 200, 400, 200, 200, 200, 300, 200, 200, 400, 300, 300, 300, 300, 300, 600 };


void playMelody(MelodyType melodyType) {
  const int* melody;
  const int* noteDurations;
  int length;

  // Select the correct melody notes, note durations and array length
  switch (melodyType) {
    case START:
      melody = startMelody;
      noteDurations = startDurations;
      length = ARRAY_LENGTH(startMelody);
      break;
    case EASY:
      melody = easyMelody;
      noteDurations = easyDurations;
      length = ARRAY_LENGTH(easyMelody);
      break;
    case MEDIUM:
      melody = mediumMelody;
      noteDurations = mediumDurations;
      length = ARRAY_LENGTH(mediumMelody);
      break;
    case HARD:
      melody = hardMelody;
      noteDurations = hardDurations;
      length = ARRAY_LENGTH(hardMelody);
      break;
    case DEFEAT:
      melody = defeatMelody;
      noteDurations = defeatDurations;
      length = ARRAY_LENGTH(defeatMelody);
      break;
    case LEVELCOMPLETE:
      melody = levelCompleteMelody;
      noteDurations = levelCompleteDurations;
      length = ARRAY_LENGTH(levelCompleteMelody);
      break;
    case GAMECOMPLETE:
      melody = gameCompleteMelody;
      noteDurations = gameCompleteDurations;
      length = ARRAY_LENGTH(gameCompleteMelody);
      break;
  }

  // play the selected melody
  for (int i = 0; i < length; i++) {
    int note = pgm_read_word_near(melody + i);
    int duration = pgm_read_word_near(noteDurations + i);
    tone(buzzerPin, note, duration);
    delay(duration * 1.05);
  }
  // buzzer off
  noTone(buzzerPin);
}


// types of soud effects
enum SoundType {
  PAUSED,
  UNPAUSED,
  OUTOFBOUNDS,
  MOVE,
  BLOCKED,
  CLIMB,
  ITEM,
  FALL,
  IMPACT,
  EMERGE,
  COUNTDOWN,
  COUNTDOWNFINISHED,
  BURN,
  DUST
};

// notes, note durations and array lengths for sound effects
const int pausedSound[] PROGMEM = {1000};
const int pausedSoundDurations[] PROGMEM = {10};
const int pausedSoundLength = ARRAY_LENGTH(pausedSound);

const int unpausedSound[] PROGMEM = {200};
const int unpausedSoundDurations[] PROGMEM = {10};
const int unpausedSoundLength = ARRAY_LENGTH(unpausedSound);

const int itemPickUpSound[] PROGMEM = {400, 600, 800, 1200};
const int itemPickUpSoundDurations[] PROGMEM = {50, 50, 50, 100};
const int itemPickUpSoundLength = ARRAY_LENGTH(itemPickUpSound);

const int emergeSound[] PROGMEM = {200, 0, 400, 0, 600, 0,  800, 0, 1200, 200, 0, 400, 0, 600, 0, 800, 0, 1200};
const int emergeSoundDurations[] PROGMEM = {40, 10, 40, 10, 10, 10, 40, 10, 80, 40, 10, 40, 10, 40, 10, 40, 10, 80};
const int emergeSoundLength = ARRAY_LENGTH(emergeSound);

const int countDownFinishedSound[] PROGMEM = {800, 800, 800, 1200};
const int countDownFinishedSoundDurations[] PROGMEM = {80, 80, 80, 200};
const int countDownFinishedSoundLength = ARRAY_LENGTH(countDownFinishedSound);

const int countDownSound[] PROGMEM = {1000, 600};
const int countDownSoundDurations[] PROGMEM = {30, 30};
const int countDownSoundLength = ARRAY_LENGTH(countDownSound);

const int outOfBoundsSound[] PROGMEM = {100, 100};
const int outOfBoundsSoundDurations[] PROGMEM = {40, 40};
const int outOfBoundsSoundLength = ARRAY_LENGTH(outOfBoundsSound);

const int moveSound[] PROGMEM = {1000, 800};
const int moveSoundDurations[] PROGMEM = {40, 40};
const int moveSoundLength = ARRAY_LENGTH(moveSound);

const int fallSound[] PROGMEM = {1200, 0, 1000, 0, 800, 0, 600, 0, 400};
const int fallSoundDurations[] PROGMEM = {50, 10, 50, 10, 50, 10, 50, 10, 100};
const int fallSoundLength = ARRAY_LENGTH(fallSound);

const int blockedSound[] PROGMEM = {400, 400};
const int blockedSoundDurations[] PROGMEM = {40, 40};
const int blockedSoundLength = ARRAY_LENGTH(blockedSound);

const int climbingSound[] PROGMEM = {100, 0, 1200, 0, 1000, 0, 1200, 0, 100, 0, 1200};
const int climbingSoundDurations[] PROGMEM = {20, 10, 20, 10, 20, 10, 20, 10, 20, 10, 20};
const int climbingSoundLength = ARRAY_LENGTH(climbingSound);

const int impactSound[] PROGMEM = {150, 50, 100};
const int impactSoundDurations[] PROGMEM = {100, 20, 50};
const int impactSoundLength = ARRAY_LENGTH(impactSound);

const int fireSound[] PROGMEM = {600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600};
const int fireSoundDurations[] PROGMEM = {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
const int fireSoundLength = ARRAY_LENGTH(fireSound);

const int dustSound[] PROGMEM = {800, 750, 700, 650, 600, 550, 500, 450, 400, 350, 300, 250, 200, 150, 100};
const int dustSoundDurations[] PROGMEM = {40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 400};
const int dustSoundLength = ARRAY_LENGTH(dustSound);


void playSoundEffect(SoundType soundType) {

  const int* sounds;
  const int* durations;
  int length;

  // select sound effect
  switch (soundType) {
      case PAUSED:
      sounds = pausedSound;
      durations = pausedSoundDurations;
      length = pausedSoundLength;
      break;
      case UNPAUSED:
      sounds = unpausedSound;
      durations = unpausedSoundDurations;
      length = unpausedSoundLength;
      break;
    case OUTOFBOUNDS:
      sounds = outOfBoundsSound;
      durations = outOfBoundsSoundDurations;
      length = outOfBoundsSoundLength;
      break;
    case MOVE:
      sounds = moveSound;
      durations = moveSoundDurations;
      length = moveSoundLength;
      break;
    case BLOCKED:
      sounds = blockedSound;
      durations = blockedSoundDurations;
      length = blockedSoundLength;
      break;
    case CLIMB:
      sounds = climbingSound;
      durations = climbingSoundDurations;
      length = climbingSoundLength;
      break;
    case ITEM:
      sounds = itemPickUpSound;
      durations = itemPickUpSoundDurations;
      length = itemPickUpSoundLength;
      break;
    case FALL:
      sounds = fallSound;
      durations = fallSoundDurations;
      length = fallSoundLength;
      break;
    case IMPACT:
      sounds = impactSound;
      durations = impactSoundDurations;
      length = impactSoundLength;
      break;
    case EMERGE:
      sounds = emergeSound;
      durations = emergeSoundDurations;
      length = emergeSoundLength;
      break;
    case COUNTDOWN:
      sounds = countDownSound;
      durations = countDownSoundDurations;
      length = countDownSoundLength;
      break;
    case COUNTDOWNFINISHED:
      sounds = countDownFinishedSound;
      durations = countDownFinishedSoundDurations;
      length = countDownFinishedSoundLength;
      break;
    case BURN:
      sounds = fireSound;
      durations = fireSoundDurations;
      length = fireSoundLength;
      break;
    case DUST:
      sounds = dustSound;
      durations = dustSoundDurations;
      length = dustSoundLength;
      break;
  }

  // play sound effect
  for (int i = 0; i < length; i++) {
    int note = pgm_read_word_near(sounds + i);
    int duration = pgm_read_word_near(durations + i);
    tone(buzzerPin, note, duration);
    delay(duration * 1.05);
  }
  // buzzer off
  noTone(buzzerPin);
}


// climbing mode on sound effect
void climbingModeOnSound() {
    tone(buzzerPin, 1200, 100);
}


// climbing mode off sound effect
void climbingModeOffSound() {
    tone(buzzerPin, 600, 100);
}
