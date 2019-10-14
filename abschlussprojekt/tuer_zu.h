#include <Arduino.h>
#include "makros.h"

// Auswertung des REED-Kontaktes
bool tuer_zu() {                                                                                //  Funktion: Ist die Tür zu?
  return ( digitalRead( REEDKONTAKT ) == LOW );                                                 //  REED-Kontakt gleich Null?
}
