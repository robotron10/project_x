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
//#include "/home/froth/project_x/abschlussprojekt/update_limits.h"                               // Auslagern von Funktionen

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
#define HELLIGKEITSSENSOR_UV_LED 10                                                               //  Anschlußpin Helligkeitssensor
#define REEDKONTAKT 11                                                                            //  Anschlußpin REED-Kontakt

// analog
#define ERDFEUCHTESENSOR_1 A0                                                                     //  Anschlußpin Erdfeuchtesensor 1
#define ERDFEUCHTESENSOR_2 A1                                                                     //  Anschlußpin Erdfeuchtesensor 2
#define LIMIT_ERDFEUCHTE_1 A2                                                                     //  Anschlußpin Poti 1
#define LIMIT_ERDFEUCHTE_2 A3                                                                     //  Anschlußpin Poti 2
#define LIMIT_TEMPERATUR A4                                                                       //  Anschlußpin Poti 3
#define LIMIT_LUEFTER A5                                                                          //  Anschlußpin Poti 4
#define HELLIGKEITSSENSOR A6                                                                      //  Anschlußpin Helligkeitssensor

//-------------------------------------------------------
//--- Variablen
//-------------------------------------------------------

// volatile int taster_status = 0;

unsigned long t0_temperaturmessung;                                                               //  Rechenzeit Temperaturmessung
unsigned long dt_temperaturmessung_ms = 2000;                                                     //  Ablaufzeit Temperaturmessung

unsigned long t0_display;                                                                         //  
unsigned long dt_display_ms = 5000;                                                               //  

unsigned long t0_erdfeuchtemessung;                                                               //  
unsigned long dt_erdfeuchtemessung_ms = 5000;                                                     //  

unsigned long t0_pumpe_1;                                                                         //  
unsigned long dt_pumpendauer_1_ms = 5000;                                                         //  

unsigned long t0_pumpe_2;                                                                         //  
unsigned long dt_pumpendauer_2_ms = 5000;                                                         //  

unsigned long t0_durchfeuchtung_1;                                                                //  
unsigned long dt_warten_durchfeuchtung_1_ms = 10000;                                              //

unsigned long t0_durchfeuchtung_2;                                                                //
unsigned long dt_warten_durchfeuchtung_2_ms = 10000;                                              //

unsigned long t0_heizung;                                                                         //  
unsigned long dt_heizung_ms = 5000;                                                               //  

unsigned long t0_luefter;                                                                         //
unsigned long dt_luefter_ms = 5000;                                                               //  

unsigned long t0_helligkeit;                                                                      //
unsigned long dt_helligkeit_ms = 5000;                                                            //

//int limit_feuchte_1_adc = 500;
//int limit_feuchte_2_adc = 500;
int limit_helligkeit = 500;
//int limit_temp_adc = 25;                                                                        //  Lufttemperaturwert -> Heizen

//int limit_luefter_adc = 27;                                                                     //  Lufttemperaturwert -> Lüften
int limit_luefter_C = 15;

int hum_erde_1_adc, hum_erde_2_adc, temp_luft_C, hum_luft, voriger_taster, helligkeit;
int limit_feuchte_1_adc, limit_feuchte_2_adc, limit_temp_adc, limit_temp_C, limit_luefter_adc;
//int hum_erde_1_prozent, hum_erde_2_prozent, limit_feuchte_1_prozent, limit_feuchte_2_prozent, limit_temp_prozent, limit_luefter_prozent;

// für Übergangsautomat
#define zustand_warten_zu_trocken  0
#define zustand_pumpt 1
#define zustand_warten_durchfeuchtung 2
int zustand_1 = zustand_warten_zu_trocken;
int zustand_2 = zustand_warten_zu_trocken;

int aktueller_taster = LOW;
int displ_nr = 0;


//-------------------------------------------------------
// LCD per I2C und die Adresse 0x37
//-------------------------------------------------------

// Windows, auch Linux mit folgender lib: https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
LiquidCrystal_I2C lcd(0x3F, 20, 4 );

// Linux
//LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Temperatur- und Feuchtesensor einrichten (Pin, Typ) 
DHT dht(TEMPERATURSENSOR_LUEFTER, DHTTYPE);

//-------------------------------------------------------
//--- Funktionen
//-------------------------------------------------------


// ISR um die Pumpe manuell ein-/ausschalten zu können
//void taster_unterbricht() {
//  taster_status = digitalRead(TASTER_DISPLAY);                                                //  Tasterstatus einlesen (High/Low)
//  digitalWrite(RELAIS_PUMPE_UNTEN, taster_status);                                            //  Relais schalten
//}

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

  // Werte für das Display mappen
  //hum_erde_1_prozent = map(hum_erde_1_adc, 0, 1023, 0, 100);
  //hum_erde_2_prozent = map(hum_erde_2_adc, 0, 1023, 0, 100);

}
//void update_limits(void);
//------------------------------------------------------
//--- Limits einlesen
//------------------------------------------------------
void update_limits() {
  limit_feuchte_1_adc = analogRead(LIMIT_ERDFEUCHTE_1);                                         //  Werte für den ADC
  limit_feuchte_2_adc = analogRead(LIMIT_ERDFEUCHTE_2);

  limit_temp_adc = analogRead(LIMIT_TEMPERATUR);
  
  limit_luefter_adc = analogRead(LIMIT_LUEFTER);
  limit_luefter_C = map( limit_luefter_adc, 0, 1023, 15, 35 );                                  //  mapping für Poti Grad Celsius

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
  int nr_display,                                                                               //  Übergabe der Vareablen, die in der Funktion vorkommen
  int temp_luft_C, 
  int hum_luft,
  int hum_erde_1_adc, 
  int hum_erde_2_adc, 
  int limit_feuchte_1_adc, 
  int limit_feuchte_2_adc, 
  int limit_temp_adc, 
  int limit_luefter_adc ) {
    

  int hum_erde_1_prozent = map(hum_erde_1_adc, 0, 1023, 0, 100);                                //  mapping für LCD lesbare Werte in %
  int hum_erde_2_prozent = map(hum_erde_2_adc, 0, 1023, 0, 100);
  int limit_feuchte_1_prozent = map(limit_feuchte_1_adc, 0, 1023, 0, 100);
  int limit_feuchte_2_prozent = map(limit_feuchte_2_adc, 0, 1023, 0, 100);
  int limit_temp_prozent = map(limit_temp_adc, 0, 1023, 100, 0);
  int limit_luefter_prozent = map(limit_luefter_adc, 0, 1023, 0, 100);

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
      lcd.setCursor(0, 0);                                                                      //  Displayausgabe erste Zeile
      lcd.print(
        make_string(
          String( "Ist_Erd_1:   " + String(hum_erde_1_prozent) + String( (char)37 )             //  char 37 = "%"
                )
        )
      );

      lcd.setCursor(0, 1);                                                                      //  Displayausgabe zweite Zeile
      lcd.print(
        make_string(
          String( "Ist_Erd_2:   " + String(hum_erde_2_prozent) + String( (char)37 )
                )
        )
      );

      lcd.setCursor(0, 2);                                                                      //  Displayausgabe dritte Zeile
      lcd.print(
        make_string(
          String( "Ist_Temp:    " + String(temp_luft_C) + String( (char)223 )                   //  char 223 = "°" 
                )
        )
      );

      lcd.setCursor(0, 3);                                                                      //  Displayausgabe vierte Zeile
      lcd.print(
        make_string(
          String( "Luftfeuchte: " + String(hum_luft) + String( (char)37 )
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
          String( "Soll_Erd_1: " + String(limit_feuchte_1_prozent) + String( (char)37 )
                )
        )
      );

      lcd.setCursor(0, 1);                                                                      //  Displayausgabe zweite Zeile
      lcd.print(
        make_string(
          String( "Soll_Erd_2: " + String(limit_feuchte_2_prozent) + String( (char)37 )
                )
        )
      );

      lcd.setCursor(0, 2);                                                                      //  Displayausgabe dritte Zeile
      lcd.print(
        make_string(
          String( "Soll_Heiz : " + String(limit_temp_prozent) + String( (char)223 )
                )
        )
      );

      lcd.setCursor(0, 3);                                                                      //  Displayausgabe vierte Zeile
      lcd.print(
        make_string(
          String( "Soll_Fan  : " + String(limit_luefter_prozent) + String( (char)223 )
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

  //lcd.init();                                                                                 //  Windows
  //lcd.backlight();                                                                            //  Windows
  //lcd.begin(20, 4);                                                                           //  Linux
  lcd.begin();                                                                                  //  Linux
  lcd.setCursor(0, 0);
  lcd.print("+++ STARTING...  +++");

  // Temperatursensor
  dht.begin();
  update_messwerte();
  update_limits();

  // Pumpen
  pinMode(RELAIS_PUMPE_UNTEN, OUTPUT);                                                          //  Relais als Ausgang
  digitalWrite(RELAIS_PUMPE_UNTEN, HIGH );                                                      //  HIGH = Relais AUS
  pinMode(RELAIS_PUMPE_OBEN, OUTPUT);                                                           //  Relais Ausgang
  digitalWrite(RELAIS_PUMPE_OBEN, HIGH );                                                       //  HIGH = Relais AUS

  // Lüfter
  pinMode(RELAIS_LUEFTER, OUTPUT);                                                              //  Relais als Ausgang
  digitalWrite(RELAIS_LUEFTER, HIGH );                                                          //  HIGH = Relais AUS

  // Taster
  pinMode(TASTER_DISPLAY, INPUT);

  // REED-Kontakt
  pinMode(REEDKONTAKT, INPUT);                                                                  //  REED-Kontakt als Eingang
  digitalWrite(REEDKONTAKT, HIGH );                                                             //  HIGH = Relais AUS

  // Relais Heizung
  pinMode(RELAIS_HEIZUNG, OUTPUT);                                                              //  Relais als Ausgang
  digitalWrite(RELAIS_HEIZUNG, HIGH );                                                          //  HIGH = Relais AUS

  // Helligkeitssensor
  pinMode(HELLIGKEITSSENSOR_UV_LED, INPUT);                                                     //  Helligkeitssensor als Eingang
  digitalWrite(HELLIGKEITSSENSOR_UV_LED, LOW);                                                  //  Startwert AUS
  //helligkeit = analogRead(HELLIGKEITSSENSOR);                                                   //  Schwelle Helligkeit

  // Relais LEDs
  pinMode(RELAIS_UV_LED, OUTPUT);                                                               //  Relais als Ausgang
  digitalWrite(RELAIS_UV_LED, HIGH);                                                            //  HIGH = Relais AUS
  

  // ISR einrichten
  //  attachInterrupt(0, taster_unterbricht, LOW);
  
  // Zeiten setzen
  t0_heizung = millis();
  t0_luefter = millis();
  t0_display = millis();
  t0_helligkeit = millis();
  t0_temperaturmessung = millis();
  t0_erdfeuchtemessung = millis();

  displ_nr = 0;
  zustand_1 = zustand_warten_zu_trocken;
  zustand_2 = zustand_warten_zu_trocken;

  //taster = digitalRead(TASTER_DISPLAY);                                                     //  Tasterzustand lesen

  lcd.setCursor(0, 1); // Spalte, Zeile
  lcd.print("+++ ...COMPLETE! +++");

  
}



//-------------------------------------------------------
//--- Schleife des Programms
//-------------------------------------------------------

void loop() {

  update_limits();                                                                            //  Werte der Potis am ADC einlesen

  /********************************( Änderung am Taster )*****************************************************/
  aktueller_taster = digitalRead(TASTER_DISPLAY);                                             //  Variable
  if (( voriger_taster == LOW ) and ( aktueller_taster == HIGH)) {                            //  wurde der Tasterstatus geändert
    t0_display = millis() - dt_display_ms;                                                    //  Wenn Taster gedrückt, gleich die Displayausgabe aktualisieren
    displ_nr++;                                                                               //  Displaynummer eins hochzählen
    
    if ( displ_nr > 1 ) {                                                                     //  Wenn Displaynummer größer Eins...
      displ_nr = 0;                                                                           //  ... die Displaynummer Null setzen
    }
    
  }
  voriger_taster = aktueller_taster;

  /********************************( Display Ausgabe )*****************************************************/
  if ( ( millis() - t0_display ) > dt_display_ms ) {                                          //  wenn Displayablaufzeit vergangen, dann...
    t0_display = millis();                                                                    //  Neuer Wert Displayzeit
    update_lcd(                                                                               //  Displaywerte aktualisieren
       displ_nr,                                                                              // Variablen, die übergeben werden, hier angeben/eintragen
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
  if ( ( millis() - t0_temperaturmessung ) > dt_temperaturmessung_ms ) {
    t0_temperaturmessung = millis();                                                          //  Timer auf neuen Wert setzen
    update_messwerte();                                                                       //  Messwerte aktualisieren
  }


  /******************************** Heizung <--> Temperatursensor DHT22 ***********************************/
  if ( ( millis() - t0_heizung ) > dt_heizung_ms ) {
    t0_heizung = millis();
    if (temp_luft_C < limit_temp_C ) {                                                        //  wenn Lufttemperatur kälter als eingesteller Wert, dann
      digitalWrite(RELAIS_HEIZUNG, LOW );                                                     //  heizen
    }
    else {                                                                                    //  sonst
      digitalWrite(RELAIS_HEIZUNG, HIGH );                                                    //  nicht heízen
    }
  }

  /******************************** Lüfter <--> Temperatursensor DHT22 ************************************/
  if ( ( millis() - t0_luefter ) > dt_luefter_ms ) {
    t0_luefter = millis();
    if (temp_luft_C > limit_luefter_C ) {                                                     //  wenn Lufttemperatur wärmer als eingesteller Wert, dann
      digitalWrite(RELAIS_LUEFTER, LOW );                                                     //  Lüfter einschalten (LOW = Relais EIN)
    }
    else {                                                                                    //  sonst
      digitalWrite(RELAIS_LUEFTER, HIGH );                                                    //  Lüfter ausschalten (High = Relais AUS)
    }
  }
 
  /******************************** LED-Licht <--> Helligkeitssensor *************************************/
  if ( ( millis() - t0_helligkeit ) > dt_helligkeit_ms ) {
    t0_helligkeit = millis();
    helligkeit = digitalRead( HELLIGKEITSSENSOR_UV_LED );
  //  if (helligkeit > limit_helligkeit ) {                                                   //  wenn dunkel, dann:
    if ( helligkeit == HIGH ) {
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
      //--- im Zustand bleiben, bis if() erfüllt -> neuer ZUstand
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
