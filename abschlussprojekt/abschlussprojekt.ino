////////////////////////////////////////////////////////////////////////////////////////////////
// Datum: 04.11.2019
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
//////////////////////////////////////////////////////////////////////////////////////////////7

#include "makros.h"
#include "tuer_zu.h"
#include "pumpe_1.h"
#include "pumpe_2.h"
#include "update_limits.h"
#include "update_lcd.h"
#include "update_messwerte.h"

//-------------------------------------------------------
//--- Variablen
//-------------------------------------------------------

unsigned long t0_temperaturmessung;                                                               //  Rechenzeit Temperaturmessung
unsigned long dt_temperaturmessung_ms = 2000;                                                     //  Ablaufzeit Temperaturmessung in ms

unsigned long t0_display;                                                                         //  
unsigned long dt_display_ms = 2000;                                                               //  Ablaufzeit Displayaktualisierung in ms

unsigned long t0_erdfeuchtemessung;                                                               //  
unsigned long dt_erdfeuchtemessung_ms = 5000;                                                     //  Ablaufzeit Erdfeuchtemessung

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
int limit_luefter_C;

// für Tasterzustand auslesen
int voriger_taster;
int aktueller_taster = HIGH;

// für Displayumschaltung
int displ_nr = 0;

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
  //zustand_1 = zustand_warten_zu_trocken;                                                        //  Startzustand Übergangsautomat 1 setzen
  //zustand_2 = zustand_warten_zu_trocken;                                                        //  Startzustand Übergangsautomat 2 setzen 

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
    if ( digitalRead( HELLIGKEITSSENSOR ) == HIGH ) {                                         //  wenn dunkel, dann:
      digitalWrite(RELAIS_UV_LED, LOW );                                                      //  Licht anschalten (LOW = Relais EIN) 
    }
    else {                                                                                    //  sonst:
      digitalWrite(RELAIS_UV_LED, HIGH );                                                     //  Licht ausschalten (High = Relais AUS)
    }
  }


  /******************************** Erdfuchtesensor <--> pumpem oben + unten  ***************************************/

  // für die Pumpe unten
  pumpen_1();

  // für die Pumpe oben
  pumpen_2();


}
