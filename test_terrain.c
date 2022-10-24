#include "terrain.h"
#include <stdio.h>

int main(int argc, char **argv) {
  FILE *f;
  Terrain t;
  int x, y;

  if (argc < 2) {
    printf("Usage : %s <fichier>\n", argv[0]);
    return 1;
  }

  f = fopen(argv[1], "r");
  while (lire_terrain(f, &t, &x, &y) == ERREUR_FICHIER) {
	  printf("Veuillez entrer un nom de fichier valide.\n");
	  scanf("%s",argv[1]);
	  f = fopen(argv[1],"r");
}
  fclose(f);
  afficher_terrain(&t);
  printf("Position initiale du robot : (%d, %d)\n", x, y);
}
