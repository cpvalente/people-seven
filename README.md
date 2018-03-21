# people-seven

  People Counter - Receiver Node
  == Program receives a value over i2c and displays in an array of
  == 6 x seven segment displays
  == programmed to run on Teensy 3.2

  Carlos Valente 15.03.18

  =======| TODO
  - Receive value over serial / i2c https://www.pjrc.com/teensy/td_libs_Wire.html
  -- Read values from custom header
  - Implement timers
  -- display duty cycle (fade)
  -- update display
  -- stream values
  - Add hardware listing to README
  - Add file header
  - Extend Symbol Encoding for alphanumeric
  -- Simplified serial receiver should handle char

  =======| Implemented
  - Serial for debug and comm
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
  - Extend lookup table for alphanumeric characters
  - Extend display for spatial applications
  - Fade with Shift PWM
  -- Write faster shiftOut
  -- Display is class?
  - Debug Mode


  =======| COMMS - HEADER DESCRIPTION
  - 0x00. Check
  - 1. Datatype description
  -- 0x00. Write new value (bin)
  -- 0x01. Write new value (char)
  -- 0x02. Set Brightness
  -- 0x03. Reboot
  -- 0x04. Change Mode
  - 2. Do acknowledge
  -- 0x00 no ACK
  -- 0x00 ACK
  - 4. Data byte 1 | Value (Brightness)
  - n. Data byte n

    =======| SCREEN MODES
  - 0x00. Display array
  - 0x01. Display All
  - 0x02. Display None
  - 0x04. DEBUG

    =======| SEGMENTS ARRANGEMENT (MSBFIRST)

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
