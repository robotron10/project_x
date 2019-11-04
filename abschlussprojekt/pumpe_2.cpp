#include <Arduino.h>
#include "makros.h"
#include "tuer_zu.h"
#include "pumpe_2.h"

extern int hum_erde_2_adc;
extern int limit_feuchte_2_adc;

unsigned long t0_pumpe_2;
unsigned long dt_pumpendauer_2_ms = 5000;
unsigned long t0_durchfeuchtung_2;                                                                //  
unsigned long dt_warten_durchfeuchtung_2_ms = 10000;

int zustand_2 = zustand_warten_zu_trocken;

void pumpen_2(){

  // für die Pumpe oben
  switch ( zustand_2 ) {

    case zustand_warten_zu_trocken :
      //--- im Zustand bleiben, bis if() erfüllt -> neuer Zustand
      if (( hum_erde_2_adc > limit_feuchte_2_adc) and ( tuer_zu() )) {                        //  wenn Erdfeuchte2 sehr trocken, dann
        digitalWrite(RELAIS_PUMPE_OBEN, LOW );                                                //  Pumpe an und wässern (LOW = Relais EIN)
        t0_pumpe_2 = millis();                                                                //  neue Zeit setzen
        zustand_2 = zustand_pumpt;                                                            //  neuen Zustand setzen
      }
      break;

    case zustand_pumpt:
      if (( millis() - t0_pumpe_2 > dt_pumpendauer_2_ms ) or ( not tuer_zu() )) {             //  wenn die Pumpzeit (Pumpe an) abgelaufen ist, dann
        digitalWrite(RELAIS_PUMPE_OBEN, HIGH );                                               //  Pumpe ausschalten (HIGH = Relais AUS)
        t0_durchfeuchtung_2 = millis();                                                       //  neue Zeit setzen
        zustand_2 = zustand_warten_durchfeuchtung;                                            //  neuen Zustand setzen
      }
      break;

    case zustand_warten_durchfeuchtung :
      if (( millis() - t0_durchfeuchtung_2 > dt_warten_durchfeuchtung_2_ms ) or ( not tuer_zu() )) {     //  wenn die Sickerzeit des Wassers abgelaufen ist, dann
        zustand_2 = zustand_warten_zu_trocken;                                                //  neuen Zustand setzen um wieder zu messen, ob die Erde trocken oder feucht ist
      }
      break;
  }
  
}
