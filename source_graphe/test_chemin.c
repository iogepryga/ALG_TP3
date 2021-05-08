#include <stdio.h>
#include <stdlib.h>

#include "graphe.h"
#include "time.h"


parc_t select_arc_rand(psommet_t g) {
  if(g == NULL)
    return NULL;
  int nb_arcs = nombre_arc(g);
  if(nb_arcs != 0) {
    int indice = rand() % nb_arcs;
    parc_t arc_courant = g->liste_arcs;
    for(int i = 0 ; i < indice ; i++)
      arc_courant = arc_courant->arc_suivant;
    return arc_courant;
  }
  return NULL;
}

psommet_t select_sommet_rand(pgraphe_t g) {
  int nb_sommets = nombre_sommets(g);
  if(nb_sommets != 0) {
    int indice = rand() % nb_sommets;
    psommet_t g_courant = g;
    for(int i = 0 ; i < indice ; i++)
      g_courant = g_courant->sommet_suivant;
    return g_courant;
  }
  return NULL;
}

chemin_t gen_chemin_rand_fixedsaut(pgraphe_t g, int nb_saut) {
  chemin_t c; c.depart = NULL; c.liste_arcs = NULL;
  if(g != NULL) {
    c.depart = select_sommet_rand(g);
    psommet_t g_courant = c.depart;
    for(int i = 0 ; i < nb_saut ; i ++) {
      parc_t tmp = select_arc_rand(g_courant);
      if(tmp == NULL)
        return c;
      chemin_arc_push(&c,*tmp);
      g_courant = tmp->dest;
    }
  }
  return c;
}



int main (int argc, char **argv)
{
  pgraphe_t g ;
  
  if (argc != 2)
    {
      fprintf (stderr, "erreur parametre \n") ;
      exit (-1) ;
    }
  
  
  lire_graphe (argv [1], &g) ;

  #define NB_EXPE 100


  #define NB_SAUT_MAX 6
  // int NB_SAUT_MAX = nombre_arcs(g); // eulerien
  // int NB_SAUT_MAX = nombre_sommets(g) - 1; // hamiltonien


  init_couleur_sommet(g,0);
  init_champ_tmp_sommet(g,0);
  srand(time(NULL));

  printf("=========================\n      propriété graphe :\n");
  printf("graphe_eulerien(g) : %d\n", graphe_eulerien(g));
  printf("graphe_hamiltonien(g) : %d\n", graphe_hamiltonien(g));

  printf("=========================\n      %d chemin(s) testé(s) :\n",NB_EXPE);
  for(int i = 0 ; i < NB_EXPE ; i++){
    printf("<--------------->\n");

    chemin_t ctmp = gen_chemin_rand_fixedsaut(g,NB_SAUT_MAX);
    // chemin_t ctmp = gen_chemin_rand_fixedsaut(g,rand()%(NB_SAUT_MAX+1));

    print_chemin(ctmp);
    printf("longueur ?                                   = %d\n",longueur(g,ctmp));
    printf("elementaire            (sommets distincts) ? : %d\n",elementaire(g,ctmp));
    printf("simple                      (arc distinct) ? : %d\n",simple(g,ctmp));
    printf("eulerien       (tout les arcs et distinct) ? : %d\n",eulerien(g,ctmp)); //if(eulerien(g,ctmp))printf("LAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
    printf("hamiltonien (tout les sommets et distinct) ? : %d\n",hamiltonien(g,ctmp));// if(hamiltonien(g,ctmp))printf("LEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n");
    free_chemin(ctmp);
  }
}