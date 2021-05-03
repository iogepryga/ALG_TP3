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

  /*
    la fonction lire_graphe alloue le graphe (matrice,...) 
    et lit les donnees du fichier passe en parametre
  */
  
  
  lire_graphe (argv [1], &g) ;

  #define NB_EXPE 20
  #define NB_SAUT_MAX 6

  init_couleur_sommet(g,0);
  init_champ_tmp_sommet(g,0);
  srand(time(NULL));

  printf("=========================\n      %d chemin(s) testé(s) :\n",NB_EXPE);
  for(int i = 0 ; i < NB_EXPE ; i++){
    printf("<--------------->\n");
    chemin_t ctmp = gen_chemin_rand_fixedsaut(g,rand()%NB_SAUT_MAX);
    print_chemin(ctmp);
    printf("longueur ? = %d\n",longueur(g,ctmp));
    printf("elementaire ? : %d\n",elementaire(g,ctmp));
    printf("simple ? : %d\n",simple(g,ctmp));
    printf("eulerien ? : %d\n",eulerien(g,ctmp));
    printf("hamiltonien ? : %d\n",hamiltonien(g,ctmp));
    // print_chemin(ctmp);
    free_chemin(ctmp);
  }
}