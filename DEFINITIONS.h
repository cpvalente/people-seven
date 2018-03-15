#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <Arduino.h>
#include <Wire.h>     // i2c communications
#include <EEPROM.h>
#include "SymbolEncoding.h"

#define LATCH_PIN 2 // coincidentally 2 is also the bit in GPIOD for the pin
#define CLOCK_PIN 3
#define DATA_PIN  4

#define ADDRESS     1 // to be replaced by EEPROM Value, could then be used as memory address
#define DIGITS_SIZE 2 // how many seven segment

#define send(b) shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, b)

#endif
