#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> 

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

int main(int argc, char *argv[]) {
    const char *eyes = "oo";
    const char *poignet = "   ";
    const char *color = NULL;
    const char *message = " ";
    int bouge = 0;
    int distance = 0;
    int read_mode = 0;
    const char *read_path = NULL;

    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            message = argv[i + 1];
            i += 2;
        }
        else if (strcmp(argv[i], "-read") == 0) {
            read_mode = 1;
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                read_path = argv[i + 1];
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
    if (!read_mode) {
        if (color) printf("%s", color);
        affiche_bulle(0, message);
        affiche_saucisse(0, eyes, poignet, "  ");
        if (color) printf("%s", RESET);
        return 0;
    }

    // Mode reading
    FILE *f = NULL;

    if (read_path) {
        f = fopen(read_path, "r");
        if (!f) { perror("fopen"); return 1; }
    } else {
        f = stdin;
    }

    reading_animee(f, bouge ? distance : 0, color, eyes, poignet);

    if (f != stdin) fclose(f);

    return 0;
}