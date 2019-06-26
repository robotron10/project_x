// Datum: 12.06.2019
// Autor: Stephanie Nawroth, Torsten Graf, Fabian Roth
//
// Thema: vollautomatisches Gewächshaus

#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//-------------------------------------------------------
//--- Makros -> Anschlußpins der Peripherie
//-------------------------------------------------------

#define DHTTYPE DHT22                     // Typ des Temperatur- und Feuchtesensors  
#define TASTER_PUMPE_OBEN 2               // Anschlußpin für den Taster zum Einschalten der Wasserpumpe oben mit ISR                     
#define TASTER_PUMPE_UNTEN 3              // Anschlußpin für den Taster zum Einschalten der Wasserpumpe unten mit ISR
#define RELAIS_PUMPE_OBEN 4               // Anschlußpin für das Relais, welches die Wasserpumpe oben steuert
#define RELAIS_PUMPE_UNTEN 5              // Anschlußpin für das Relais, welches die Wasserpumpe unten steuert
#define ERDFEUCHTESENSOR_1 A0             // Anschlußpin für den Erdfeuchtesensor
#define ERDFEUCHTESENSOR_2 A1             // Anschlußpin für den Erdfeuchtesensor
#define HELLIGKEITSSENSOR A2              // 
#define LIMIT_ERDFEUCHTE A3               // 
#define LIMIT_TEMPERATUR A4
#define TEMPERATURSENSOR_LUEFTER 8        // Anschlußpin für Name  DHT22
#define RELAIS_LUEFTER 9
#define RELAIS_HEIZUNG 10
#define RELAIS_MOTOR_FENSTER 11
#define HELLIGKEITSSENSOR_UV_LED 12
#define RELAIS_UV_LED 7

//-------------------------------------------------------
//--- Variablen
//-------------------------------------------------------

volatile int taster_status = 0;

unsigned long t0_temperaturmessung;
unsigned long dt_temperaturmessung_ms = 2000;

unsigned long t0_display;
unsigned long dt_display_ms = 5000;

unsigned long t0_erdfeuchtemessung;
unsigned long dt_erdfeuchtemessung_ms = 5000;

unsigned long t0_pumpe;
unsigned long dt_pumpendauer_ms = 5000;

unsigned long t0_durchfeuchtung;
unsigned long dt_warten_durchfeuchtung_ms = 10000;

unsigned long t0_heizung;
unsigned long dt_heizung_ms = 5000;

unsigned long t0_luefter;
unsigned long dt_luefter_ms = 5000;

unsigned long t0_helligkeit;
unsigned long dt_helligkeit_ms = 5000;

int limit_bodenfeuchte_1 = 500;
int limit_bodenfeuchte_2 = 500;
int limit_helligkeit = 500;
float limit_temperatur = 25.00;                               // Lufttemperaturwert -> Heizen
float limit_luefter = 27.00;                                  // Lufttemperaturwert -> Lüften


#define zustand_warten_zu_trocken  0
#define zustand_pumpt 1
#define zustand_warten_durchfeuchtung 2
int zustand = zustand_warten_zu_trocken;

int taster, hum_erde_1, hum_erde_2, helligkeit;
float temp_luft, hum_luft;


//-------------------------------------------------------
// LCD per I2C und die Adresse 0x37
// Pin 4 -> SDA
// Pin 5 -> SCL
//-------------------------------------------------------

// Windows
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
//  taster_status = digitalRead(TASTER_PUMPE_UNTEN);                // Tasterstatus einlesen (High/Low)
//  digitalWrite(RELAIS_PUMPE_UNTEN, taster_status);                // Relais schalten
//}

//------------------------------------------------------
//--- String ausgeben
//------------------------------------------------------
String make_string( String s ) {
  while ( s.length() < 20 ) {
    s = s + " ";
  }
  return s;
}

//------------------------------------------------------
//--- Messwerte erfassen
//------------------------------------------------------
void update_messwerte() {
  hum_erde_1 = analogRead( ERDFEUCHTESENSOR_1 );                  // Lesen der Erdfeuchtigkeit, Werte zw. 0..1023
  hum_erde_2 = analogRead( ERDFEUCHTESENSOR_2 );                  // Lesen der Erdfeuchtigkeit, Werte zw. 0..1023
  hum_luft = dht.readHumidity();                                  // Lesen der Luftfeuchtigkeit und speichern in die Variable hum_luft
  temp_luft = dht.readTemperature();                              // Lesen der Temperatur in °C und speichern in die Variable temp_luft
}

//------------------------------------------------------
//--- Anzeige aktualisieren
//------------------------------------------------------
void update_lcd() {
  lcd.setCursor(0, 0);                                              // Displayausgabe erste Zeile

  lcd.print(
    make_string(
      String( "Temperatur:  " + String(temp_luft) + String( (char)223 )
            )
    )
  );

  lcd.setCursor(0, 1);                                              // Displayausgabe zweite Zeile
  lcd.print(
    make_string(
      String( "Luftfeuchte: " + String(hum_luft) + String( (char)37 )
            )
    )
  );

  lcd.setCursor(0, 2);                                              // Displayausgabe dritte Zeile
  lcd.print(
    make_string(
      String( "Erdfeuchte1: " + String(hum_erde_1) + String( (char)37 )
            )
    )
  );

  lcd.setCursor(0, 3);                                              // Displayausgabe vierte Zeile
  lcd.print(
    make_string(
      String( "Erd_Limit_1: " + String(limit_bodenfeuchte_1) + String( (char)37 )
            )
    )
  );

}

//------------------------------------------------------
//--- Setup des Programms, wird nur einmal ausgeführt
//------------------------------------------------------
void setup() {

  //  LCD Setup
  //lcd.init();                                                   // Windows
  //lcd.backlight();                                              // Windows
  //lcd.begin(20, 4);                                             // Linux
  lcd.begin();                                                    // Linux
  lcd.setCursor(0, 0);
  lcd.print("+++ STARTING...  +++");

  // Temperatursensor Setup
  dht.begin();
  temp_luft = dht.readTemperature();                              // Lesen der Temperatur in °C und speichern in die Variable temp_luft
  hum_luft = dht.readHumidity();                                  // Lesen der Luftfeuchtigkeit und speichern in die Variable hum_luft
  hum_erde_1 = analogRead( ERDFEUCHTESENSOR_1 );                  // Lesen der Erdfeuchtigkeit
  hum_erde_2 = analogRead( ERDFEUCHTESENSOR_2 );                  // Lesen der Erdfeuchtigkeit

  limit_bodenfeuchte_1 = analogRead(LIMIT_ERDFEUCHTE);

  // Pumpen 
  pinMode(RELAIS_PUMPE_UNTEN, OUTPUT);                            // Relais für die Wasserpume
  digitalWrite(RELAIS_PUMPE_UNTEN, HIGH );                        // HIGH = Relais AUS
  pinMode(RELAIS_PUMPE_OBEN, OUTPUT);                             // Relais für die Wasserpume
  digitalWrite(RELAIS_PUMPE_OBEN, HIGH );                         // HIGH = Relais AUS

  // Lüfter 
  pinMode(RELAIS_LUEFTER, OUTPUT);                                // Relais für den Lüfter
  digitalWrite(RELAIS_LUEFTER, HIGH );                            // HIGH = Relais AUS
  
  // Taster
  pinMode(TASTER_PUMPE_UNTEN, INPUT);

  // Relais Heizung
  pinMode(RELAIS_HEIZUNG, OUTPUT);
  digitalWrite(RELAIS_HEIZUNG, LOW );                             // --- Solid State Relais: LOW = AUS

  // Helligkeitssensor
  helligkeit = analogRead(HELLIGKEITSSENSOR);

  // ISR einrichten
  //  attachInterrupt(0, taster_unterbricht, LOW);
  
  t0_heizung = millis();
  t0_luefter = millis();
  t0_display = millis();
  t0_helligkeit = millis();
  t0_temperaturmessung = millis();
  t0_erdfeuchtemessung = millis();

  zustand = zustand_warten_zu_trocken;

  //taster = digitalRead(TASTER_PUMPE_UNTEN);                     // Tasterzustand an Pin 3 lesen

  lcd.setCursor(0, 1); // Spalte, Zeile
  lcd.print("+++ ...COMPLETE! +++");
}

//-------------------------------------------------------
//--- Schleife des Programms
//-------------------------------------------------------

void loop() {

  limit_bodenfeuchte_1 = analogRead(LIMIT_ERDFEUCHTE);

  /*  if ( taster == LOW ) and ( digitalRead(TASTER_PUMPE_UNTEN) == HIGH ) {
    taster == HIGH;
    }
  */

  /********************************( Display Ausgabe )*****************************************************/
  if ( ( millis() - t0_display ) > dt_display_ms ) {
    t0_display = millis();
    update_lcd();
  }

  /******************************** Messen Temperatur und Feuchtigkeit ***********************************/
  if ( ( millis() - t0_temperaturmessung ) > dt_temperaturmessung_ms ) {
    t0_temperaturmessung = millis();                                // Timer auf neuen Wert setzen
    update_messwerte();
  }


  /******************************** Heizung <--> Temperatursensor DHT22 ***********************************/
  if ( ( millis() - t0_heizung ) > dt_heizung_ms ) {
    t0_heizung = millis();
    if (temp_luft < limit_temperatur ) {
      digitalWrite(RELAIS_HEIZUNG, HIGH );                          // --- Solid State Relais: HIGH = EIN, heizen;
    }
    else {
      digitalWrite(RELAIS_HEIZUNG, LOW );                           // --- Solid State Relais: LOW = AUS, nicht heízen
    }
  }

/******************************** Lüfter + Motor Scheibe vorne <--> Temperatursensor DHT22 ****************/
  if ( ( millis() - t0_luefter ) > dt_luefter_ms ) {
    t0_luefter = millis();
    if (temp_luft > limit_luefter ) {
      digitalWrite(RELAIS_LUEFTER, LOW );                          //  LOW = Relais EIN
      digitalWrite(RELAIS_MOTOR_FENSTER, LOW );                    //  LOW = Relais EIN
    }
    else {
      digitalWrite(RELAIS_LUEFTER, HIGH );                        //  High = Relais AUS
      digitalWrite(RELAIS_MOTOR_FENSTER, HIGH );                  //  High = Relais AUS
    }
  }

  /******************************** LED-Licht <--> Helligkeitssensor *************************************/
  if ( ( millis() - t0_helligkeit ) > dt_helligkeit_ms ) {
    t0_helligkeit = millis();
    if (helligkeit > limit_helligkeit ) {           //  wenn dunkel, dann:
      digitalWrite(RELAIS_UV_LED, LOW );            //  LOW = Relais EIN
    }
    else {                                          //  sonst:
      digitalWrite(RELAIS_UV_LED, HIGH );           //  High = Relais AUS
    }
  }


  /******************************** REED-Kontakt -> Scheibe offen  ***************************************/

 /* 
  if ( reedkontakt == zu ) {
    dann führe den Übergangsautomaten aus
    
  }
  
  */  
  
  
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

  switch ( zustand ) {

    case zustand_warten_zu_trocken :
      //--- im Zustand bleiben, bis if() erfüllt -> neuer ZUstand
      if ( hum_erde_1 > limit_bodenfeuchte_1 ) {
        digitalWrite(RELAIS_PUMPE_UNTEN, LOW );                        // LOW = Relais EIN
        t0_pumpe = millis();
        zustand = zustand_pumpt;
      }
      break;

    case zustand_pumpt:
      if ( millis() - t0_pumpe > dt_pumpendauer_ms ) {
        digitalWrite(RELAIS_PUMPE_UNTEN, HIGH );                        // HIGH = Relais AUS
        t0_durchfeuchtung = millis();
        zustand = zustand_warten_durchfeuchtung;
      }
      break;

    case zustand_warten_durchfeuchtung :
      if ( millis() - t0_durchfeuchtung > dt_warten_durchfeuchtung_ms ) {
        zustand = zustand_warten_zu_trocken;
      }
      break;
  }

}
