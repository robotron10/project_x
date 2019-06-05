// Datum: 05.06.2019
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
#define TEMPERATURSENSOR_LUEFTER 8        // Anschlußpin für Temperatur- und Feuchtesensor DHT22
#define RELAIS_LUEFTER 9
#define RELAIS_HEIZUNG 10
#define RELAIS_MOTOR_FENSTER 11
#define HELLIGKEITSSENSOR_UV_LED 12
#define RELAIS_UV_LED 13

//-------------------------------------------------------
//--- Variablen
//-------------------------------------------------------

volatile int taster_status = 0;

unsigned long t0_meas;
unsigned long dt_meas_ms = 2000;

unsigned long t0_display;
unsigned long dt_display_ms = 5000;

unsigned long t0_erdfeuchtemessung;
unsigned long dt_erdfeuchtemessung_ms = 5000;

int limit_bodenfeuchte = 500;
int taster;
float temp_air, hum_air, hum_soil1, hum_soil2;

//-------------------------------------------------------
// LCD per I2C und die Adresse 0x37
// Pin 4 -> SDA
// Pin 5 -> SCL
//-------------------------------------------------------

// Windows
//LiquidCrystal_I2C lcd(0x3F, 20, 4 );

// Linux
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Temperatur- und Feuchtesensor einrichten (Pin, Typ)
DHT dht(TEMPERATURSENSOR_LUEFTER, DHTTYPE);

//-------------------------------------------------------
//--- Funktionen
//-------------------------------------------------------


// ISR um die Pumpe manuell ein-/ausschalten zu können
//void taster_unterbricht() {
//  taster_status = digitalRead(TASTER_PUMPE_UNTEN);      // Tasterstatus einlesen (High/Low)
//  digitalWrite(RELAIS_PUMPE_UNTEN, taster_status);      // Relais schalten
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
//--- Anzeige aktualisieren
//------------------------------------------------------
void update_lcd() {
  lcd.setCursor(0, 0);

  lcd.print(
    make_string(
      String( "Temperatur:  " + String(temp_air) + String( (char)223 )
            )
    )
  );
  lcd.setCursor(0, 1);
  lcd.print("Luftfeuchte: ");
  lcd.setCursor(13, 1);
  lcd.print(hum_air);                             // Luftfeuchtewert ausgeben
  lcd.setCursor(0, 2);
  lcd.print("Erdfeuchte:  ");
  lcd.print(hum_soil1);                             // Erdfeuchtewert ausgeben
  lcd.setCursor(0, 3);
  lcd.print("Pumpe: ");
}

//------------------------------------------------------
//--- Setup des Programms, wird nur einmal ausgeführt
//------------------------------------------------------
void setup() {

  //  LCD Setup
  //lcd.init();               // Windows
  //lcd.backlight();          // Windows
  lcd.begin(20, 4);           // Linux
  lcd.setCursor(0, 0);
  lcd.print("+++ STARTING...  +++");

  // Temperatursensor Setup
  dht.begin();
  hum_air = dht.readHumidity();                // Lesen der Luftfeuchtigkeit und speichern in die Variable h
  hum_soil1 = analogRead( ERDFEUCHTESENSOR_1 ); // Lesen der Erdfeuchtigkeit
  hum_soil2 = analogRead( ERDFEUCHTESENSOR_2 ); // Lesen der Erdfeuchtigkeit
  temp_air = dht.readTemperature();            // Lesen der Temperatur in °C und speichern in die Variable t

  // Pumpen Setup
  pinMode(RELAIS_PUMPE_UNTEN, OUTPUT);                  // Relais für die Wasserpume
  pinMode(TASTER_PUMPE_UNTEN, INPUT);

  // ISR einrichten
  //  attachInterrupt(0, taster_unterbricht, LOW);

  t0_meas = millis();
  t0_display = millis();
  t0_erdfeuchtemessung = millis();

  taster = digitalRead(TASTER_PUMPE_UNTEN);         // Tasterzustand an Pin 3 lesen

  lcd.setCursor(0, 1); // Spalte, Zeile
  lcd.print("+++ ...COMPLETE! +++");
}

//-------------------------------------------------------
//--- Schleife des Programms
//-------------------------------------------------------

void loop() {

  /*  if ( taster == LOW ) and ( digitalRead(TASTER_PUMPE_UNTEN) == HIGH ) {
    taster == HIGH;
    }
  */

  int messwert_bodenfeuchte_1 = analogRead( ERDFEUCHTESENSOR_1 ); // Werte zwischen 0..1023
  int messwert_bodenfeuchte_2 = analogRead( ERDFEUCHTESENSOR_2 ); // Werte zwischen 0..1023

  /********************************( Tempsensor )*********************************************/
  if ( ( millis() - t0_meas ) > dt_meas_ms ) {
    t0_meas = millis();
    hum_soil1 = analogRead( ERDFEUCHTESENSOR_1 );   // Lesen der Erdfeuchtigkeit 0..1023
    hum_soil2 = analogRead( ERDFEUCHTESENSOR_2 );   // Lesen der Erdfeuchtigkeit
    hum_air = dht.readHumidity();                   // Lesen der Luftfeuchtigkeit und speichern in die Variable h
    temp_air = dht.readTemperature();               // Lesen der Temperatur in °C und speichern in die Variable t
  }

  //if (isnan(h) || isnan(t)) {             // is not a number
  //  lcd.setCursor(0, 1);
  //  lcd.print("Fehler Luftsensor!");
  //  return;
  //}

  /********************************( Display Ausgabe )*********************************************/
  if ( ( millis() - t0_display ) > dt_display_ms ) {
    t0_display = millis();
    update_lcd();
  }

  /********************************( Wasserpumpe -- Erdfeuchtesensoren)*********************************************/
  if ( ( millis() - t0_erdfeuchtemessung ) > dt_erdfeuchtemessung_ms ) {
    t0_erdfeuchtemessung = millis();

    if (messwert_bodenfeuchte_1 > limit_bodenfeuchte ) {
      digitalWrite(RELAIS_PUMPE_UNTEN, HIGH);
      lcd.setCursor(13, 3);
      lcd.print("EIN");                             // Ausgabe auf den Display
    }
  }

}
