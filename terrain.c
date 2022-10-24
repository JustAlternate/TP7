
#include "terrain.h"
#include "robot.h"
#include <stdio.h>
#include <string.h>

erreur_terrain lire_terrain(FILE *f, Terrain *t, int *x, int *y) {
    int l, h;   // Dimensions du terrain
    int rx, ry; // Coordonnées initiales du robot
    char ligne[DIM_MAX];
    int i, j;
	int test_lecture_dim = 0;
	int compteur_c = 0; // Compte le nombre de fois où la caractère 'C' est rencontré

    if (f == NULL) {
        return ERREUR_FICHIER;
    }

    // Lecture de la largeur et de la hauteur
    test_lecture_dim = fscanf(f,"%d\n", &l);
	if (test_lecture_dim == 0) {
		return ERREUR_LECTURE_LARGEUR;
	}

	test_lecture_dim = fscanf(f,"%d\n", &h);
	if (test_lecture_dim == 0) {
		return ERREUR_LECTURE_HAUTEUR;
	}

	if (h < 0 || h > DIM_MAX) {
		return ERREUR_HAUTEUR_INCORRECTE;
	}
	if (l < 0 || l > DIM_MAX) {
		return ERREUR_LARGEUR_INCORRECTE;
	}

    t->hauteur = h;
    t->largeur = l;

    // Lecture du terrain
    for (i = 0 ; i < h ; i++) {
        // On met la ligne i du fichier dans la chaine ligne
        if (fgets(ligne, DIM_MAX, f) == NULL) {
			return ERREUR_LIGNES_MANQUANTES;
		}

        // Pour chaque caractère on associe dans le tableau le type de terrain correspondant
        for (j = 0 ; j < l ; j++) {
            switch (ligne[j]) {
                case '.' : t->tab[j][i] = LIBRE; break;
                case '#' : t->tab[j][i] = ROCHER; break;
                case '~' : t->tab[j][i] = EAU; break;
                // C est la position originale du robot ; c'est donc une case libre, et on enregistre aussi ces coordonnées
                case 'C' :
                    t->tab[j][i] = LIBRE;
                    rx = j;
                    ry = i;
					compteur_c++;
                    break;
				case '\0':
					return ERREUR_LIGNE_TROP_COURTE;
					break;
				default:
					return ERREUR_CARACTERE_INCORRECT;
            }
        }
    }
    if (compteur_c != 1) {
		return ERREUR_POSITION_ROBOT_MANQUANTE;
	}
    // On modifie les coordonnées initiales du robot
    *x = rx;
    *y = ry;


    return OK;
}

/* Renvoie la largeur d'un terrain */
int largeur(Terrain *t) {
	return t->largeur;
}

/* Renvoie la hauteur d'un terrain */
int hauteur(Terrain *t) {
	return t->hauteur;
}

/* Renvoie vrai ssi 0 <= x < largeur, 0 <= y < hauteur et si la case (x,y) du terrain est libre */
int est_case_libre(Terrain *t, int x, int y) {
	if (x >= 0 && x < t->largeur && y >= 0 && y < t->hauteur) {
		return (t->tab[x][y] == LIBRE);
	}
	return 0;
}

/* Affichage du terrain t sur la sortie standard */
void afficher_terrain(Terrain *t) {
	int i, j;
	for (i=0 ; i < t->hauteur ; i++) {
		for (j = 0 ; j < t->largeur; j++) {
			switch (t->tab[j][i]) {
				case LIBRE: printf("."); break;
				case EAU: printf("~"); break;
				case ROCHER : printf("#"); break;
			}
		}
		printf("\n");
	}
}

/* Écriture d'un terrain t dans un fichier f ouvert en écriture.
   x et y contiennent les coordonnées du robot
   Le terrain est écrit au format lisible par lire_terrain */
void ecrire_terrain(FILE *f, Terrain *t, int x, int y) {
	int i,j;
	fprintf(f, "%d\n%d\n", t->largeur, t->hauteur);
	for (i=0; i < t->hauteur ; i++) {
		for (j=0; j < t->largeur ; j++) {
			switch (t->tab[j][i]) {
				case LIBRE:
					if (j == x && i == y) {
						fprintf(f,"C");
					}
					else {
						fprintf(f,".");
					}
					break;
				case ROCHER: fprintf(f,"#"); break;
				case EAU: fprintf(f, "~"); break;
			}
		}
		fprintf(f,"\n");
	}

}
