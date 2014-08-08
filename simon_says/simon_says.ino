/**
 * An Arduino circuit design and program that simulates the game "Simon Says".
 *
 * Author: David Timmons ( http://david.timmons.io )
 * License: MIT
 */

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

    // Begin the game.
    // Check if a button switch was pressed.
    int simonLed = getSimonLed(analogRead(BUTTONS));
    if (DEBUG) {
        Serial.print("Button <Analog, Result>: ");
        Serial.print(analogRead(BUTTONS));
        Serial.print(", ");
        Serial.println(simonLed);
    }
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
 * Cycles LEDs on and off in a decorative pattern while waiting for game start.
 */
void playLightShow() {

    // Cycle lights.
    for (int i = 0; i < QTY; i++) {

        // Get the last position in the array.
        int last = QTY - 1 - i;

        // Turn off previous lights.
        if (i-1 >= 0) {
            digitalWrite(LEDS[i-1], LOW);
            digitalWrite(LEDS[last+1], LOW);
        }

        // Turn on current lights.
        digitalWrite(LEDS[i], HIGH);
        digitalWrite(LEDS[last], HIGH);

        // Check the tilt switch for a new game.
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
