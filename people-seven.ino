/*
  NOTES:
  - Had some problems with the wire library in the Master
  Seem to work fine here
  Either way https://www.pjrc.com/teensy/td_libs_Wire.html

  - Receive events are not fully implemented, I2C expects a int32 number
*/

#include "DEFINITIONS.h"

// ================ | COMMS
//const int ADDRESS = EEPROM.read(ADDRESS); //NIU

// ================ | Display
byte digits_b[DIGITS_SIZE];   // Display state
byte brightness = 100;        // Initialize at full brightness
byte cycle;
bool newData;                 // Data flag

void setup() {
  // ================ | INIT I/O for 7 Segment
  pinMode(DATA_PIN,  OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  // ================ | INIT Comms - I2C Inter machine
  Wire.begin(ADDRESS);             // join i2c bus
  Wire.onReceive(receiveEvent);    // register event handlers -NIU

  // ================ | INIT Comms - Serial Debug
  Serial.begin(9600);
  delay(1000);                    // Little sleep
  Serial.println("PEOPLE SEVEN STARTED");

  // ================ | INIT Aux Variables
  for (int i = DIGITS_SIZE; i >= 0 ; --i){
    /* Show DASH on everything if not initialized */
    digits_b[i] = DASH;
  }
  newData = true;
  cycle = 0;
}

void loop(){
  refresh();
}

void refresh(){
  /* Simple fake duty cycle */
  if (brightness == 0) {
    if (newData) updateDisplay();
  } else {
    if (cycle <= brightness) updateDisplay();
    else clear();
  }
  cycle++;
}

void serialEvent() {
  /* Handler for Serial comm in */
  if (Serial.available() > 0) {
    newData = true;
    char check = Serial.peek();  // check byte for formatted data
    if (check != SS_CHECK){
      /* Not formatted data, proceed as int data */
		  if (check != '\n' && check != '\r') {
            int c = Serial.parseInt();  // modify to receive char
            populateDigits(c);
		  } else {
        while (Serial.available())
          Serial.read();              // Discard data ??
		  }
     }

    /* Checked true for formatted data */
    String ss;
    Serial.read();               // discard first byte
    byte ctrl = Serial.read();   // control byte
    byte ACK  = Serial.read();   // should this come after all of it?

    bool bDoRestart = false;
    switch (ctrl){
          int intVal;
          byte byteVal;
          case SS_WRBIN:  // Write new value from binary
          byteVal = Serial.peek();
          intVal = Serial.parseInt();
          ss += "Control 1: Write new value from binary\n";
          ss += "Byte: " + byteVal;
          ss += " Int: " + intVal;
          populateDigits(Serial.parseInt());
          break;

          case SS_WRVAL:  // Write new value from char
          byteVal = Serial.peek();
          intVal = Serial.parseInt();
          ss += "Control 2: Write new value from char\n";
          ss += "Byte: " + byteVal;
          ss += " Int: " + intVal;
          break;

          case SS_WBRI:  // Set Brightness
          brightness = Serial.read();
          ss += "Control 3: Set Bright \n";
          ss += " Val: " + brightness;
          break;

          case SS_RBOT:  // Reboot
          bDoRestart = true;
          ss += "Control 4: Reboot \n";
          break;

          case SS_MODE:  // Change Mode
          // Nothing yet
          ss += "Control 5: Change Mode \n";
          break;

          default:
          /* Trow error */
          ss += "ERROR: control not implemented \n";
          ss += "Control: " + ctrl;
          ss += "\n";
          while (Serial.available()) Serial.read(); // Discard data ??
          break;
        }
    if (ACK == SS_ACK) Serial.println(ss);        // good enough for now
    if (bDoRestart) _softRestart();
  }
}


void populateDigits(int number){
  /* Populate digits from int */
  for (int i = DIGITS_SIZE; i >= 0 ; --i){
    /* Show 8 on everything if not initialized */
    digits_b[i] = NUMERIC[0];
  }

  int count = 1;
  while (number > 0){
    digits_b[DIGITS_SIZE - count] = NUMERIC[number % 10];
    number /= 10;
    count++;
  }
  newData = true;
}

void updateDisplay(){
  /* Send new data to display */
  PORTD &= ~(1<<LATCH_PIN);
  for (int i = DIGITS_SIZE; i >= 0 ; --i){
    send(digits_b[i]); // send data
  }
  PORTD |= (1<<LATCH_PIN);

  newData = false;    // reset flag
}

void allHigh(){
  /* Set all segments to high */
  PORTD &= ~(1<<LATCH_PIN);
  for (int i = DIGITS_SIZE; i >= 0 ; --i){
    send(ALL); // send data
  }
  PORTD |= (1<<LATCH_PIN);
}

void clear(){
  /* Set all segments to low */
  PORTD &= ~(1<<LATCH_PIN);
  for (int i = DIGITS_SIZE; i >= 0 ; --i){
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
   long longReceived;
   byte byteArray[n];

   Serial.println("Reading I2C...");

   for (int i = 0; i < n; i++){
    byteArray[i] = Wire.read();
   }

  longReceived = ( ((unsigned long)byteArray[0] << 24)
               + (  (unsigned long)byteArray[1] << 16)
               + (  (unsigned long)byteArray[2] << 8 )
               + (  (unsigned long)byteArray[3]      ));

  Serial.print("Received: ");
  Serial.println(longReceived);
  Serial.println("Updating screen...");
  populateDigits(longReceived);
}

void updateID(byte val, int address){
  /* Update the module ID in the EEPROM (for i2c comm) */
  EEPROM.write(address, val);
}

void _softRestart(){
  /* Soft restart, Teensy 3.x specific */
  Serial.end();
  SCB_AIRCR = 0x05FA0004;  // restart TEENSY
}
