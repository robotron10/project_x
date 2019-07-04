#include <Arduino.h>

#define ERDFEUCHTESENSOR_1 A0             // Anschlußpin für den Erdfeuchtesensor
#define ERDFEUCHTESENSOR_2 A1             // Anschlußpin für den Erdfeuchtesensor
#define HELLIGKEITSSENSOR A2              // 
#define LIMIT_ERDFEUCHTE_1 A3             // 
#define LIMIT_ERDFEUCHTE_2 A4
#define LIMIT_TEMPERATUR A5
#define LIMIT_LUEFTER A6



void update_limits() {

int limit_bodenfeuchte_1 = 500;
int limit_bodenfeuchte_2 = 500;
int limit_helligkeit = 500;
int limit_temperatur = 25;                               // Lufttemperaturwert -> Heizen
int limit_luefter = 27;                                  // Lufttemperaturwert -> Lüften

int hum_erde_1, hum_erde_2, temp_luft, hum_luft, voriger_taster, helligkeit;
int hum_erde_1_displ, hum_erde_2_displ, temp_luft_displ, hum_luft_displ;
int limit_bodenfeuchte_1_displ, limit_bodenfeuchte_2_displ, limit_temperatur_displ, limit_luefter_displ;


  limit_bodenfeuchte_1 = analogRead(LIMIT_ERDFEUCHTE_1);
  limit_bodenfeuchte_2 = analogRead(LIMIT_ERDFEUCHTE_2);
  limit_temperatur = analogRead(LIMIT_TEMPERATUR);
  limit_luefter = analogRead(LIMIT_LUEFTER);

  // Werte für das Display mappen
  limit_bodenfeuchte_1_displ = map(limit_bodenfeuchte_1, 0, 1023, 0, 100);
  limit_bodenfeuchte_2_displ = map(limit_bodenfeuchte_2, 0, 1023, 0, 100);
  limit_temperatur_displ = map(limit_temperatur, 0, 1023, 100, 0);
  limit_luefter_displ = map(limit_temperatur, 0, 1023, 0, 100);
}
