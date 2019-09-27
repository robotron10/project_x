#include "Arduino.h"
#include "DHT.h"
#include "update_messwerte.h"


void upmess::update_messwerte() 
{
  
  hum_erde_1_adc = analogRead( ERDFEUCHTESENSOR_1 );  
  hum_erde_2_adc = analogRead( ERDFEUCHTESENSOR_2 );  
  hum_luft = dht.readHumidity();                      
  temp_luft_C = dht.readTemperature();           

}
