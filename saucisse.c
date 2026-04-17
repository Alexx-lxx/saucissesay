#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "saucisse.h"

void update() { printf("\033[H\033[J"); }

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

