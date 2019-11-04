#include <Arduino.h>
#include "makros.h"
#include "update_limits.h"

extern int limit_feuchte_1_adc;
extern int limit_feuchte_2_adc;
extern int limit_temp_adc;
extern int limit_temp_C;
extern int limit_luefter_adc;
extern int limit_luefter_C;

// aktualisieren der Schwellenwerte
void update_limits() {
  limit_feuchte_1_adc = analogRead( LIMIT_ERDFEUCHTE_1 );                                        //  Werte für den ADC Erdfeuchtesensor 1
  limit_feuchte_2_adc = analogRead( LIMIT_ERDFEUCHTE_2 );                                        //  Werte für den ADC Erdfeuchtesensor 2

  limit_temp_adc = analogRead( LIMIT_TEMPERATUR );                                               //  Werte für den ADC Temp-Sensor <-> Heizung
  limit_temp_C = map( limit_temp_adc, 0, 1023, 10, 40 );                                         //  mapping für Poti Grad Celsius
  
  limit_luefter_adc = analogRead( LIMIT_LUEFTER );                                               //  Werte für den ADC Temp-Sensor <-> Lüfter
  limit_luefter_C = map( limit_luefter_adc, 0, 1023, 15, 35 );                                   //  mapping für Poti Grad Celsius

}
