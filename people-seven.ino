#include "DEFINITIONS.h"

// ================ | COMMS
//const int ADDRESS = EEPROM.read(ADDRESS); //NIU

// ================ | Display
byte digits_b[DIGITS_SIZE]    = ""; // Display state
byte brightness = 255;              // initialize at full brightness

void setup() {

  // ================ | INIT I/O for 7 Segment
  pinMode(DATA_PIN,  OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  // ================ | INIT Comms - I2C Inter machine
  //Wire.setSDA(18);            // Pinout for Teensy 3.2
  //Wire.setSCL(19);            // Not compatible with arduino library
  Wire.begin(ADDRESS);          // join i2c bus
  Wire.onReceive(receiveEvent); // register event handlers
  Wire.onRequest(requestEvent);

  // ================ | INIT Comms - Serial Debug
  Serial.begin(38400);
  while (!Serial){ ; } // Wait for connection
  Serial.println("STARTED");

  // ================ | INIT Aux Variables
  for (int i = 0; i < DIGITS_SIZE; ++i){
    /* Show - on everything if not initialized */
    digits_b[i] =  DASH;
  }
  updateDisplay();
}


void loop(){

  serialEvent(); // TEMPORARY: FEATHER DOESNT SUPPORT SERIAL EVENTS

}

void serialEvent() {
  /* Handler for Serial comm in */
  while (Serial.available()) {

    byte check = Serial.peek();  // check byte for formatted data
    Serial.println("BYTE RECEIVED: " + check);

    if (check != 0x00){
      /* Not formatted data, proceed as int data */
      int c = Serial.parseInt();  // should be modified to receive char
      populateDigits(c);

    } else {
      /* Checked true for formatted data */
      String ss = "";
      Serial.read();  // discard first byte
      byte ctrl = Serial.read();   // control byte
      byte ACK  = Serial.read();   // should this come after all of it?

      switch (ctrl){
        case 0x00:  // Write new value from binary
          int g;
          byte bf;
          bf = Serial.peek();
          g = Serial.parseInt();
          ss += "Control 1: Write new value \n";
          ss += "Byte: " + bf;
          ss += " Int: " + g;
          populateDigits(Serial.parseInt());
        break;

        case 0x01:  // Write new value from char
          int s;
          byte sa;
          bf = Serial.peek();
          g = Serial.parseInt();
          ss += "Control 2: Write new value \n";
          ss += "Byte: " + sa;
          ss += " Int: " + s;
        break;

        case 0x02:  // Set Brightness
          brightness = Serial.read();
          ss += "Control 3: Set Bright \n";
          ss += " Val: " + brightness;
        break;

        case 0x03:  // Reboot
          ss += "Control 4: Change Mode \n";
        break;

        case 0x04:  // Change Mode
          // Nothing yet
          ss += "Control 5: Change Mode \n";
        break;

        default:
          /* Trow error */
          ss += "ERROR: control not implemented \n";
        break;
        while (Serial.available()) Serial.read(); // Discard data ??
      }
      if (ACK == 0x00) Serial.println(ss);  // good enough for now
    }
  }
}

void populateDigits(int number){
  /* Populate digits from int */
  for (int i = 0; i < DIGITS_SIZE; ++i){
    /* Show 8 on everything if not initialized */
    digits_b[i] = NUMERIC[8];
  }

  int count = 1;
  while (number > 0){
    digits_b[DIGITS_SIZE - count] = NUMERIC[number % 10];
    number /= 10;
    count++;
  }
}

void updateDisplay(){
  /* Send new data to display */
  PORTD &= ~(1<<LATCH_PIN);
  for (int i = DIGITS_SIZE; i >= 0 ; --i){
    send(digits_b[i]); // send data
  }
  PORTD |= (1<<LATCH_PIN);
}

void allHigh(){
  /* Set all segments to high */
  PORTD &= ~(1<<LATCH_PIN);
  for (int i = 0; i < DIGITS_SIZE; ++i){
    send(ALL); // send data
  }
  PORTD |= (1<<LATCH_PIN);
}

void clear(){
  /* Set all segments to low */
  PORTD &= ~(1<<LATCH_PIN);
  for (int i = 0; i < DIGITS_SIZE; ++i){
    send(EMPTY); // send data
  }
  PORTD |= (1<<LATCH_PIN);
}

byte intToByte(int i){
  /* Call lookup table for integers */
  if (i > 0 && i < 9) return NUMERIC[i];
  else return DASH;
}

void receiveEvent(int n){
  /* Handler for when a message is received */
  /* THIS IS A PLACE HOLDER, IMPLEMENTATION
   * SHOULD BE THE SAME AS SERIAL EVENT */
  while (1 < Wire.available()) {  // loop through all but the last
    char c = Wire.read();         // receive byte as a character
    Serial.print(c);
  }
  int x = Wire.read();           // receive byte as an integer
  Serial.println(x);
}

void requestEvent(){
  /* Handler for request events */
  Wire.write("nothing yet");
  Serial.println("requested");
}

void updateID(byte val, int address){
  /* Update the module ID in the EEPROM (for i2c comm) */
  EEPROM.write(address, val);
}

void _softRestart(){
  /* Soft restart, Teensy 3.x specific */
  Serial.end();
  Wire.end();
  SCB_AIRCR = 0x05FA0004;  // restart TEENSY
}

void printLoop(){
  /* Send character 0 -9 to all digits */
  for (int i = 9; i >= 0; --i){
    PORTD &= ~(1<<LATCH_PIN); // latch low

    for (int i = DIGITS_SIZE; i >= 0; --i){
      send(NUMERIC[i]);
    }

    PORTD |= (1<<LATCH_PIN);  // latch high
    delay(500);               // Bad c
  }
}
