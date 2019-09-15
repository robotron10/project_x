#include "update_messwerte.h"
#include "DHT.h"
#include "Arduino.h"

#define ERDFEUCHTESENSOR_1 A0                                                                     //  Anschlußpin Erdfeuchtesensor 1
#define ERDFEUCHTESENSOR_2 A1                                                                     //  Anschlußpin Erdfeuchtesensor 2

int hum_erde_1_adc;
int hum_erde_2_adc;
int hum_luft;
int temp_luft_C;

void update_messwerte(void) {
  
  hum_erde_1_adc = analogRead( ERDFEUCHTESENSOR_1 );  //  Lesen der Erdfeuchtigkeit, Werte zw. 0..1023
  hum_erde_2_adc = analogRead( ERDFEUCHTESENSOR_2 );  //  Lesen der Erdfeuchtigkeit, Werte zw. 0..1023
  hum_luft = dht.readHumidity();                      //  Lesen der Luftfeuchtigkeit und speichern in die Variable hum_luft
  temp_luft_C = dht.readTemperature();                //  Lesen der Temperatur in °C und speichern in die Variable temp_luft

}
