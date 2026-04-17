#ifndef __TAMAGOSHI_SAUCISSE__
#define __TAMAGOSHI_SAUCISSE__

typedef enum {
    LIFEROCKS,
    LIFESUCKS,
    BYEBYELIFE
} etat;


int generer_entier(int borne) ; 


int generer_signe(int entier) ; 


int stock_update(int stock, int lunchfood) ; 


int fitness_update(int fitness, int lunchfood) ; 


void affiche_tamagoshi(etat e) ;


etat calculer_etat(int fitness) ;


int nourrir_tamagoshi(int stock, char *nom) ;


int partie_tamagoshi(char *nom) ;


void jeu_tamagoshi(char *nom) ;


#endif