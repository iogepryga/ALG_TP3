#include <stdio.h>
#include <stdlib.h>

#include "graphe.h"

int main (int argc, char **argv)
{
  pgraphe_t g ;
  int nc ;
  
  if (argc != 2)
    {
      fprintf (stderr, "erreur parametre \n") ;
      exit (-1) ;
    }

  /*
    la fonction lire_graphe alloue le graphe (matrice,...) 
    et lit les donnees du fichier passe en parametre
  */
  
  
  lire_graphe (argv [1], &g) ;

  /*
    la fonction ecrire_graphe affiche le graphe a l'ecran
  */  
  
  printf ("nombre de sommets du graphe %d nombre arcs %d \n", nombre_sommets (g), nombre_arcs (g)) ;
  fflush (stdout) ;
  
  ecrire_graphe (g) ;      

  nc = colorier_graphe (g) ;
  
  printf ("nombre chromatique graphe = %d\n", nc) ;

  ecrire_graphe_colorie (g) ;


  printf("=========================\n      nombre_sommets(g) = %d\n", nombre_sommets(g));

  printf("=========================\n      nombre_sommets(g) = %d\n", nombre_arcs(g));

  printf("=========================\n      nombre_arc\n");
  for(pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant) {
    printf("<----------------------->\n");
    printf("nombre_arc(g_courant) (label:%d) = %d\n",g_courant->label,nombre_arc(g_courant));
  }

  printf("=========================\n      init_couleur_sommet(g,9)\nAvant :\n");
  afficher_graphe_sommet(g);
  printf("Apres :\n");
  init_couleur_sommet(g,9);
  afficher_graphe_sommet(g);

  printf("=========================\n      init_champ_tmp_sommet(g,9)\nAvant :\n");
  afficher_graphe_sommet(g);
  printf("Apres :\n");
  init_champ_tmp_sommet(g,9);
  afficher_graphe_sommet(g);

  printf("=========================\n      afficher_graphe_largeur\n");
  for(pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant) {
    printf("<-----------------------> (label:%d) \n",g_courant->label);
    afficher_graphe_largeur(g,g_courant->label);
    printf("\n");
  }

  printf("=========================\n      afficher_graphe_profondeur\n");
  for(pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant) {
    printf("<-----------------------> (label:%d) \n",g_courant->label);
    afficher_graphe_profondeur(g,g_courant->label);
    printf("\n");
  }

  // printf("=========================\n      algo_dijkstra\n");
  // for(pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant) {
  //   printf("<-----------------------> (label:%d) \n",g_courant->label);
  //   algo_dijkstra(g,g_courant->label);
  //   // afficher_graphe_largeur(g,g_courant->label);
  //   afficher_graphe_sommet(g);
  //   printf("\n");
  // }

  printf("=========================\n      degre_sortant_sommet\n");
  for(pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant) {
    printf("<----------------------->\n");
    printf("degre_sortant_sommet(g,g_courant) (label:%d) = %d\n",g_courant->label,degre_sortant_sommet(g,g_courant));
  }

  printf("=========================\n      degre_entrant_sommet\n");
  for(pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant) {
    printf("<----------------------->\n");
    printf("degre_entrant_sommet(g,g_courant) (label:%d) = %d\n",g_courant->label,degre_entrant_sommet(g,g_courant));
  }

  printf("=========================\n      degre_maximal_graphe(g) = %d\n", degre_maximal_graphe(g));

  printf("=========================\n      degre_minimal_graphe(g) = %d\n", degre_minimal_graphe(g));

  printf("=========================\n      independant(g) : %d\n", independant(g));

  printf("=========================\n      complet(g) : %d\n", complet(g));

  printf("=========================\n      regulier(g) : %d\n", regulier(g));

  // printf("=========================\n      graphe_eulerien(g) : %d\n", graphe_eulerien(g));

  // printf("=========================\n      graphe_hamiltonien(g) : %d\n", graphe_hamiltonien(g));

  // printf("=========================\n      distance\n<----------------------->\n");
  // for(pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant)
  //   for(pgraphe_t sub_g_courant = g ; sub_g_courant != NULL ; sub_g_courant = sub_g_courant->sommet_suivant) 
  //     printf("distance(g,%d,%d) = %d\n",g_courant->label,sub_g_courant->label,distance(g,g_courant->label,sub_g_courant->label));

  printf("=========================\n      excentricite\n");
  for(pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant) {
    printf("<----------------------->\n");
    printf("excentricite(g,g_courant->label) (label:%d) = %d\n",g_courant->label,excentricite(g,g_courant->label));
    afficher_graphe_sommet(g);
    printf("\n");
  }

  printf("=========================\n      diametre(g) : %d\n", diametre(g));
  afficher_graphe_sommet(g);















}
