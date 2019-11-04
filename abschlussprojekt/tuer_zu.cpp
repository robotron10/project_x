//////////////////////////////////////
/// Auswertung des REED-Kontaktes  ///
//////////////////////////////////////
#include <Arduino.h>
#include "makros.h"
#include "tuer_zu.h"

bool tuer_zu() {                                 //  Funktion: Ist die Tür zu?
  
  return ( digitalRead( REEDKONTAKT ) == LOW );  //  REED-Kontakt gleich Null?
  
}
