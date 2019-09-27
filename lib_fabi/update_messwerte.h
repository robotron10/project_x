//  https://www.arduino.cc/en/Hacking/LibraryTutorial
#ifndef upmess_h
#define upmess_h

#include "Arduino.h"

//  Deklaration
//  Prototypen
class upmess {

   public:
      int hum_erde_1_adc;
      int hum_erde_2_adc;
      int hum_luft;
      int temp_luft_C;
      void update_messwerte();
                          

};
#endif
