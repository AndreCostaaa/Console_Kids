#ifndef _FctAntirebond
#define _FctAntirebond

  // definition des valeurs passees et reçu par la fontion "Antirebond"
  #define INACTIF         0 // 0: L'entree est non actionnee
  #define FLANC_INACTIF   1 // 1: L'entree passe en position de repos
  #define ACTIF           2 // 2: L'entree est active
  #define FLANC_ACTIF     3 // 3: L'entree passe a l'etat actif
  #define JUSTE_ACTIF     6 // 6: L'entree est active avant antirebond (ne pas utiliser)
  #define JUSTE_INACTIF   9 // 9: L'entree est inactive avant antirebond (ne pas utiliser)
  
  #define ACTIF_A_1       (bool)1  // Si l'entrée est active à 1
  #define ACTIF_A_0       (bool)0  // Si l'entrée est active à 0                            
							                            
  // Fonction gerant les flancs et les etats d'interrupteurs avec antirebond  
  void Antirebond (int iEntree,byte *bRead, bool boActif);   // Lecture des entrees. Le paramètre "int" est l'entree a traiter
#endif
