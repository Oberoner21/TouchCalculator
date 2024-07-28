/*
  SmartDebug.h
  Simplified debugging by use the arduino framework

  Thanks to Hobbyelektroniker.ch
  https://www.youtube.com/watch?v=MOJGBwsPD7I

  Oberoner21, Version 1.0 vom 28.07.2024
*/

#define DEBUG 1     // 0 disable debugging / 1 enable debugging
#if DEBUG == 1
  #define DEBUG_BEGIN(speed) Serial.begin(speed); while(!Serial)
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
  #define DEBUG_PRINTLN_VALUE(txt, val) Serial.print(txt); Serial.print(": "); Serial.println(val)
#else
  #define DEBUG_BEGIN(speed)
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTLN_VALUE(txt, val)
#endif