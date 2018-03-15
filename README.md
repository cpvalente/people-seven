# people-seven

/* ================ ================ ================

  People Counter - Receiver Node
  == Program receives a value over i2c and displays in an array of
  == 6 x seven segment displays
  == programmed to run on Teensy 3.2

  Carlos Valente 15.03.18

  =======| TODO
  - Receive value over i2c https://www.pjrc.com/teensy/td_libs_Wire.html
  -- Read values from custom header
  - Add hardware listing to README

  =======| Implemented
  - Serial for debug and comm https://www.pjrc.com/teensy/td_serial.html
  - Create lookup table for numeric characters
  - Write simple values to displays
  -- Use PORT instead of digitalWrite

  =======| EXTEND
  - Real values
  - Different display modes
  - Write ID to EEPROM? https://www.pjrc.com/teensy/td_libs_EEPROM.html
  - i2c dynamic address
  -- ("does anyone have address a, no reply to assign address")
  -- https://forum.arduino.cc/index.php?topic=128361.0
  -- http://www.microchip.com/forums/m729059.aspx
  -- https://github.com/PaulStoffregen/Wire/blob/master/examples/Scanner/Scanner.ino
  -- https://forum.arduino.cc/index.php?topic=37304.0
  - Shift Register https://www.arduino.cc/en/Tutorial/ShiftOut
  - Extend lookup table for alphanumeric characters
  - Extend display for spatial applications
  -- Display is class?


  =======| COMMS - HEADER DESCRIPTION
  - 1. Datatype description
  -- 0x00. Write new value
  -- 0x01. Change Mode
  -- 0x02. Shutdown
  -- 0x03. Reboot
  - 2. Data Size
  - 3. Do acknowledge
  - 4. Data 1
  - n. Data n

    =======| SCREEN MODES
  - 0x00. Display array
  - 0x01. Display Full
  - 0x02. Display Off

    =======| SEGMENTS ARRANGEMENT

    1111111
  6         2
  6         2
  6         2
    7777777
  5         3
  5         3
  5         3
    4444444     88
                88

  ================ ================ ================ */
