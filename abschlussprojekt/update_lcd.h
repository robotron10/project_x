#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "make_string.h"

// I2C Adresse, Spalten, Zeilen
LiquidCrystal_I2C lcd( 0x3F, 20, 4 );

void update_lcd( 
  int nr_display,                                                                                 //  Übergabe der Variablen, die in der Funktion vorkommen
  int temp_luft_C, 
  int hum_luft,
  int hum_erde_1_adc, 
  int hum_erde_2_adc, 
  int limit_feuchte_1_adc, 
  int limit_feuchte_2_adc, 
  int limit_temp_adc, 
  int limit_luefter_adc ) {
    

  int hum_erde_1_prozent = map( hum_erde_1_adc, 0, 1023, 0, 100 );                                //  mapping für LCD lesbare Werte in %
  int hum_erde_2_prozent = map( hum_erde_2_adc, 0, 1023, 0, 100 );
  int limit_feuchte_1_prozent = map( limit_feuchte_1_adc, 0, 1023, 0, 100 );
  int limit_feuchte_2_prozent = map( limit_feuchte_2_adc, 0, 1023, 0, 100 );
  int limit_temp_prozent = map( limit_temp_adc, 0, 1023, 0, 100 );
  int limit_luefter_prozent = map( limit_luefter_adc, 0, 1023, 0, 100 );

/* Anzeigebespiel DisplayNr.0 :
 * 
 * *******************
 * Ist_Erd_1:   50%  *
 * Ist_Erd_1:   70%  *
 * Ist_Temp:    25°  *
 * Luftfeuchte: 65%  *
 * *******************
 * 
 */


  
  switch ( nr_display ) {                                                                       //  

    case 0 :                                                                                    //  Displayinhalt 1
      lcd.setCursor(0, 0);                                                                      //  setzte Cursor erste Spalte/erste Zeile
      lcd.print(                                                                                //  Displayausgabe
        make_string(                                                                            //  baue einen String mit Leerzeichen am Ende
          String( "Ist_Erd_1:   " + String( hum_erde_1_prozent ) + String( (char)37 )           //  Strings verketten, char 37 = "%"
                )
        )
      );

      lcd.setCursor(0, 1);                                                                      //  setzte Cursor erste Spalte/zweite Zeile
      lcd.print(
        make_string(
          String( "Ist_Erd_2:   " + String( hum_erde_2_prozent ) + String( (char)37 )
                )
        )
      );

      lcd.setCursor(0, 2);                                                                      //  setzte Cursor erste Spalte/dritte Zeile
      lcd.print(
        make_string(
          String( "Ist_Temp:    " + String( temp_luft_C ) + String( (char)223 ) + "C"           //  char 223 = "°" 
                )
        )
      );

      lcd.setCursor(0, 3);                                                                      //  setzte Cursor erste Spalte/vierte Zeile
      lcd.print(
        make_string(
          String( "Luftfeuchte: " + String( hum_luft ) + String( (char)37 )
                )
        )
      );

      break;

/* Anzeigebespiel DisplayNr.1 :
 * 
 * *******************
 * Soll_Erd_1:  50%  *
 * Soll_Erd_2:  70%  *
 * Soll_Heiz :  25°  *
 * Soll_Fan:    65%  *
 * *******************
 * 
 */
    default:                                                                                    //  Displayinhalt 2
      lcd.setCursor(0, 0);                                                                      //  Displayausgabe erste Zeile
      lcd.print(
        make_string(
          String( "Soll_Erd_1: " + String( limit_feuchte_1_prozent ) + String( (char)37 )
                )
        )
      );

      lcd.setCursor(0, 1);                                                                      //  Displayausgabe zweite Zeile
      lcd.print(
        make_string(
          String( "Soll_Erd_2: " + String( limit_feuchte_2_prozent ) + String( (char)37 )
                )
        )
      );

      lcd.setCursor(0, 2);                                                                      //  Displayausgabe dritte Zeile
      lcd.print(
        make_string(
          String( "Soll_Heiz : " + String( limit_temp_prozent ) + String( (char)223 ) + "C"
                )
        )
      );

      lcd.setCursor(0, 3);                                                                      //  Displayausgabe vierte Zeile
      lcd.print(
        make_string(
          String( "Soll_Fan  : " + String( limit_luefter_prozent ) + String( (char)223 ) + "C"
                )
        )
      );

      break;

  }
}
