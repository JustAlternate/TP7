#include "environnement.h"
#include "interprete.h"
#include "programme.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_TAB 100



void gestion_erreur_terrain(erreur_terrain e) {
  switch (e) {
  case OK:
    break;
  case ERREUR_FICHIER:
    printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
    exit(1);
  case ERREUR_LECTURE_LARGEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la largeur\n");
    exit(1);
  case ERREUR_LECTURE_HAUTEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la hauteur\n");
    exit(1);
  case ERREUR_LARGEUR_INCORRECTE:
    printf("Erreur lecture du terrain : largeur incorrecte\n");
    exit(1);
  case ERREUR_HAUTEUR_INCORRECTE:
    printf("Erreur lecture du terrain : hauteur incorrecte\n");
    exit(1);
  case ERREUR_CARACTERE_INCORRECT:
    printf("Erreur lecture du terrain : caractère incorrect\n");
    exit(1);
  case ERREUR_LIGNE_TROP_LONGUE:
    printf("Erreur lecture du terrain : ligne trop longue\n");
    exit(1);
  case ERREUR_LIGNE_TROP_COURTE:
    printf("Erreur lecture du terrain : ligne trop courte\n");
    exit(1);
  case ERREUR_LIGNES_MANQUANTES:
    printf("Erreur lecture du terrain : lignes manquantes\n");
    exit(1);
  case ERREUR_POSITION_ROBOT_MANQUANTE:
    printf(
        "Erreur lecture du terrain : position initiale du robot manquante\n");
    exit(1);
  }
}

void affichage_position_programme(erreur_programme e) {
  int i;
  printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
  printf("%s\n", e.ligne);
  /* Impression de e.num_colonne-1 espaces */
  for (i = 1; i < e.num_colonne; i++) {
    printf(" ");
  }
  /* Impression d'un curseur de position */
  printf("^\n");
}

void gestion_erreur_programme(erreur_programme e) {
  switch (e.type_err) {
  case OK_PROGRAMME:
    break;
  case ERREUR_FICHIER_PROGRAMME:
    printf("Erreur lecture du programme : erreur d'ouverture du fichier\n");
    exit(2);
  case ERREUR_BLOC_NON_FERME:
    printf("Erreur lecture du programme : bloc non fermé\n");
    exit(2);
  case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
    printf("Erreur lecture du programme : fermeture de bloc excédentaire\n");
    affichage_position_programme(e);
    exit(2);
  case ERREUR_COMMANDE_INCORRECTE:
    printf("Erreur lecture du programme : commande incorrecte\n");
    affichage_position_programme(e);
    exit(2);
  }
}








int main(int argc, char **argv) {
  Environnement envt;
  Programme prog;
  erreur_terrain errt;
  erreur_programme errp;
  etat_inter etat;
  resultat_inter res;

  if (argc != 2) {
    printf("Usage: %s <fichier_test> \n", argv[0]);
    return 1;
  }
  char fichier_terrain[MAX_TAB];
  char fichier_programme[MAX_TAB];
  int nb_execution;
  char reponse_attendu;

  int x_fin, y_fin;
  char o_fin;

  //on va lire le fichier:
  FILE *f;

  f = fopen(argv[1], "r");

  if (f == NULL){
    printf("le fichier test est invalide.");
  }

  if (fscanf(f, "%s", fichier_terrain) == 0){
    printf("fichier mal écrit");
  }
  if (fscanf(f, "%s", fichier_programme) == 0){
    printf("fichier mal écrit");
  }
  if (fscanf(f, "%d\n", &nb_execution) == 0){
    printf("fichier mal ecrit");
  }
  if (fscanf(f, "%c\n", &reponse_attendu) == 0){
    printf("fichier mal ecrit");
  }
  if (reponse_attendu == 'N' || reponse_attendu == 'F'){
    if (fscanf(f, "%d\n", &x_fin) == 0){
      printf("fichier mal ecrit");
    }
    if (fscanf(f, "%d\n", &y_fin) == 0){
      printf("fichier mal ecrit");
    }
    if (fscanf(f, "%c", &o_fin) == 0){
      printf("fichier mal ecrit");
    }
  }
  //printf("le fichier de terrain est: %s.\nle fichier de programme est: %s.\n", fichier_terrain, fichier_programme);

  /* Initialisation de l'environnement : lecture du terrain,
     initialisation de la position du robot */
  
  errt = initialise_environnement(&envt, fichier_terrain);
  gestion_erreur_terrain(errt);

  /* Lecture du programme */
  errp = lire_programme(&prog, fichier_programme);
  gestion_erreur_programme(errp);

  /* Initialisation de l'état */
  init_etat(&etat);
  int pas = 0;
  do
  {
    res = exec_pas(&prog, &envt, &etat);
    /* Affichage du terrain et du robot */
    //afficher_envt(&envt);
    pas++;
  } while (res == OK_ROBOT && pas < nb_execution);

  /* Affichage du résultat */
  switch (res) {
  case OK_ROBOT:
    if (reponse_attendu == 'N'){
      if (x_fin == envt.r.x && y_fin == envt.r.y){
        switch (envt.r.o)
        {
        case Est:
          if (o_fin == 'E'){
            printf("victoire le test est passé\n");
            return 1;
          }
          break;
        case Ouest:
          if (o_fin == 'O'){
            printf("victoire le test est passé\n");
            return 1;
          }
          break;
        case Nord:
          if (o_fin == 'N'){
            printf("victoire le test est passé\n");
            return 1;
          }
          break;
        case Sud:
          if (o_fin == 'S'){
            printf("victoire le test est passé\n");
            return 1;
          }
          break;
        default:
          break;
        }
      }
    }
    printf("le robot a dépasser son nombre d'éxécusion sans arriver sur la case demandée!\n");
    return 1;
    break;
  case SORTIE_ROBOT:
    if (reponse_attendu == 'S'){
      printf("victoire le test est passé\n");
      return 1;
    }
    printf("Le robot est sorti :-)\n");
    break;
  case ARRET_ROBOT:
    if (reponse_attendu == 'F'){
      if (x_fin == envt.r.x && y_fin == envt.r.y){
        switch (envt.r.o)
        {
        case Est:
          if (o_fin == 'E'){
            printf("victoire le test est passé\n");
            return 1;
          }
          break;
        case Ouest:
          if (o_fin == 'O'){
            printf("victoire le test est passé\n");
            return 1;
          }
          break;
        case Nord:
          if (o_fin == 'N'){
            printf("victoire le test est passé\n");
            return 1;
          }
          break;
        case Sud:
          if (o_fin == 'S'){
            printf("victoire le test est passé\n");
            return 1;
          }
          break;
        default:
          break;
        }
      }
    }
    printf("Robot sur une case libre, programme terminé :-/\n");
    printf("x attendu:%d,y attendu:%d,o attendu:%c\nx du robot:%d,y du robot:%d,o du robot:%d\n", x_fin, y_fin, o_fin, envt.r.x, envt.r.y, envt.r.o);
    printf("nb_execution:%d\nreponse_attendu:%c\n", nb_execution, reponse_attendu);
    break;
  case PLOUF_ROBOT:
    if (reponse_attendu == 'P'){
      printf("victoire le test est passé\n");
      return 1;
    }
    printf("Le robot est tombé dans l'eau :-(\n");
    break;
  case CRASH_ROBOT:
    if (reponse_attendu == 'O'){
      printf("victoire le test est passé\n");
      return 1;
    }
    printf("Le robot s'est écrasé sur un rocher X-(\n");
    break;
  case ERREUR_PILE_VIDE:
    printf("ERREUR : pile vide\n");
    break;
  case ERREUR_ADRESSAGE:
    printf("ERREUR : erreur d'adressage\n");
    break;
  case ERREUR_DIVISION_PAR_ZERO:
    printf("ERREUR : division par 0\n");
    break;
  }
}
