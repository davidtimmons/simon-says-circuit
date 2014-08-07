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

// Define switch pin positions.
const int TILT = 10;
const int BUTTONS = A1;


/**
 * Initialize the Arduino.
 */
void setup() {
    // Print debug results to the serial monitor.
    if (DEBUG) Serial.begin(9600);
}


/**
 * Control the Arduino.
 */
void loop() {
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
