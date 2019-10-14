#include <Arduino.h>

// diese Funtion wird in "update_lcd.h" benötigt
String make_string( String s ) {                                                                //  erstelle String
  while ( s.length() < 20 ) {                                                                   //  wenn String-Länge kleiner als 20
    s = s + " ";                                                                                //  füge ein Leerzeichen ein
  }
  return s;                                                                                     //  Wert s zurückgeben
}
