simon-says-circuit
==================
_**Author:** David Timmons (http://david.timmons.io)_

An Arduino circuit design and program that simulates the game "Simon Says".


## Contents
This _Arduino_ project is similar to the electronic memory games made popular
in the 1980s. The program creates a randomized sequence of progressively complex
LED flashes. A player must memorize the pattern and input the correct sequence.

This version gives the player three chances. After entering an incorrect pattern
three times, the game is over. The player can reset the game by moving the
board to activate the tilt switch.


## Components
_Microcontroller:_
* (x1) _Arduino Uno_

_Resistors:_
* (x8) 220 Ohm
* (x3) 10K Ohm
* (x1) 1M Ohm

_Diodes:_
* (x3) Red LED
* (x1) Green LED
* (x1) Yellow LED
* (x1) Clear LED
* (x1) Blue LED

_Switches:_
* (x4) Push button
* (x1) Tilt switch


## Screenshots
**Circuit Schematic**

![circuit schematic](https://raw.githubusercontent.com/davidtimmons/simon-says-circuit/master/simon_says/simon-says-circuit-schematic-20140808.png "'Simon Says' Circuit Schematic")

**Breadboard**

![breadboard](https://raw.githubusercontent.com/davidtimmons/simon-says-circuit/master/simon_says/simon-says-breadboard-20140808.jpg "'Simon Says' Breadboard")
