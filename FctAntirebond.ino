#include "FctAntirebond.h"

// Fonction Antirebond ******************************************************************
void Antirebond (int iEntree,byte *bRead, bool boActif){
  boolean boInput = 0;
  
  if(boActif == ACTIF_A_1){        // Si l'entrée utilise une pulldown (actif à 1)
    boInput = digitalRead(iEntree);
      }
  else{
    boInput = !digitalRead(iEntree);
      }
  
  if(boInput){ // Si l'entrée est active
    switch (*bRead){
      case ACTIF          : 
      case FLANC_ACTIF    : *bRead = ACTIF;
                             break;
      
      case INACTIF        : *bRead = JUSTE_ACTIF;
                             break;
      case JUSTE_ACTIF    : *bRead = FLANC_ACTIF;
                             break;
    }
  }
  else{       // Donc l'entrée est inactive
    switch (*bRead){
      case INACTIF        :
      case FLANC_INACTIF  : *bRead = INACTIF;
                             break;
      
      case ACTIF          : *bRead = JUSTE_INACTIF;
                             break;
      case JUSTE_INACTIF  : *bRead = FLANC_INACTIF;
                             break;
    }
  }
}
