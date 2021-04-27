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
#include "file.h"


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

void init_champ_tmp_sommet (pgraphe_t g, int x) {
  psommet_t p = g ;

  while (p != NULL) {
    p->tmp = x ; // couleur indefinie
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
  // if(g == NULL)
  //   return;
  // init_champ_tmp_sommet(g,-1);
  // fap f = creer_fap_vide();
  // g = chercher_sommet(g,r);
  // if(g == NULL)
  //   return;
  // g->tmp = 0;
  // f = inserer(f,g,0);
  
  // pgraphe_t gtmp; int prio;
  // while(!est_fap_vide(f)) {
  //   f = extraire_tete(f,&gtmp,&prio);
  //   printf("%d ",gtmp->label);
  //   parc_t arc_courant = gtmp->liste_arcs;
  //   while( arc_courant != NULL) {
  //     int dist = gtmp->tmp+arc_courant->poids;
  //     if(arc_courant->dest->tmp == -1) { // possibilité de factorisé
  //       arc_courant->dest->tmp = dist; // la
  //       f = inserer(f,arc_courant->dest,dist); // la
  //     } else if(arc_courant->dest->tmp > dist) {
  //       f = extraire_label(f,&gtmp,&prio,arc_courant->dest->label);
  //       arc_courant->dest->tmp = dist; // avec la
  //       f = inserer(f,arc_courant->dest,dist); // avec la	
  //     }
  //   }
  // }
  

  if (g == NULL)
		return;

  // On initialise tous les champs tmp à 0 (parcours vierge)
  init_champ_tmp_sommet(g, 0);
  // On crée une file vide
  pfile_t file = creer_file();

  psommet_t sommet = chercher_sommet(g, r);
  if (sommet == NULL)
    return;

  enfiler(file, sommet);
  while (!file_vide(file)) {
    sommet = (psommet_t) defiler(file);
    printf("%d ", sommet->label);
    sommet->tmp = 1;
    
    parc_t arc = sommet->liste_arcs;
    while (arc != NULL) {
      if (arc->dest->tmp == 0)
        enfiler(file, arc->dest);
      arc = arc->arc_suivant;
    }
  }

  // Retour à la ligne
  puts("");
}



void afficher_graphe_profondeur_rec (pgraphe_t g) {
  if(g != NULL) {
    g->tmp = 1;
    parc_t arc_courant = g->liste_arcs;
    while(arc_courant != NULL) {
      if(arc_courant->dest->tmp == 0)
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
  init_champ_tmp_sommet(g,0);
  g = chercher_sommet(g,r);
  if(g == NULL)
    return;
  afficher_graphe_profondeur_rec(g);
}

void algo_dijkstra (pgraphe_t g, int r) {
  /*
    algorithme de dijkstra
    des variables ou des chanmps doivent etre ajoutees dans les structures.
  */

  if(g == NULL)
      return;
      
 //init arbre
  init_champ_tmp_sommet (g, INT_MAX);

 //init tableau parent
  int nb_sommets = nombre_sommets (g);
  int Parent[nb_sommets];
  for (int i = 0; i < nb_sommets; i ++){
    Parent[i] = NULL;
  }

 //init tableau sommets / file
  psommet_t* Tableau_sommets = tableau_liste_sommets(g);

 //init tableau distance sommet
  int* Tableau_dist_sommets[nb_sommets];
  for (int i = 0; i < nb_sommets; i++){
    Tableau_dist_sommets[i] = INT_MAX;
  }
  
 //init sommet r
  psommet_t sommet = chercher_sommet (g, r);
  sommet -> tmp = 0;
  int indice = indiceOff(Tableau_sommets, nb_sommets, r);
  if ( indice != -1 )
    Tableau_dist_sommets[indice] = 0;
  
  
  while (!tableeau_est_vide(Tableau_sommets,nb_sommets)){

    if (plus_petite_distance(Tableau_dist_sommets, nb_sommets, Tableau_sommets) == -1)
      break;
    
    g = Tableau_sommets[plus_petite_distance(Tableau_dist_sommets, nb_sommets, Tableau_sommets)];

    parc_t arc = g -> liste_arcs;
    int indice_sommet = indiceOff(Tableau_sommets, nb_sommets, g -> label);
    int indice_dest = indiceOff(Tableau_sommets, nb_sommets, arc -> dest -> label);
    while (arc != NULL){
    
      if (indice_sommet == -1 || indice_dest == -1)
          break;

      if ( Tableau_dist_sommets[indice_dest] > ( Tableau_dist_sommets[indice_sommet] + arc -> poids)){
        Tableau_dist_sommets[indice_dest] = (Tableau_dist_sommets[indice_sommet] + arc -> poids);
        g -> tmp = Tableau_dist_sommets[indice_sommet] + arc -> poids;
        Parent[indice_dest] = g -> label;
      }
      
      arc = arc -> arc_suivant;
    }
    
    Tableau_sommets [indice_sommet] = NULL;
  }


}
//Fonctions pour algo de dijkstra
int plus_petite_distance (int* Tab, int len, psommet_t* File){
  int indice = 0;
  while (File[indice] == NULL && indice < len)
    indice++;
  
  if (indice == len)
    return -1;

  int res = Tab[indice];
  for (int i = indice + 1; i < len; i++){
    if (Tab[i] < Tab[res] && File[i] != NULL)
      res = i;
  }
  return res;
}

int indiceOff (psommet_t* Tab,int len, int elem){
  for (int i = 0; i < len; i++){
    if (Tab[i] -> label == elem)
      return i;
  }
  return -1;
}

psommet_t* tableau_liste_sommets (pgraphe_t g){
  psommet_t Tab[nombre_sommets (g)];
  int i = 0;
  while (g != NULL){
    Tab[i] = g;
    g = g -> sommet_suivant;
    i++;
  }
  return Tab;
}

int tableeau_est_vide (psommet_t* Tab, int len){
  for (int i = 0; i < len; i++){
    if (Tab[i] != NULL)
      return 0;
  }
  return 1;
}



// ======================================================================




int degre_sortant_sommet (pgraphe_t g, psommet_t s)
{
  /*
    Cette fonction retourne le nombre d'arcs sortants 
    du sommet n dans le graphe g
  */
  if(g == NULL || s == NULL)
    return 0;
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
    return 0;
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
 	if(g == NULL)
    return 0;
	int max = INT_MAX,tmp;
	for (pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant) {
		tmp = degre_entrant_sommet(g,g_courant) + degre_sortant_sommet(g,g_courant);
		if(tmp > max)
			max = tmp;
	}
  return max;
}


int degre_minimal_graphe (pgraphe_t g) // sortant ou entrant ?!
{
  /*
    Min des degres des sommets du graphe g
  */
	if(g == NULL)
		return 0;
	int min = 0,tmp;
	for (pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant) {
		tmp = degre_entrant_sommet(g,g_courant) + degre_sortant_sommet(g,g_courant);
		if(tmp > min)
			min = tmp;
	}
  return min;
}


int independant (pgraphe_t g)
{
  /* Les aretes du graphe n'ont pas de sommet en commun */
	if(g == NULL)
		return 0;
	int d = degre_maximal_graphe(g);
  return d == 0 || d == 1 ;
}

#define Onveutquoi 1 // 1 ou 2 ?

int complet (pgraphe_t g) // algo bourrin dégeulasse
{
  /* Toutes les paires de sommet du graphe sont jointes par un arc */
	if(g == NULL)
		return 0;
	int nb_sommets = nombre_sommets(g), tmp;
	for(pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant)
		if((degre_entrant_sommet(g,g_courant) + degre_sortant_sommet(g,g_courant)) != Onveutquoi*(nb_sommets-1))
			return 0;
  return 1;
}

int regulier (pgraphe_t g)
{
  /* 
     graphe regulier: tous les sommets ont le meme degre
     g est le ponteur vers le premier sommet du graphe
     renvoie 1 si le graphe est régulier, 0 sinon
  */
	if(g == NULL)
		return 0;
  return (degre_minimal_graphe(g) == degre_maximal_graphe(g));
}




/*
  placer les fonctions de l'examen 2017 juste apres
*/

void chemin_arc_push(pchemin_t c, arc_t arc) { // (=append)
	if(c == NULL)
		return;
	parc_t new_arc = (parc_t)malloc(sizeof(arc_t)); *new_arc = arc; new_arc->arc_suivant = NULL;
	if(c->liste_arcs == NULL) {
		c->liste_arcs = new_arc;
	} else {
		parc_t arc_courant = c->liste_arcs;
		while(arc_courant->arc_suivant != NULL) {
			arc_courant = arc_courant->arc_suivant;
		}
		arc_courant = new_arc;
	}
}

void chemin_arc_remove_last(pchemin_t c) {
	if(c == NULL || c->liste_arcs == NULL )
		return;
	parc_t arc_courant = c->liste_arcs;
	if(c->liste_arcs->arc_suivant == NULL) {
		free(c->liste_arcs);
		c->liste_arcs = NULL;
	} else {
		parc_t arc_courant = c->liste_arcs;
		while(arc_courant->arc_suivant->arc_suivant != NULL) {
			arc_courant = arc_courant->arc_suivant;
		}
		free(arc_courant->arc_suivant);
		arc_courant->arc_suivant = NULL;
	}
}

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
  for(parc_t arc_courant = c.liste_arcs; arc_courant != NULL ; arc_courant = arc_courant->arc_suivant){
    for(parc_t sub_arc_courant = arc_courant->arc_suivant ; sub_arc_courant != NULL ; sub_arc_courant = sub_arc_courant->arc_suivant)
      if(sub_arc_courant == arc_courant)
        return 0;
	}
  return 1;
}

int eulerien(pgraphe_t g, chemin_t c) { 
  if(g == NULL)
    return -1;
  for(pgraphe_t g_courant = g ; g_courant->sommet_suivant != NULL ; g_courant = g_courant->sommet_suivant) {
    for(parc_t arc_sommet_courant = g_courant->liste_arcs ; arc_sommet_courant != NULL ; arc_sommet_courant = arc_sommet_courant->arc_suivant) {

			pgraphe_t g_depart = c.depart ;
      parc_t arc_chemin_courant = c.liste_arcs;
      for(; arc_chemin_courant != NULL ; arc_chemin_courant = arc_chemin_courant->arc_suivant) {
        if(g_courant == g_depart && arc_sommet_courant->dest == arc_chemin_courant->dest)
          break;
				g_depart = arc_chemin_courant->dest;
			}
      if(arc_chemin_courant == NULL ){
        return 0;
      }
			
    }
  }
  return 1;
}

int hamiltonien(pgraphe_t g, chemin_t c) {
  if(g == NULL)
    return -1;
  for(pgraphe_t g_courant = g ; g_courant->sommet_suivant != NULL ; g_courant = g_courant->sommet_suivant) {
    parc_t arc_chemin_courant = c.liste_arcs;
    for(pgraphe_t sommet_courant = c.depart; arc_chemin_courant != NULL ; sommet_courant = arc_chemin_courant->dest, arc_chemin_courant = arc_chemin_courant->arc_suivant)
      if(g_courant == sommet_courant)
        break;
    if(arc_chemin_courant == NULL )
      return 0;
  }
  return 1;
}


int graphe_eulerien(pgraphe_t g) {

}

int graphe_hamiltonien(pgraphe_t g) {

}


int longueur(pgraphe_t g, chemin_t c) {
  if(g == NULL)
    return 0;
  int somme = 0;
  for(parc_t arc_courant = c.liste_arcs ; arc_courant != NULL ; arc_courant = arc_courant->arc_suivant)
    somme += arc_courant->poids;
    return somme;
}

int distance(pgraphe_t g, int x, int y) { // attention : destructeur couleur
  if(g == NULL)
    return 0;
  pgraphe_t sommet_x = chercher_sommet(g,x);
  pgraphe_t sommet_y = chercher_sommet(g,y);
  if(sommet_x == NULL || sommet_y == NULL)
    return 0;
  algo_dijkstra(g,x);
  return sommet_y->tmp;
}






void distance_max_rec(pgraphe_t g,chemin_t c) {
  if(g != NULL) {
    int dist;
    for(parc_t arc_courant = g->liste_arcs; arc_courant != NULL; arc_courant = arc_courant->arc_suivant) {
      dist = g->tmp + arc_courant->poids;
      if(arc_courant->dest->tmp < dist) {
				chemin_arc_push(&c,*arc_courant);
				if(simple(g,c)) {
					arc_courant->dest->tmp = dist;
					distance_max_rec(arc_courant->dest,c);
				}
				chemin_arc_remove_last(&c);
      }
    }
  }
}
void distance_max(pgraphe_t g, int r) {
  if(g == NULL)
    return;
  init_champ_tmp_sommet(g,0);
  pgraphe_t g = chercher_sommet(g,r);
  if(g == NULL)
    return;
	chemin_t c; c.depart = g;c .liste_arcs = NULL;
  g->tmp = 0;
  distance_max_rec(g,c);
}

int excentricite(pgraphe_t g, int n) {
	if(g == NULL)
    return 0;
	distance_max(g,n);
	int max = 0;
	for(pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant)
		if(g_courant->tmp > max)
			max = g_courant->tmp;
	return max;
}

int diametre(pgraphe_t g) {
	if(g == NULL)
    return 0;
	int max = 0,tmp;
	for(pgraphe_t g_courant = g ; g_courant->sommet_suivant != NULL ; g_courant = g_courant->sommet_suivant) {
		tmp = excentricite(g,g->label);
		if(tmp > max)
			max = tmp;
	}
	return max;
}