#include <Arduino.h>

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

// für Zustandsautomat
#define zustand_warten_zu_trocken  0
#define zustand_pumpt 1
#define zustand_warten_durchfeuchtung 2
