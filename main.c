#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> 
#include "saucisse.h"
#include "tamagoshi_saucisse.h"


static const char *color_from_name(const char *name) {
    if (strcmp(name, "rouge") == 0 || strcmp(name, "red") == 0) return ROUGE;
    if (strcmp(name, "vert") == 0 || strcmp(name, "green") == 0) return VERT;
    if (strcmp(name, "jaune") == 0 || strcmp(name, "yellow") == 0) return JAUNE;
    if (strcmp(name, "bleu") == 0 || strcmp(name, "blue") == 0) return BLEU;
    if (strcmp(name, "violet") == 0 || strcmp(name, "purple") == 0) return VIOLET;
    if (strcmp(name, "cyan") == 0) return CYAN;
    if (strcmp(name, "blanc") == 0 || strcmp(name, "white") == 0) return BLANC;
    return NULL;
}



int main(int argc, char *argv[]) {
    const char *eyes = "oo";
    const char *poignet = "   ";
    const char *color = NULL;
    const char *message = " ";
    int bouge = 0;
    int distance = 0;
    int mode = 0;
    const char *read_path = NULL;
    char *nom = "la saucisse";

    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            message = argv[i + 1];
            i += 2;
        }
        else if (strcmp(argv[i], "-read") == 0) {
            mode = 1;
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                read_path = argv[i + 1];
                i += 2;
            } else i++;
        }
        else if (strcmp(argv[i], "-game") == 0) {
            mode = 2;
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                nom = argv[i + 1];
                i += 2;
            } else i++;
        }
        else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            color = color_from_name(argv[i + 1]);
            i += 2;
        }
        else if (strcmp(argv[i], "-bouge") == 0) {
            bouge = 1;
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                distance = atoi(argv[i + 1]);
                i += 2;
            } else i++;
        }
        else if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            eyes = argv[i + 1];
            i += 2;
        }
        else if (strcmp(argv[i], "-b") == 0) {
            eyes = "{O"; poignet = " o≈≈≈≈≈≈≈≈≈≈";
            i++;
        }
        else if (strcmp(argv[i], "-d") == 0) {
            eyes = "xx";
            i++;
        }
        else if (strcmp(argv[i], "-g") == 0) {
            eyes = "$$"; poignet = "€£";
            i++;
        }
        else if (strcmp(argv[i], "-p") == 0) {
            eyes = "@@"; poignet = "¿¿";
            i++;
        }
        else if (strcmp(argv[i], "-s") == 0) {
            eyes = "~~"; poignet = "#*#";
            i++;
        }
        else if (strcmp(argv[i], "-t") == 0) {
            eyes = "--";
            i++;
        }
        else if (strcmp(argv[i], "-w") == 0) {
            eyes = "OO"; poignet = "!∫!";
            i++;
        }
        else if (strcmp(argv[i], "-y") == 0) {
            eyes = "^^"; poignet = " π ";
            i++;
        }
        else {
            i++;
        }
    }

    // Mode simple 
    if (mode == 0) {
        if (color) printf("%s", color);
        affiche_bulle(0, message);
        affiche_saucisse(0, eyes, poignet, "  ");
        if (color) printf("%s", RESET);
    } 

    // Mode reading
    else if (mode == 1) {

        FILE *f = NULL;

        if (read_path) {
            f = fopen(read_path, "r");
            if (!f) { perror("fopen"); return 1; }
        } else {
            f = stdin;
        }

        reading_animee(f, bouge ? distance : 0, color, eyes, poignet);

        if (f != stdin) fclose(f);
    }

    // Mode tamagoshi
    else {
        if (color) printf("%s", color);
        jeu_tamagoshi(nom);
        if (color) printf("%s", RESET);
    }


    return 0;
}