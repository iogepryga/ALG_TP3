#include <stdio.h>
#include <stdlib.h>

#include "graphe.h"

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

  printf("digraph aef{\n\t/* HORIZONTAL */ rankdir=LR;\n\tnode [peripheries=1];\n");
  for(pgraphe_t gc = g ; gc != NULL ; gc= gc->sommet_suivant)
    for(parc_t ac = gc->liste_arcs ; ac != NULL ; ac = ac->arc_suivant)
      printf("\t%d -> %d [label=%d];\n",gc->label,ac->dest->label,ac->poids);
  printf("}\n");
}