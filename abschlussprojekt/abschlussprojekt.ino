// Datum: 03.06.2019
// Autor: Stephanie Nawroth, Thorsten Graf, Fabian Roth
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
#define ERDFEUCHTESENSOR_1 6              // Anschlußpin für den Erdfeuchtesensor
#define ERDFEUCHTESENSOR_2 7              // Anschlußpin für den Erdfeuchtesensor
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
unsigned long t0_display;
unsigned long dt_display_ms = 5000;

//-------------------------------------------------------
// LCD per I2C und die Adresse 0x37
// Pin 4 -> SDA
// Pin 5 -> SCL
//-------------------------------------------------------

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);                               

// Temperatur- und Feuchtesensor einrichten (Pin, Typ)
DHT dht(TEMPERATURSENSOR_LUEFTER, DHTTYPE);

//-------------------------------------------------------
//--- Funktionen
//-------------------------------------------------------


// ISR um die Pumpe manuell ein-/ausschalten zu können
void taster_unterbricht() {
   taster_status = digitalRead(TASTER_PUMPE_UNTEN);      // Tasterstatus einlesen (High/Low)
   digitalWrite(RELAIS_PUMPE_UNTEN, taster_status);      // Relais schalten
}


//------------------------------------------------------
//--- Setup des Programms, wird nur einmal ausgeführt
//------------------------------------------------------

void setup() {
  
  //  LCD Setup
  lcd.begin(20, 4);
  lcd.setCursor(0,0);
  lcd.print("+++ SYSTEMSTART +++");
  
  // Temperatursensor Setup
  dht.begin();
  
  // Pumpen Setup
  pinMode(RELAIS_PUMPE_UNTEN, OUTPUT);                  // Relais für die Wasserpume
  pinMode(TASTER_PUMPE_UNTEN, INPUT);
  
  // ISR einrichten    
  attachInterrupt(0, taster_unterbricht, LOW);
}

//-------------------------------------------------------
//--- Schleife des Programms
//-------------------------------------------------------

void loop() {

  int taster = digitalRead(TASTER_PUMPE_UNTEN);         // Tasterzustand an Pin 3 lesen

  
  /********************************( Tempsensor )*********************************************/ 
  delay(2000);                                                
  float h = dht.readHumidity();             // Lesen der Luftfeuchtigkeit und speichern in die Variable h
  float t = dht.readTemperature();          // Lesen der Temperatur in °C und speichern in die Variable t
  
  //if (isnan(h) || isnan(t)) {             // is not a number
  //  lcd.setCursor(0, 1);
  //  lcd.print("Fehler beim auslesen des Sensors!");
  //  return;
  //}
  
/********************************( Display Ausgabe )*********************************************/
  if ( millis() - t0_display > dt_display_ms ) {
     t0_display = millis();
     lcd.setCursor(0,0);         
     lcd.print("Temperatur:  ");
     lcd.setCursor(13,0);
     lcd.print(t);                             // Temperaturwert ausgeben
     lcd.setCursor(18,0);
     lcd.print((char)223);                     // '°' Zeichen
     lcd.setCursor(0,1);         
     lcd.print("Luftfeuchte: ");
     lcd.setCursor(13,1);
     lcd.print(h);                             // Luftfeuchtewert ausgeben
     lcd.setCursor(0,2);
     lcd.print("Erdfeuchte: ");
     //lcd.print(String(millis(), DEC));
     lcd.setCursor(0,3);
     lcd.print("Pumpe: ");
  }

/********************************( Wasserpumpe )*********************************************/
     // Bedingung für die Wasserpumpe(Relais)
     if (taster == HIGH){                               // bei Tastendruck - Relais ein
          digitalWrite(RELAIS_PUMPE_UNTEN, HIGH);
          lcd.setCursor(13,3);
          lcd.print("EIN");                             // Ausgabe auf den Display
     } else {
          digitalWrite(RELAIS_PUMPE_UNTEN, LOW);        // kein Tastendruck - Relais aus
          lcd.setCursor(13,3);
          lcd.print("AUS");                             // Ausgabe auf den Display
     }
    
  }


