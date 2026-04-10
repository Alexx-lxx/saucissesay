#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> 

// Couleurs 
#define RESET   "\033[0m"
#define ROUGE   "\033[31m"
#define VERT    "\033[32m"
#define JAUNE   "\033[33m"
#define BLEU    "\033[34m"
#define VIOLET  "\033[35m"
#define CYAN    "\033[36m"
#define BLANC   "\033[37m"

void update(void) { printf("\033[H\033[J"); }

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

void affiche_bulle(int offset, const char *text)
{
    // 1) Calculer largeur max d'une ligne (supporte \n) 
    int maxw = 0;
    int cur = 0;
    for (const char *p = text; *p; p++) {
        if (*p == '\n') {
            if (cur > maxw) maxw = cur;
            cur = 0;
        } else {
            cur++;
        }
    }
    if (cur > maxw) maxw = cur;

    // 2) Ligne du haut 
    for (int k = 0; k < offset; k++) putchar(' ');
    putchar(' ');
    for (int i = 0; i < maxw + 2; i++) putchar('_');
    putchar('\n');

    // 3) Afficher chaque ligne avec indentation + padding 
    const char *line = text;
    while (1) {
        const char *end = strchr(line, '\n');
        int len = end ? (int)(end - line) : (int)strlen(line);

        for (int k = 0; k < offset; k++) putchar(' ');
        putchar('<');
        putchar(' ');
        fwrite(line, 1, len, stdout);

        // padding pour aligner les bords 
        for (int s = len; s < maxw; s++) putchar(' ');
        putchar(' ');
        putchar('>');
        putchar('\n');

        if (!end) break;
        line = end + 1;
    }

    // 4) Ligne du bas
    for (int k = 0; k < offset; k++) putchar(' ');
    putchar(' ');
    for (int i = 0; i < maxw + 2; i++) putchar('-');
    putchar('\n');

    // 5) Queue 
    for (int k = 0; k < offset; k++) putchar(' ');
    printf("  \\\n");
}

void affiche_saucisse(int offset,
                      const char *eyes,
                      const char *poignet,
                      const char *tongue)
{
    #define INDENT do { for (int k = 0; k < offset; k++) printf(" "); } while (0)

    INDENT; printf("                     .--.. __\n");
    INDENT; printf("                    /     \\  \\     |||\n");
    INDENT; printf("           \\\\\\     |  %s   |  |  \\ |||\n", eyes);
    INDENT; printf("          \\   //   | \\_/   |  |   \\%s/\n", poignet);
    INDENT; printf("           '--/---//   %s /   / |-'-'\n", tongue);
    INDENT; printf("                 // //   /   ---'\n");
    INDENT; printf("                //  \\\\  /   /\n");
    INDENT; printf("               //  //  /   /\n");
    INDENT; printf("              //  \\\\  /   /\n");
    INDENT; printf("             //  //  /   /\n");
    INDENT; printf("            /|   '  /   /\n");
    INDENT; printf("           //\\\\.__./   /\n");
    INDENT; printf("           //   ||\\\\  /\n");
    INDENT; printf("           \\\\_  || '-'\n");
    INDENT; printf("           /' / \\_'\n");
    INDENT; printf("          /  /   | |\n");
    INDENT; printf("          '-'    '-'\n");

    #undef INDENT
}

// Reading animée
void reading_animee(FILE *f,
                       int max_offset,
                       const char *color,
                       const char *eyes,
                       const char *poignet)
{
    char eaten[2048];
    int len = 0;
    eaten[0] = '\0';

    int offset = 0;
    int ch;

    while (1) {

        char tongue[3] = "  ";

        // lire prochain caractère
        ch = fgetc(f);

        if (ch != EOF) {
            tongue[0] = (char)ch;
            tongue[1] = ' ';
            tongue[2] = '\0';

            if (ch == '\n') { tongue[0] = '\\'; tongue[1] = 'n'; }
        }

        update();
        if (color) printf("%s", color);

        affiche_bulle(offset, eaten);          // ancien texte
        affiche_saucisse(offset, eyes, poignet, tongue); // nouvelle lettre

        if (color) printf("%s", RESET);
        fflush(stdout);

        sleep(1);

        // avaler après affichage 
        if (ch != EOF) {
            if (len < (int)sizeof(eaten) - 2) {
                eaten[len++] = (char)ch;
                eaten[len] = '\0';
            }
        }

        // mouvement 
        if (offset < max_offset) {
            offset++;
        }

        // condition d'arrêt 
        if (ch == EOF && offset >= max_offset)
            break;
    }

    // frame finale (langue vide) 
    update();
    if (color) printf("%s", color);
    affiche_bulle(offset, eaten);
    affiche_saucisse(offset, eyes, poignet, "  ");
    if (color) printf("%s", RESET);
    fflush(stdout);
}


// Fonctions pour le tamagoshi

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
    if (newstock > 10)
        newstock = 10;

    return newstock;
}

int fitness_update(int fitness, int lunchfood) {
    int digestion = -generer_entier(4);
    int newfitness = (fitness + lunchfood) + digestion;

    if (newfitness < 0)
        newfitness = 0;
    if (newfitness > 10)
        newfitness = 10;

    return newfitness;
}

void affiche_tamagoshi(int etat) {
    if (etat == 0) {
        affiche_bulle(0, "Tout va bien !");
        affiche_saucisse(0, "^^", " π ", " ");
    } else if (etat == 1) {
        affiche_bulle(0, "Je ne me sens pas bien...");
        affiche_saucisse(0, "~~", "#*#", " ");
    } else {
        affiche_bulle(0, "...");
        affiche_saucisse(0, "XX", "¿¿", "U");
    }
}

void jeu_tamagoshi(char *nom) {
    
    // Initialisation de l'aléatoire
    srand (getpid());
    
    char rejouer = 'o';
    int record = 0;

    while (rejouer == 'o') {
        int etat = 0;
        int fitness = 5;
        int stock = 5;
        int lunchfood;
        int duree_de_vie = 0;

        while (etat != 2) {
            if ((fitness <= 6)&&(fitness >= 4)) 
                // Etat Liferocks
                etat = 0;
            else if ((((fitness <= 3)&&(fitness >= 1))||((fitness <= 9)&&(fitness >= 7))))
                // Etat Lifesucks
                etat = 1;
            else
                // Etat Byebyelife
                etat = 2;

            duree_de_vie++;
            update();
            printf("|================ Jour %d =================|\n\n", duree_de_vie);
            affiche_tamagoshi(etat);
            
            if (etat != 2) {    

                printf("\n\nVous avez %d blé\n", stock);
                printf("Combien de blé voulez vous donner à %s aujourd'hui ?\n", nom);
                scanf("%d", &lunchfood);
                
                while (lunchfood > stock || lunchfood < 0) {
                    if (lunchfood > stock)
                        printf("Vous n'avez pas assez de blé\n");
                    else
                        printf("Vous ne pouvez pas donner une quantité négative de blé.\n");

                    printf("Combien de blé voulez-vous donner à la %s aujourd'hui ?\n", nom);
                    scanf("%d", &lunchfood);
                }

                stock = stock_update(stock, lunchfood);
                fitness = fitness_update(fitness, lunchfood);
            }
        }

        printf("%s a survécu %d jours\n\n", nom, duree_de_vie);
        if (duree_de_vie > record) {
            record = duree_de_vie;
            printf("Nouveau record !\n");
        }
        printf("Votre record est %d jours\n\n", record);
        printf("Voulez vous rejouer ? (o/n)\n");
        scanf(" %c", &rejouer);
        
    }
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