#include <Arduino.h>
#include "makros.h"
#include "DHT.h"

// Temperatur- und Feuchtesensor einrichten (Pin, Typ)
DHT dht(TEMPERATURSENSOR_LUEFTER, DHTTYPE);

extern int hum_erde_1_adc;
extern int hum_erde_2_adc;
extern int hum_luft;
extern int temp_luft_C;

// aktualisieren der Erdfeuchte-, Luftfeute- und Temperaturwerte
void update_messwerte() {
  hum_erde_1_adc = analogRead( ERDFEUCHTESENSOR_1 );                                            //  Lesen der Erdfeuchtigkeit, Werte zw. 0..1023
  hum_erde_2_adc = analogRead( ERDFEUCHTESENSOR_2 );                                            //  Lesen der Erdfeuchtigkeit, Werte zw. 0..1023
  hum_luft = dht.readHumidity();                                                                //  Lesen der Luftfeuchtigkeit und speichern in die Variable hum_luft
  temp_luft_C = dht.readTemperature();                                                          //  Lesen der Temperatur in °C und speichern in die Variable temp_luft

}
