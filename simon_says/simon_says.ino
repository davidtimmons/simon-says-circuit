/**
 * An Arduino circuit design and program that simulates the game "Simon Says".
 *
 * Author: David Timmons ( http://david.timmons.io )
 * License: MIT
 */

// Turn debug mode on or off; this toggles serial monitor printing.
const boolean DEBUG = false;

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

    // Create a random "Simon Says" pattern.
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

    // The game is over; turn off all lights and reset.
    delay(LIGHT_DELAY);
    for (int i = 0; i < QTY; i++) {
        digitalWrite(LEDS[i], LOW);
        gameStart = false;
    }
}


/**
 * Return the LED associated with the active button switch.
 *
 * Args:
 *     keyVal: an integer from 3 to 1023 containing the analog switch value.
 *
 * Return:
 *     The corresponding LED from among the colored game diodes.
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
 * Run the "Simon Says" game.
 *
 * Args:
 *     *pattern: a pointer to an integer array containing the game LED pattern.
 */
void playGame(int *pattern) {

    // Set game parameters then begin.
    int lives = TOTAL_LIVES;
    int currentLevel = 1;
    while (gameStart && lives > 0 && currentLevel < TOTAL_LEVELS) {

        // Display the pattern.
        for (int lvl = 0; lvl < currentLevel; lvl++) {

            // Blink the current pattern LED.
            digitalWrite(pattern[lvl], HIGH);
            delay(GAME_SPEED);
            digitalWrite(pattern[lvl], LOW);

            // Do not wait after blinking the final pattern LED.
            if (lvl + 1 != currentLevel) delay(GAME_SPEED);
        }

        // Check player input.
        if (verifyPlayerInput(pattern, currentLevel)) {

            // Check for game reset.
            if (digitalRead(TILT)) {
                gameStart = false;
                signalGameChange(1);
                return;
            }

            // The player was correct; move to the next level.
            currentLevel++;
            delay(GAME_SPEED);

        // The player was incorrect; subtract a life.
        } else {

            // Remove a life from the current total.
            lives--;

            // Turn on the next red LED.
            if (lives == TOTAL_LIVES - 1) digitalWrite(RED_1, HIGH);
            else if (lives == TOTAL_LIVES - 2) digitalWrite(RED_2, HIGH);
            else if (lives == TOTAL_LIVES - 3) digitalWrite(RED_3, HIGH);
            delay(LIGHT_DELAY);
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

        // Check the tilt switch and buttons for a new game.
        if (digitalRead(TILT) || analogRead(BUTTONS) > 3) {
            gameStart = true;
            signalGameChange(2);
            return;
        }

        // Wait before cycling the next set of lights.
        delay(LIGHT_DELAY);
    }

    // Turn off final LED pair.
    digitalWrite(LEDS[0], LOW);
    digitalWrite(LEDS[QTY-1], LOW);
}


/**
 * Blinks all LEDs to signal a change in game state.
 *
 * Args:
 *     blinks: an integer containing the number of times to blink all LEDs.
 */
void signalGameChange(int blinks) {

    // Blink all LEDs to signal a change in game state.
    for (int j = 0; j < blinks; j++) {
        for (int k = 0; k < QTY; k++) {
            digitalWrite(LEDS[k], HIGH);
        }
        delay(LIGHT_DELAY);
        for (int k = 0; k < QTY; k++) {
            digitalWrite(LEDS[k], LOW);
        }
        delay(LIGHT_DELAY * 2);
    }
}


/**
 * Determine whether the player successfully matched the pattern.
 *
 * Args:
 *     *pattern: a pointer to an integer array containing the game LED pattern.
 *     currentLevel: an integer specifying the current game level.
 *
 * Return:
 *     A boolean signaling whether the player correctly entered the current
 *     game pattern.
 */
boolean verifyPlayerInput(int *pattern, int currentLevel) {

    // Get player input up through the current game level.
    int lvl = 0;
    while (lvl < currentLevel) {

        // Wait for a button press.
        int simonLed = -1;
        while (simonLed == -1) {

            // Check for game reset.
            if (digitalRead(TILT)) return true;

            // Check for a button press.
            simonLed = getSimonLed(analogRead(BUTTONS));
        }

        // Blink the corresponding LED.
        digitalWrite(simonLed, HIGH);
        delay(GAME_SPEED);
        digitalWrite(simonLed, LOW);

        // Check if the button press was correct.
        if (pattern[lvl] == simonLed) lvl++;
        else return false;
    }

    // The player successfully matched the entire pattern.
    return true;
}
