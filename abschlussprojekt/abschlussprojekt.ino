// Datum: 10.09.2019
// Autor: Stephanie Nawroth, Torsten Graf, Fabian Roth
//
// Thema: vollautomatisches Gewächshaus
//
// Arduino IDE 1.8.9
// DHT Sensor Library 1.3.7 + https://github.com/adafruit/Adafruit_Sensor
//
// Liquid Crystal I2C https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
// mkdir ~/Arduino/libraries/LiquidCrystal_I2C_master
//

#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include "update_messwerte.h"

//-------------------------------------------------------
//--- Makros -> Anschlußpins der Peripherie am Arduino
//-------------------------------------------------------

//  digital
#define DHTTYPE DHT22                                                                             //  Typ des Temperatursensors  
#define TASTER_DISPLAY 2                                                                          //  Anschlußpin Taster Display umschalten (Interrupt-PIN)
#define RELAIS_PUMPE_OBEN 4                                                                       //  Anschlußpin Relais 1 Wasserpumpe oben
#define RELAIS_PUMPE_UNTEN 5                                                                      //  Anschlußpin Relais 2 Wasserpumpe unten
#define RELAIS_LUEFTER 6                                                                          //  Anschlußpin Relais 3 Lüfter
#define RELAIS_UV_LED 7                                                                           //  Anschlußpin Relais 4 Licht (LED-Streifen)
#define RELAIS_HEIZUNG 8                                                                          //  Anschlußpin Relais 5 Heizfolie
#define TEMPERATURSENSOR_LUEFTER 9                                                                //  Anschlußpin Temperatursensor (DHT22)
#define HELLIGKEITSSENSOR 10                                                                      //  Anschlußpin Helligkeitssensor für LED-Steuerung
#define REEDKONTAKT 11                                                                            //  Anschlußpin REED-Kontakt

// analog
#define ERDFEUCHTESENSOR_1 A0                                                                     //  Anschlußpin Erdfeuchtesensor 1
#define ERDFEUCHTESENSOR_2 A1                                                                     //  Anschlußpin Erdfeuchtesensor 2
#define LIMIT_ERDFEUCHTE_1 A2                                                                     //  Anschlußpin Poti 1
#define LIMIT_ERDFEUCHTE_2 A3                                                                     //  Anschlußpin Poti 2
#define LIMIT_TEMPERATUR A6                                                                       //  Anschlußpin Poti 3
#define LIMIT_LUEFTER A5                                                                          //  Anschlußpin Poti 4

//-------------------------------------------------------
//--- Variablen
//-------------------------------------------------------

unsigned long t0_temperaturmessung;                                                               //  Rechenzeit Temperaturmessung
unsigned long dt_temperaturmessung_ms = 2000;                                                     //  Ablaufzeit Temperaturmessung in ms

unsigned long t0_display;                                                                         //  
unsigned long dt_display_ms = 2000;                                                               //  Ablaufzeit Displayaktualisierung in ms

unsigned long t0_erdfeuchtemessung;                                                               //  
unsigned long dt_erdfeuchtemessung_ms = 5000;                                                     //  Ablaufzeit Erdfeuchtemessung

unsigned long t0_pumpe_1;                                                                         //  
unsigned long dt_pumpendauer_1_ms = 5000;                                                         //  Ablaufzeit 

unsigned long t0_pumpe_2;                                                                         //  
unsigned long dt_pumpendauer_2_ms = 5000;                                                         //  Ablaufzeit 

unsigned long t0_durchfeuchtung_1;                                                                //  
unsigned long dt_warten_durchfeuchtung_1_ms = 10000;                                              //  Ablaufzeit 

unsigned long t0_durchfeuchtung_2;                                                                //
unsigned long dt_warten_durchfeuchtung_2_ms = 10000;                                              //

unsigned long t0_heizung;                                                                         //  
unsigned long dt_heizung_ms = 5000;                                                               //  

unsigned long t0_luefter;                                                                         //
unsigned long dt_luefter_ms = 5000;                                                               //  

unsigned long t0_helligkeit;                                                                      //
unsigned long dt_helligkeit_ms = 5000;                                                            //

//  für Einlesen der Sensoren
int hum_erde_1_adc;
int hum_erde_2_adc;
int hum_luft;
int temp_luft_C;

//  für Schwellen aus Potis
int limit_feuchte_1_adc;
int limit_feuchte_2_adc;
int limit_temp_adc;
int limit_temp_C;
int limit_luefter_adc;
int limit_luefter_C = 15;

// für Übergangsautomat
#define zustand_warten_zu_trocken  0
#define zustand_pumpt 1
#define zustand_warten_durchfeuchtung 2
int zustand_1 = zustand_warten_zu_trocken;
int zustand_2 = zustand_warten_zu_trocken;

// für Tasterzustand auslesen
int voriger_taster;
int aktueller_taster = HIGH;

// für Displayumschaltung
int displ_nr = 0;


//-------------------------------------------------------
// LCD per I2C und die Adresse 0x37
//-------------------------------------------------------
LiquidCrystal_I2C lcd( 0x3F, 20, 4 );

// Temperatur- und Feuchtesensor einrichten (Pin, Typ) 
DHT dht(TEMPERATURSENSOR_LUEFTER, DHTTYPE);

//-------------------------------------------------------
//--- Funktionen
//-------------------------------------------------------

//------------------------------------------------------
//--- String ausgeben
//------------------------------------------------------
String make_string( String s ) {                                                                //  erstelle String
  while ( s.length() < 20 ) {                                                                   //  wenn String-Länge kleiner als 20
    s = s + " ";                                                                                //  füge ein Leerzeichen ein
  }
  return s;                                                                                     //  Wert s zurückgeben
}

//------------------------------------------------------
//--- Messwerte erfassen
//------------------------------------------------------
void update_messwerte() {
  hum_erde_1_adc = analogRead( ERDFEUCHTESENSOR_1 );                                            //  Lesen der Erdfeuchtigkeit, Werte zw. 0..1023
  hum_erde_2_adc = analogRead( ERDFEUCHTESENSOR_2 );                                            //  Lesen der Erdfeuchtigkeit, Werte zw. 0..1023
  hum_luft = dht.readHumidity();                                                                //  Lesen der Luftfeuchtigkeit und speichern in die Variable hum_luft
  temp_luft_C = dht.readTemperature();                                                          //  Lesen der Temperatur in °C und speichern in die Variable temp_luft

}

//------------------------------------------------------
//--- Limits einlesen
//------------------------------------------------------
void update_limits() {
  limit_feuchte_1_adc = analogRead( LIMIT_ERDFEUCHTE_1 );                                        //  Werte für den ADC Erdfeuchtesensor 1
  limit_feuchte_2_adc = analogRead( LIMIT_ERDFEUCHTE_2 );                                        //  Werte für den ADC Erdfeuchtesensor 2

  limit_temp_adc = analogRead( LIMIT_TEMPERATUR );                                               //  Werte für den ADC Temp-Sensor <-> Heizung
  limit_temp_C = map( limit_temp_adc, 0, 1023, 10, 40 );                                         //  mapping für Poti Grad Celsius
  
  limit_luefter_adc = analogRead( LIMIT_LUEFTER );                                               //  Werte für den ADC Temp-Sensor <-> Lüfter
  limit_luefter_C = map( limit_luefter_adc, 0, 1023, 15, 35 );                                   //  mapping für Poti Grad Celsius

}

//------------------------------------------------------
//--- REED-Kontakt auswerten
//------------------------------------------------------
bool tuer_zu() {                                                                                //  Funktion: Ist die Tür zu?
  return ( digitalRead( REEDKONTAKT ) == LOW );                                                 //  REED-Kontakt gleich Null?
}

//------------------------------------------------------
//--- Anzeige aktualisieren
//------------------------------------------------------
void update_lcd( 
  int nr_display,                                                                               //  Übergabe der Variablen, die in der Funktion vorkommen
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

//------------------------------------------------------
//--- Setup des Programms, wird nur einmal ausgeführt
//------------------------------------------------------
void setup() {

  lcd.begin();                                                                                  //  LCD Initialisierung
  lcd.setCursor(0, 0);                                                                          //  Setze Cursor auf Position
  lcd.print("+++ STARTING...  +++");                                                            //  Ausgabe aufs LCD

  update_messwerte();                                                                           //  Messwerte einlesen
  update_limits();                                                                              //  Schwellen von den Potis auslesen
  
  // Temperatursensor
  dht.begin();                                                                                  //  Initialisierung DHT-Sensors

  // Taster
  pinMode(TASTER_DISPLAY, INPUT_PULLUP);                                                        //  Taster als Eingang, interner Pullup (36k) ein

  // REED-Kontakt
  pinMode(REEDKONTAKT, INPUT_PULLUP);                                                           //  REED-Kontakt als Eingang, interner Pullup (36k) ein
  //digitalWrite(REEDKONTAKT, HIGH );                                                             //  Pull-up ein (36k)
  
  // Helligkeitssensor
  pinMode(HELLIGKEITSSENSOR, INPUT);                                                            //  Helligkeitssensor als Eingang
  
  // Pumpen
  pinMode(RELAIS_PUMPE_UNTEN, OUTPUT);                                                          //  Relais als Ausgang
  digitalWrite(RELAIS_PUMPE_UNTEN, HIGH );                                                      //  HIGH = Relais AUS
  pinMode(RELAIS_PUMPE_OBEN, OUTPUT);                                                           //  Relais Ausgang
  digitalWrite(RELAIS_PUMPE_OBEN, HIGH );                                                       //  HIGH = Relais AUS

  // Lüfter
  pinMode(RELAIS_LUEFTER, OUTPUT);                                                              //  Relais als Ausgang
  digitalWrite(RELAIS_LUEFTER, HIGH );                                                          //  HIGH = Relais AUS

  // Relais Heizung
  pinMode(RELAIS_HEIZUNG, OUTPUT);                                                              //  Relais als Ausgang
  digitalWrite(RELAIS_HEIZUNG, HIGH );                                                          //  HIGH = Relais AUS

  // Relais LEDs
  pinMode(RELAIS_UV_LED, OUTPUT);                                                               //  Relais als Ausgang
  digitalWrite(RELAIS_UV_LED, HIGH);                                                            //  HIGH = Relais AUS


  // Zeiten setzen
  t0_heizung = millis();                                                                        //  Startzeit Timer Heizung
  t0_luefter = millis();                                                                        //  Startzeit Timer Lüfter
  t0_display = millis();                                                                        //  Startzeit Timer Display
  t0_helligkeit = millis();                                                                     //  Startzeit Timer Helligkeit
  t0_temperaturmessung = millis();                                                              //  Startzeit Timer Temp-Messung
  t0_erdfeuchtemessung = millis();                                                              //  Startzeit Timer Erdfeuchtemessung

  displ_nr = 0;                                                                                 //  Startwert Displaynummer 
  zustand_1 = zustand_warten_zu_trocken;                                                        //  Startzustand Übergangsautomat 1 setzen
  zustand_2 = zustand_warten_zu_trocken;                                                        //  Startzustand Übergangsautomat 2 setzen 

  lcd.setCursor(0, 1); // Spalte, Zeile                                                         //  Setze Cursor auf Position 
  lcd.print("+++ ...COMPLETE! +++");                                                            //  Ausgabe aufs LCD 

  
}



//-------------------------------------------------------
//--- Schleife des Programms
//-------------------------------------------------------

void loop() {

  update_limits();                                                                            //  Werte der Potis am ADC einlesen

  /********************************( Änderung am Taster )*****************************************************/
  aktueller_taster = digitalRead(TASTER_DISPLAY);                                             //  Variable
  if (( voriger_taster == HIGH ) and ( aktueller_taster == LOW)) {                            //  wurde der Tasterstatus geändert
    t0_display = millis() - dt_display_ms;                                                    //  Wenn Taster gedrückt, gleich die Displayausgabe aktualisieren
    displ_nr++;                                                                               //  Displaynummer eins hochzählen
    
    if ( displ_nr > 1 ) {                                                                     //  Wenn Displaynummer größer Eins...
      displ_nr = 0;                                                                           //  ... die Displaynummer Null setzen
    }
    
  }
  voriger_taster = aktueller_taster;

  /********************************( Display Ausgabe )*****************************************************/
  if ( ( millis() - t0_display ) > dt_display_ms ) {                                          //  wenn Displayablaufzeit vergangen, dann...
    t0_display = millis();                                                                    //  neuer Wert Displayzeit
    update_lcd(                                                                               //  Displaywerte aktualisieren
       displ_nr,                                                                              //  Variablen, die übergeben werden, hier angeben/eintragen
       temp_luft_C,
       hum_luft,
       hum_erde_1_adc,
       hum_erde_2_adc,
       limit_feuchte_1_adc,
       limit_feuchte_2_adc, 
       limit_temp_adc, 
       limit_luefter_adc 
       );
  }

  /******************************** Messen Temperatur und Feuchtigkeit ***********************************/
  if ( ( millis() - t0_temperaturmessung ) > dt_temperaturmessung_ms ) {                      //  wenn Zeit für Temp-Messung abgelaufen, dann
    t0_temperaturmessung = millis();                                                          //  Timer auf neuen Wert setzen
    update_messwerte();                                                                //  Messwerte aktualisieren
     
  }


  /******************************** Heizung <--> Temperatursensor DHT22 ***********************************/
  if ( ( millis() - t0_heizung ) > dt_heizung_ms ) {                                          //  wenn Zeit für Heizen abgelaufen, dann
    t0_heizung = millis();                                                                    //  Timer auf neuen Wert setzen
    if (temp_luft_C < limit_temp_adc ) {                                                        //  wenn Lufttemperatur kälter als eingesteller Wert, dann
      digitalWrite(RELAIS_HEIZUNG, LOW );                                                     //  heizen
    }
    else {                                                                                    //  sonst
      digitalWrite(RELAIS_HEIZUNG, HIGH );                                                    //  nicht heízen
    }
  }

  /******************************** Lüfter <--> Temperatursensor DHT22 ************************************/
  if ( ( millis() - t0_luefter ) > dt_luefter_ms ) {
    t0_luefter = millis();
    if (temp_luft_C > limit_luefter_adc ) {                                                     //  wenn Lufttemperatur wärmer als eingesteller Wert, dann
      digitalWrite(RELAIS_LUEFTER, LOW );                                                     //  Lüfter einschalten (LOW = Relais EIN)
    }
    else {                                                                                    //  sonst
      digitalWrite(RELAIS_LUEFTER, HIGH );                                                    //  Lüfter ausschalten (High = Relais AUS)
    }
  }
 
  /******************************** LED-Licht <--> Helligkeitssensor *************************************/
  if ( ( millis() - t0_helligkeit ) > dt_helligkeit_ms ) {
    t0_helligkeit = millis();
    if ( digitalRead( HELLIGKEITSSENSOR ) == HIGH ) {                                         //  wenn dunkel, dann:
      digitalWrite(RELAIS_UV_LED, LOW );                                                      //  Licht anschalten (LOW = Relais EIN) 
    }
    else {                                                                                    //  sonst:
      digitalWrite(RELAIS_UV_LED, HIGH );                                                     //  Licht ausschalten (High = Relais AUS)
    }
  }


  /******************************** Erdfuchtesensor <--> pumpem oben + unten  ***************************************/
  /*


    // --- Übergangsautomat Erdfeuchte:
    1. (warten)
      warten auf "zu trocken", d.h. bis Feuchtigkeit zu niedrig.
      Wenn zu trocken; Pumpe ein, Statzeit für Pumpe notieren
      Zustand ändern in -> pumpen
    2. (pumpen)
      pumpen, bis Pumpzeit abgelaufen
      Pumpe aus, Startzeit für "warten auf Durchfeuchtung" notieren,
      Zustand ändern in -> "warten auf Durchfeuchtung" ändern
    3. (warten auf durchfeuchtung)
     wenn Zeit für "warten auf Durchfeuchtung" abgelaufen -> warten auf zu trocken
  */
  
  // für die Pumpe unten
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
