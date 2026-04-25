#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "saucisse.h"
#include "tamagoshi_saucisse.h"


int generer_entier(int borne) {
    
    /* Genere un nombre aleatoire dans l'interval entier [0;borne-1]*/
    int nombre ;
    nombre = rand () % borne;

    return nombre;
}

int generer_signe(int entier) {
    int signe = generer_entier(2);
    if (signe == 0) 
        return entier;
    else 
        return (-entier);
}

int stock_update(int stock, int lunchfood) {
    int crop = generer_signe(generer_entier(4));
    int newstock = (stock - lunchfood) + crop;

    if (newstock < 0) 
        newstock = 0;
    else if (newstock > 10)
        newstock = 10;

    return newstock;
}

int fitness_update(int fitness, int lunchfood) {
    int digestion = -generer_entier(4);
    int newfitness = (fitness + lunchfood) + digestion;

    if (newfitness < 0)
        newfitness = 0;
    else if (newfitness > 10)
        newfitness = 10;

    return newfitness;
}

void affiche_tamagoshi(etat e) {
    
    // Etat LIFEROCKS : tout va bien
    const char* yeux_bien[] = {"^^", "oo", "OO", "°°"};
    const char* poignet_bien[] = {" π ", " ω ", " U ", " ∩ "};
    const char* phrase_bien[] = {"Tout va bien !", "Quelle belle journée !", "Il fait beau aujourd'hui !", "Aucun problème dans ma vie !"};

    // Etat LIFESUCKS : pas bien
    const char* yeux_malade[] = {"~~", "--", "TT", "QQ"};
    const char* poignet_malade[] = {"#*#", "...", "___", "≈ ≈"};
    const char* phrase_malade[] = {"Je ne me sens pas très bien...", "J'ai un peu mal au ventre...", "Je crois que je vais tomber malade...", "Quelque chose ne va pas..."};

    switch (e) {
        case LIFEROCKS :
            affiche_bulle(0, phrase_bien[generer_entier(4)]);
            affiche_saucisse(0, yeux_bien[generer_entier(4)], poignet_bien[generer_entier(4)], " ");
            break;

        case LIFESUCKS :
            affiche_bulle(0, phrase_malade[generer_entier(4)]);
            affiche_saucisse(0, yeux_malade[generer_entier(4)], poignet_malade[generer_entier(4)], " ");
            break;

        case BYEBYELIFE :
            affiche_bulle(0, "...");
            affiche_saucisse(0, "XX", "¿¿", "U");
            break ;

        default : 
            printf("Erreur") ;
            break ;
    }
}

etat calculer_etat(int fitness) {
    if ((fitness <= 6)&&(fitness >= 4))         
        return LIFEROCKS;
    else if ((((fitness <= 3)&&(fitness >= 1))||((fitness <= 9)&&(fitness >= 7))))
        return LIFESUCKS ;
    else
        return BYEBYELIFE ;
}


int nourrir_tamagoshi(int stock, char *nom) {
    int lunchfood ; 
    
    printf("\n\nVous avez %d blé\n", stock);
    printf("Combien de blé voulez vous donner à %s aujourd'hui ?\n", nom);
    
    int verif = scanf("%d", &lunchfood);
    
    while (verif != 1 || lunchfood < 0 || lunchfood > stock) {


        if (verif != 1) {
            printf("Erreur, vous devez entrer un nombre\n");
            scanf("%*s"); // Supprime la dernière entrée lue pour éviter une boucle infinie
        } else if (lunchfood < 0) {
            printf("Vous ne pouvez pas donner une quantité négative de blé.\n");
        } else if (lunchfood > stock) {
            printf("Vous n'avez pas assez de blé\n");
        }

        if (verif != 1 || lunchfood < 0 || lunchfood > stock) {
            printf("Combien de blé voulez-vous donner à %s aujourd'hui ?\n", nom);
            verif = scanf("%d", &lunchfood);
    
        }
    }

    return lunchfood ;
}

void affiche_var_stock(int var_stock) {
    if (var_stock == -4) // Traite le premier jours.
        printf("\n");
    else {
        if (var_stock > 0) 
            printf("Génial ! une livraison de %d blés est arrivée cette nuit !\n", var_stock);
        else if (var_stock < 0)
            printf("Aie... %d de blés ont pourris cette nuit...\n", -var_stock);
        else 
            printf("Il n'est rien arrivé à votre stock de blé cette nuit !\n");
    }
}


int partie_tamagoshi(char *nom) {
    etat etat_courant = LIFEROCKS;
    int fitness = 5;
    int stock = 5;
    int new_stock;
    int var_stock = -4; // Valeur impossible, traite le premier jours
    int lunchfood;
    int duree_de_vie = 1;

    while (etat_courant != BYEBYELIFE) {

        // Affichage
        update();
        printf("|================ Jour %d =================|\n\n", duree_de_vie);
        affiche_tamagoshi(etat_courant);
        affiche_var_stock(var_stock);

        // Nourrir le tamagoshi
        lunchfood = nourrir_tamagoshi(stock, nom);

        // Mise à jours du stock et de la variation du stock pour l'affichage
        new_stock = stock_update(stock, lunchfood);
        var_stock = new_stock - stock + lunchfood ;
        stock = new_stock ;

        // Mise à jours de la santé
        fitness = fitness_update(fitness, lunchfood);

        // Mise à jours de l'état
        etat_courant = calculer_etat(fitness) ; 
        
        // Incrémentation du jours
        duree_de_vie++;
        
    }
    // Affiche l'état final BYEBYELIFE
    update();
    printf("|================ Jour %d =================|\n\n", duree_de_vie);
    affiche_tamagoshi(etat_courant);

    return duree_de_vie ; 
}

void jeu_tamagoshi(char *nom) {

    // Initialisation de l'aléatoire
    srand (getpid());
    
    char rejouer = 'o';
    int record = 0;
    int score;

    while (rejouer == 'o') {
        
        score = partie_tamagoshi(nom);

        printf("%s a survécu %d jours\n\n", nom, score);
        if (score > record) {
            record = score;
            printf("Nouveau record !\n");
        }
        printf("Votre record est %d jours\n\n", record);
        printf("Voulez vous rejouer ? (o/n)\n");
        
        scanf(" %c", &rejouer);

    }
}