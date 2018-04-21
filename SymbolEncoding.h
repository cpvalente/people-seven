#ifndef SENCODINGS_H
#define SENCODINGS_H

const byte NUMERIC[] = {
  B11111100,      // 0
  B01100000,      // 1
  B11011010,      // 2
  B11110010,      // 3
  B01100110,      // 4
  B10110110,      // 5
  B10111110,      // 6
  B11100000,      // 7
  B11111110,      // 8
  B11110110       // 9
};

const byte C      = B10011100;
const byte F      = B10001110;
const byte DASH   = B00000010;
const byte ALL    = B11111111;
const byte EMPTY  = B00000000;
const byte DOT    = B00000001;

#endif
