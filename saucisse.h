#ifndef __SAUCISSE__
#define __SAUCISSE__

// Couleurs 
#define RESET   "\033[0m"
#define ROUGE   "\033[31m"
#define VERT    "\033[32m"
#define JAUNE   "\033[33m"
#define BLEU    "\033[34m"
#define VIOLET  "\033[35m"
#define CYAN    "\033[36m"
#define BLANC   "\033[37m"

void update();


void affiche_bulle(int offset, const char *text) ; 


void affiche_saucisse(int offset,
                      const char *eyes,
                      const char *poignet,
                      const char *tongue);


void reading_animee(FILE *f,
                       int max_offset,
                       const char *color,
                       const char *eyes,
                       const char *poignet);


#endif