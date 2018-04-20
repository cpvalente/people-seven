#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// ================ | LIBRARIES
#include <Arduino.h>
#include <Wire.h>     // i2c communications
#include <EEPROM.h>
#include "SymbolEncoding.h"

// ================ | SEVEN SEGMENT
#define LATCH_PIN 2	// coincidentally 2 is also the bit in GPIOD for the pin
#define CLOCK_PIN 3
#define DATA_PIN  4

// ================ | COMMS
#define ADDRESS     1 // to be replaced by EEPROM Value, could then be used as memory address
#define DIGITS_SIZE 6 // how many seven segment

#define SS_CHECK    0x21	// !
#define SS_WRBIN    0x00	// Write new value from bin
#define SS_WRVAL    0x01	// Write neww value from char
#define SS_WBRI     0x02	// Set Brightness
#define SS_RBOT     0x03  // Reboot
#define SS_MODE     0x04  // Change mode
#define SS_ACK      0x00  // Do acknowledge
#define SS_NO_ACK   0x01  // Do NOT acknowledge

// ================ | UTIL
#define send(b) shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, b)
#define SCB_AIRCR (*(volatile uint32_t *)0xE000ED0C) // Application Interrupt and Reset Control location
#define charDtoInt(c) (int)(c - 48) // will only be correct for decimals

#endif
