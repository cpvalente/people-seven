#include "DEFINITIONS.h"

// ================ | COMMS
//const int ADDRESS = EEPROM.read(ADDRESS); //NIU

// ================ | Display
byte digits_b[DIGITS_SIZE]    = ""; // Dont know yet how to approach it
char digits_char[DIGITS_SIZE] = "";
char buffer[64];

void setup() {

  // ================ | INIT I/O for 7 Segment
  pinMode(DATA_PIN,  OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  // ================ | INIT Comms - I2C Inter machine
  //Wire.setSDA(18);              // Pinout for Teensy 3.2
  //Wire.setSCL(19);
  Wire.begin(ADDRESS);          // join i2c bus

  Wire.onReceive(receiveEvent); // register event handlers
  Wire.onRequest(requestEvent);

  // ================ | INIT Comms - Serial Debug
  Serial.begin(9600);
  while (!Serial){
    ; // Wait for connection
  }
  Serial.println("STARTED");

  // ================ | INIT Aux Variables
  for (int i = 0; i < DIGITS_SIZE; i++){
    /* Show 8 on everything if not initialized */
    digits_b[i]    = NUMERIC[8];;
    digits_char[i] = char(8);
  }

  updateDisplay();

    Serial.println("init");
    Serial.println(digits_b[0]);
    Serial.println(digits_b[1]);
}


void loop(){
  updateDisplay();
  /*
  for (int i = 9; i >= 0; i--){
    PORTD &= ~(1<<LATCH_PIN); // latch low
    send(NUMERIC[i]);
    send(NUMERIC[i]);
    send(NUMERIC[i]);
    send(NUMERIC[i]);
    send(NUMERIC[i]);
    send(NUMERIC[i]);
    send(NUMERIC[i]);
    send(NUMERIC[i]);
    PORTD |= (1<<LATCH_PIN);  // latch high
    delay(500);
  }
  */
}

void serialEvent() {
  /* Handler for Serial comm in */
  while (Serial.available()) {
    int c = Serial.parseInt();

    populateDigits(c);

    Serial.println("after");
    Serial.println(digits_b[0]);
    Serial.println(digits_b[1]);
    updateDisplay();
    
    //if(Serial.peek() == 'l'){  }   // check what type of transmission is this
  }
}

void populateDigits(int number){
  /* Populate digits from int */
  for (int i = 0; i < DIGITS_SIZE; i++){
    /* Show 8 on everything if not initialized */
    digits_b[i] = NUMERIC[0];
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
  for (int i = DIGITS_SIZE; i >= 0 ; i--){
    send(digits_b[i]); // send data
  }
  PORTD |= (1<<LATCH_PIN);
}

void allHigh(){
  /* Set all segments to high */
  PORTD &= ~(1<<LATCH_PIN);
  for (int i = 0; i < DIGITS_SIZE; i++){
    send(ALL); // send data
  }
  PORTD |= (1<<LATCH_PIN);
}

void clear(){
  /* Set all segments to low */
  PORTD &= ~(1<<LATCH_PIN);
  for (int i = 0; i < DIGITS_SIZE; i++){
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
