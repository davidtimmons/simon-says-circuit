/**
 * An Arduino circuit design and program that simulates the game "Simon Says".
 *
 * Author: David Timmons ( http://david.timmons.io )
 * License: MIT
 */

// TODO Reset game in the middle of a game.
// TODO Go back to light show after losing or winning a game.

// Turn debug mode on or off.
const boolean DEBUG = true;

// Define LED pin positions.
const int RED_1 = 13;
const int RED_2 = 12;
const int RED_3 = 11;
const int SIMON_1 = 5;
const int SIMON_2 = 4;
const int SIMON_3 = 3;
const int SIMON_4 = 2;
const int QTY = 7;
const int LEDS[QTY] = {RED_1, RED_2, RED_3, SIMON_1, SIMON_2, SIMON_3, SIMON_4};

// Define switch pin positions.
const int TILT = 10;
const int BUTTONS = A1;

// Define game settings.
const int LIGHT_DELAY = 700;
const int GAME_SPEED = 700;
const int TOTAL_LEVELS = 128;
const int TOTAL_LIVES = 3;
boolean gameStart = false;


/**
 * Initialize the Arduino.
 */
void setup() {

    // Print debug results to the serial monitor.
    if (DEBUG) Serial.begin(9600);

    // Set all LEDs to output mode.
    for (int i = 0; i < QTY; i++) {
        pinMode(LEDS[i], OUTPUT);
    }

    // Set tilt switch to input mode.
    pinMode(TILT, INPUT);
}


/**
 * Control the Arduino.
 */
void loop() {

    // Cycle LEDs until the player starts a game while checking for game start.
    while (!gameStart) {
        playLightShow();  // The tilt switch starts a new game.
    }

    // Create a random 'Simon Says' pattern.
    int pattern[TOTAL_LEVELS];
    for (int i = 0; i < TOTAL_LEVELS; i++) {
        int num = 1 + random(4);  // Random between [1, 4].
        if (num == 1) pattern[i] = SIMON_1;
        else if (num == 2) pattern[i] = SIMON_2;
        else if (num == 3) pattern[i] = SIMON_3;
        else pattern[i] = SIMON_4;
    }

    // Begin the game.
    playGame(pattern);
}


/**
 * Determine whether the player successfully matched the pattern.
 */
boolean verifyPlayerInput(int *pattern, int currentLevel) {

    // Get player input up through the current game level.
    int lvl = 0;
    while (lvl < currentLevel) {

        // Wait for a button press.
        int simonLed = -1;
        while (simonLed == -1) {
            simonLed = getSimonLed(analogRead(BUTTONS));
        }

        // Blink the corresponding LED.
        digitalWrite(simonLed, HIGH);
        delay(GAME_SPEED);
        digitalWrite(simonLed, LOW);
        delay(GAME_SPEED);

        // Check if the button press was correct.
        if (pattern[lvl] == simonLed) {
            lvl++;
        } else {
            // Player input was incorrect.
            return false;
        }
    }

    // The player successfully matched the pattern.
    return true;
}


/**
 * Return the LED associated with the active button switch.
 */
int getSimonLed(int keyVal) {

    // Get the LED.
    if (3 <= keyVal && keyVal <= 50) {
      return SIMON_4;
    } else if (500 <= keyVal && keyVal <= 550) {
      return SIMON_3;
    } else if (1000 <= keyVal && keyVal <= 1010) {
      return SIMON_2;
    } else if (keyVal >= 1020) {
      return SIMON_1;
    }

    // The switch is not active.
    return -1;
}


/**
 * Run the 'Simon Says' game.
 */
void playGame(int *pattern) {

    // Set game parameters then begin.
    int lives = TOTAL_LIVES;
    int currentLevel = 1;
    while (lives > 0 && currentLevel < TOTAL_LEVELS) {

        // Display the pattern.
        for (int lvl = 0; lvl < currentLevel; lvl++) {

            // Blink the LED.
            digitalWrite(pattern[lvl], HIGH);
            delay(GAME_SPEED);
            digitalWrite(pattern[lvl], LOW);

            // Do not wait after blinking the final pattern LED.
            if (lvl + 1 != currentLevel) delay(GAME_SPEED);
        }

        // Check player input.
        if (verifyPlayerInput(pattern, currentLevel)) {

            // The player was correct; move to the next level.
            currentLevel++;

        // The player was incorrect; subtract a life.
        } else {

            // Turn on the next red LED.
            if (lives == TOTAL_LIVES) digitalWrite(RED_1, HIGH);
            else if (lives == TOTAL_LIVES - 1) digitalWrite(RED_2, HIGH);
            else if (lives == TOTAL_LIVES - 2) digitalWrite(RED_3, HIGH);
            delay(LIGHT_DELAY);

            // Remove a life from the current total.
            lives--;
        }
    }
}


/**
 * Cycles LEDs on and off in a decorative pattern while waiting for game start.
 */
void playLightShow() {

    // Cycle lights.
    for (int i = 0; i < QTY; i++) {

        // Get the last position in the array.
        int last = QTY - 1 - i;

        // Turn off previous lights.
        if (i - 1 >= 0) {
            digitalWrite(LEDS[i - 1], LOW);
            digitalWrite(LEDS[last + 1], LOW);
        }

        // Turn on current lights.
        digitalWrite(LEDS[i], HIGH);
        digitalWrite(LEDS[last], HIGH);

        // Check the switches for a new game.
        if (signalGameStart()) return;

        // Wait before cycling the next set of lights.
        delay(LIGHT_DELAY);
    }

    // Turn off final LED pair.
    digitalWrite(LEDS[0], LOW);
    digitalWrite(LEDS[QTY-1], LOW);
}


/**
 * Checks the switches and determines if the game should start.
 */
boolean signalGameStart() {

    // Check the tilt switch and buttons for a new game.
    if (digitalRead(TILT) || analogRead(BUTTONS) > 3) {
        gameStart = true;
    }

    // Flash all LEDs to signal game start if appropriate.
    if (gameStart) {

        // Blink lights twice.
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < QTY; k++) {
                digitalWrite(LEDS[k], HIGH);
            }
            delay(LIGHT_DELAY);
            for (int k = 0; k < QTY; k++) {
                digitalWrite(LEDS[k], LOW);
            }
            delay(LIGHT_DELAY * 2);
        }

        // Signal game start.
        return true;
    }

    // The game has not begun.
    return false;
}
