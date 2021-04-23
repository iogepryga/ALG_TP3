/*
  Structures de type graphe
  Structures de donnees de type liste
  (Pas de contrainte sur le nombre de noeuds des  graphes)
*/


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "graphe.h"
#include "fap.h"


psommet_t chercher_sommet (pgraphe_t g, int label) {
  psommet_t s ;
  s = g ;

  while ((s!= NULL) && (s->label != label)) {
    s = s->sommet_suivant ;
  }
  return s ;
}

parc_t existence_arc (parc_t l, psommet_t s) {
  parc_t p = l ;

  while (p != NULL) {
    if (p->dest == s)
	    return p ;
    p = p->arc_suivant ;
  }
  return p ; 
}


void ajouter_arc (psommet_t o, psommet_t d, int distance) {
  parc_t parc = (parc_t) malloc (sizeof(arc_t)) ;

  if (existence_arc (o->liste_arcs, d) != NULL) {
    fprintf(stderr, "ajout d'un arc deja existant\n") ;
    exit (-1) ;
  }
  parc->poids = distance ;
  parc->dest = d ;
  parc->arc_suivant = o->liste_arcs ;
  o->liste_arcs = parc ;
  return ;
}



// ===================================================================

int nombre_sommets (pgraphe_t g) {
  psommet_t p = g ;
  int nb = 0 ;

  while (p != NULL) {
    nb = nb + 1 ;
    p = p->sommet_suivant ;
  }
  return nb ;
}

int nombre_arcs (pgraphe_t g) {
  psommet_t p = g ;
  int nb_arcs = 0 ;

  while (p != NULL) {
    parc_t l = p->liste_arcs ;
    while (l != NULL) {
      nb_arcs = nb_arcs + 1 ;
	    l = l->arc_suivant ;
	  }  
    p = p->sommet_suivant ;
  }
  return nb_arcs ;
}

void init_couleur_sommet (pgraphe_t g, int x) {
  psommet_t p = g ;

  while (p != NULL) {
    p->couleur = x ; // couleur indefinie
    p = p->sommet_suivant ; // passer au sommet suivant dans le graphe
  }

  return ;
}

int colorier_graphe (pgraphe_t g) {
  /*
    coloriage du graphe g
    
    datasets
    graphe data/gr_planning
    graphe data/gr_sched1
    graphe data/gr_sched2
  */

  psommet_t p = g ;
  parc_t a ;
  int couleur ;
  int max_couleur = INT_MIN ; // -INFINI
  int change ;
  init_couleur_sommet (g,0) ;
  
  while (p != NULL) {
    couleur = 1 ; // 1 est la premiere couleur
      // Pour chaque sommet, on essaie de lui affecter la plus petite couleur
      // Choix de la couleur pour le sommet p
    do {
	    a = p->liste_arcs ;
	    change = 0 ;
	    while (a != NULL) {
	      if (a->dest->couleur == couleur) {
		      couleur = couleur + 1 ;
		      change = 1 ;
		    } 
	      a = a->arc_suivant ; 
	    }
    } while (change == 1) ;

      // couleur du sommet est differente des couleurs de tous les voisins
      
    p->couleur = couleur ;
    if (couleur > max_couleur)
	    max_couleur = couleur ;
    p = p->sommet_suivant ;
  }
  return max_couleur ;
}

// ======================================================================


void afficher_graphe_largeur (pgraphe_t g, int r) {
  /*
    afficher les sommets du graphe avec un parcours en largeur
  */
  if(g == NULL)
    return;
  init_couleur_sommet(g,-1);
  fap f = creer_fap_vide();
  g = chercher_sommet(g,r);
  if(g == NULL)
    return;
  g->couleur = 0;
  f = inserer(f,g,0);
  
  pgraphe_t gtmp; int prio;
  while(!est_fap_vide(f)) {
    f = extraire_tete(f,&gtmp,&prio);
    printf("%d ",gtmp->label);
    parc_t arc_courant = gtmp->liste_arcs;
    while( arc_courant != NULL) {
      int dist = gtmp->couleur+arc_courant->poids;
      if(arc_courant->dest->couleur == -1) { // possibilité de factorisé
        arc_courant->dest->couleur = dist; // la
        f = inserer(f,arc_courant->dest,dist); // la
      } else if(arc_courant->dest->couleur > dist) {
        f = extraire_label(f,&gtmp,&prio,arc_courant->dest->label);
        arc_courant->dest->couleur = dist; // avec la
        f = inserer(f,arc_courant->dest,dist); // avec la
      }
    }
  }
}




void afficher_graphe_profondeur_rec (pgraphe_t g) {
  if(g != NULL) {
    g->couleur = 0;
    parc_t arc_courant = g->liste_arcs;
    while(arc_courant != NULL) {
      if(arc_courant->dest->couleur == -1)
        afficher_graphe_profondeur_rec(arc_courant->dest);
      arc_courant = arc_courant->arc_suivant;
    }
    printf("%d ", g->label); // post fix
  }
}
void afficher_graphe_profondeur (pgraphe_t g, int r) {
  /*
    afficher les sommets du graphe avec un parcours en profondeur
  */
  if(g == NULL)
    return;
  init_couleur_sommet(g,-1);
  g = chercher_sommet(g,r);
  if(g == NULL)
    return;
  afficher_graphe_profondeur_rec(g);
}


void algo_dijkstra_rec (pgraphe_t g) {
  if(g != NULL) {
    parc_t arc_courant = g->liste_arcs;
    int dist;
    while(arc_courant != NULL) {
      dist = g->couleur + arc_courant->poids;
      if(arc_courant->dest->couleur == -1 || arc_courant->dest->couleur > dist) {
        arc_courant->dest->couleur = dist;
        afficher_graphe_profondeur_rec(arc_courant->dest);
      }
      arc_courant = arc_courant->arc_suivant;
    }
  }
}

void algo_dijkstra (pgraphe_t g, int r) {
  /*
    algorithme de dijkstra
    des variables ou des chanmps doivent etre ajoutees dans les structures.
  */
  if(g == NULL)
    return;
  init_couleur_sommet(g,-1);
  g = chercher_sommet(g,r);
  if(g == NULL)
    return;
  g->couleur = 0;
  algo_dijkstra_rec(g);
}





// ======================================================================




int degre_sortant_sommet (pgraphe_t g, psommet_t s)
{
  /*
    Cette fonction retourne le nombre d'arcs sortants 
    du sommet n dans le graphe g
  */
  if(g == NULL || s == NULL)
    return -1;
  int nb_arcs = 0 ;

  parc_t arc_courant = s->liste_arcs ;
  for(nb_arcs = 0; arc_courant != NULL ; nb_arcs++)
    arc_courant = arc_courant->arc_suivant ;
  return nb_arcs ;
}

int degre_entrant_sommet (pgraphe_t g, psommet_t s)
{
  /*
    Cette fonction retourne le nombre d'arcs entrants 
    dans le noeud n dans le graphe g
  */
  if(g == NULL || s == NULL)
    return -1;
  int nb_arcs = 0 ;

  for (pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant)
    for(parc_t arc_courant = g->liste_arcs ; arc_courant != NULL ; arc_courant = arc_courant->arc_suivant )
      if(arc_courant->dest == s)
        nb_arcs++;

  return nb_arcs ;
}

int degre_maximal_graphe (pgraphe_t g) // sortant ou entrant ?!
{
  /*
    Max des degres des sommets du graphe g
  */

  return 0 ;
}


int degre_minimal_graphe (pgraphe_t g) // sortant ou entrant ?!
{
  /*
    Min des degres des sommets du graphe g
  */

  return 0 ;
}


int independant (pgraphe_t g)
{
  /* Les aretes du graphe n'ont pas de sommet en commun */

  return 0 ;
}



int complet (pgraphe_t g)
{
  /* Toutes les paires de sommet du graphe sont jointes par un arc */

  return 0 ;
}

int regulier (pgraphe_t g)
{
  /* 
     graphe regulier: tous les sommets ont le meme degre
     g est le ponteur vers le premier sommet du graphe
     renvoie 1 si le graphe est régulier, 0 sinon
  */

  return 0 ;
}




/*
  placer les fonctions de l'examen 2017 juste apres
*/

int elementaire (pgraphe_t g, chemin_t c) {
  if(g == NULL)
    return -1;
  pgraphe_t g_courant = c.depart;
  for(parc_t arc_courant = c.liste_arcs ; arc_courant != NULL ;  g_courant = arc_courant->dest, arc_courant = arc_courant->arc_suivant )
    for(parc_t sub_arc_courant = arc_courant ; sub_arc_courant != NULL ; sub_arc_courant = sub_arc_courant->arc_suivant)
      if(sub_arc_courant->dest == g_courant)
        return 0;
  return 1;
}

// int nb_arcs(chemin_t c) {
//   int nb_arcs = 0 ;
//   for(parc_t arc_courant = c.liste_arcs, nb_arcs = 0; arc_courant != NULL ; nb_arcs++)
//     arc_courant = arc_courant->arc_suivant ;
//   return nb_arcs ;
// }

int simple(pgraphe_t g, chemin_t c) {
  if(g == NULL)
    return -1;
  for(parc_t arc_courant = c.liste_arcs ; arc_courant != NULL ; arc_courant = arc_courant->arc_suivant)
    for(parc_t sub_arc_courant = arc_courant->arc_suivant ; sub_arc_courant != NULL ; sub_arc_courant = sub_arc_courant->arc_suivant)
      if(sub_arc_courant == arc_courant)
        return 0;
  return 1;
}

int eulerien(pgraphe_t g, chemin_t c) {

}

int hamiltonien(pgraphe_t g, chemin_t c) {

}

int graphe_eulerien(pgraphe_t g) {

}

int graphe_hamiltonien(pgraphe_t g) {

}

int distance(pgraphe_t g, int x, int y) {

}

int excentricite(pgraphe_t g, int n) {

}

int diametre(pgraphe_t g) {

}