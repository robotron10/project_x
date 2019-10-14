#include <Arduino.h>
#include "makros.h"


int zustand_1 = zustand_warten_zu_trocken;
unsigned long t0_pumpe_1;
unsigned long dt_pumpendauer_1_ms = 5000;
unsigned long t0_durchfeuchtung_1;                                                                //  
unsigned long dt_warten_durchfeuchtung_1_ms = 10000;

void pumpen_1(){

    switch ( zustand_1 ) {

        case zustand_warten_zu_trocken :
          //--- im Zustand bleiben, bis if() erfüllt -> neuer ZUstand
          if (( hum_erde_1_adc > limit_feuchte_1_adc ) and ( tuer_zu() )) {                       //  wenn Erdfeuchte1 sehr trocken und Tür zu, dann
            digitalWrite(RELAIS_PUMPE_UNTEN, LOW );                                               //  Pumpe an und wässern (LOW = Relais EIN)
            t0_pumpe_1 = millis();                                                                //  neue Zeit setzen
            zustand_1 = zustand_pumpt;                                                            //  neuen Zustand setzen
          }
          break;

        case zustand_pumpt:
          if (( millis() - t0_pumpe_1 > dt_pumpendauer_1_ms ) or ( not tuer_zu() )) {             //  wenn die Pumpzeit (Pumpe an) abgelaufen ist, dann
            digitalWrite(RELAIS_PUMPE_UNTEN, HIGH );                                              //  Pumpe ausschalten (HIGH = Relais AUS)
            t0_durchfeuchtung_1 = millis();                                                       //  neue Zeit setzen
            zustand_1 = zustand_warten_durchfeuchtung;                                            //  neuen Zustand setzen
          }
          break;

        case zustand_warten_durchfeuchtung :
          if (( millis() - t0_durchfeuchtung_1 > dt_warten_durchfeuchtung_1_ms ) or ( not tuer_zu() )) {     //  wenn die Sickerzeit des Wassers abgelaufen ist, dann
            zustand_1 = zustand_warten_zu_trocken;                                                //  neuen Zustand setzen um wieder zu messen, ob die Erde trocken oder feucht ist
          }
          break;
    }

}  
