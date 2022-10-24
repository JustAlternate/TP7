
#include "terrain.h"
#include "robot.h"
#include <stdio.h>
#include <string.h>

erreur_terrain lire_terrain(FILE *f, Terrain *t, Robot *robot) {
  int l, h;   // Dimensions du terrain
  char temp; //pour la gestion d'erreur
  // TEST POUR SAVOIR SI LES COORD ROBOT SONT BIEN CHANGES
  robot->x=-1;//on initialise à -1 pour gérer l'erreur ou le depart du robot est manquant
  robot->y=-1;


  if (f == NULL) {
    return ERREUR_FICHIER;
  }

  if (robot == NULL){
    return ERREUR_POINTEUR_ROBOT_NULLE;
  }

  // Lecture de la largeur
  if (fscanf(f,"%d\n",&l) == 0){
    return ERREUR_LARGEUR_MANQUANTE;
  }
  if (l > DIM_MAX){
    return ERREUR_DEPASSEMENT_LARGEUR;
  }
  t->largeur = l;


  // Lecture de la hauteur
  if (fscanf(f,"%d\n",&h) == 0){
    return ERREUR_HAUTEUR_MANQUANTE;
  }
  if (h > DIM_MAX){
    return ERREUR_DEPASSEMENT_HAUTEUR;
  }
  t->hauteur = h;

  // Lecture du terrain
  char obstacle;
  for (int ligne = 0; ligne < h; ligne++){
    for (int colonne = 0; colonne < l; colonne++){
      if (fscanf(f,"%c",&obstacle) == 0){
        return ERREUR_FICHIER_TROP_COURT;
      }
      switch(obstacle){
        case '#':
          t->tab[colonne][ligne]=ROCHER;
          break;
        case '~':
          t->tab[colonne][ligne]=EAU;
          break;
        case '.':
          t->tab[colonne][ligne]=LIBRE;
          break;
        case 'C':
          t->tab[colonne][ligne]=LIBRE;
          robot->x = colonne;
          robot->y = ligne;
          break;
        default:
          return ERREUR_CARACTERE;
      }
    }
    if (fscanf(f,"%c",&temp) == 0){
      return ERREUR_FICHIER_TROP_COURT;
    }
    if (temp != '\n'){
      return ERREUR_LIGNE_TROP_LONGUE;
    }// Pour skip le \n de fin de ligne.
  }
  robot->o = Est;
  if (robot->x != -1 && robot->y != -1){
    return OK;
  } 
  return ERREUR_DEPART_ROBOT_MANQUANT;

}


void afficher_terrain(Terrain *t){
  for (int x=0; x < t->hauteur;x++){
    for(int y=0;y<t->largeur; y++){
      switch(t->tab[y][x]){
        case LIBRE:
          printf(".");
          break;
        case EAU:
          printf("~");
          break;
        case ROCHER:
          printf("#");
          break;
      }
    }
    printf("\n");
  }
}
