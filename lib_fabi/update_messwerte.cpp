#include "Arduino.h"
#include "DHT.h"
#include "update_messwerte.h"


/*um::Morse(int pin) {

   _hum_erde_1_adc = hum_erde_1_adc;
   _hum_erde_2_adc = hum_erde_2_adc;
   _hum_luft = hum_luft;
   _temp_luft_C = temp_luft_C;

}
*/

void um::update_messwerte() {
  
  hum_erde_1_adc = analogRead( ERDFEUCHTESENSOR_1 );  
  hum_erde_2_adc = analogRead( ERDFEUCHTESENSOR_2 );  
  hum_luft = dht.readHumidity();                      
  temp_luft_C = dht.readTemperature();           

}
