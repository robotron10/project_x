#include "DHT.h"                
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTTYPE DHT22                     // Typ des Temperatur- und Feuchtesensors
#define DHTPIN 6                          // Anschlußpin für Temperatur- und Feuchtesensor
#define TASTER 5                          // Anschlußpin für den Taster zum Einschalten der Wasserpumpe
#define RELAIS 4                          // Anschlußpin für das Relais, welches die Wasserpumpe steuert

const int Taster = 3;                     // +++ lieber per define +++
volatile int TasterStatus = 0;            // 

// LCD per I2C und die Adresse 0x37
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);                               

// Temperatur- und Feuchtesensor einrichten (Pin, Typ)
DHT dht(DHTPIN, DHTTYPE);

// Setup des Programms, wird nur einmal ausgeführt
void setup() {
  
  //  LCD Setup
  lcd.begin(20, 4);
  lcd.setCursor(0,0);
  lcd.print("+++ SYSTEMSTART +++");
  
  // Temperatursensor Setup
  dht.begin();
  
  // Pumpen Setup
  pinMode(RELAIS, OUTPUT);                  // Relais für die Wasserpume
  pinMode(Taster, INPUT);
  
  // ISR einrichten    
  attachInterrupt(0, TasterUnterbricht, LOW);
}

// Schleife des Programms
void loop() {

  int taster = digitalRead(TASTER);         // Tasterzustand an Pin 5 lesen

  
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

/********************************( Wasserpumpe )*********************************************/
     // Bedingung für die Wasserpumpe(Relais)
     if (taster == HIGH){                   // bei Tastendruck - Relais ein
          digitalWrite(RELAIS, HIGH);
          lcd.setCursor(13,3);
          lcd.print("EIN");                 // Ausgabe auf den Display
     } else {
          digitalWrite(RELAIS, LOW);        // kein Tastendruck - Relais aus
          lcd.setCursor(13,3);
          lcd.print("AUS");                 // Ausgabe auf den Display
     }
    
  }

  // ISR um die Pumpe manuell ein-/ausschalten zu können
void TasterUnterbricht() {
   TasterStatus = digitalRead(Taster);      // Tasterstatus einlesen (High/Low)
   digitalWrite(RELAIS, TasterStatus);      // Relais schalten
}
