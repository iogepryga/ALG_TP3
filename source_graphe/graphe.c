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

#define degre_sommet(g,gc) (degre_entrant_sommet(g,gc)+degre_sortant_sommet(g,gc))

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
  /*
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6
  */
  psommet_t p = g ;
  int nb = 0 ;

  while (p != NULL) {
    nb = nb + 1 ;
    p = p->sommet_suivant ;
  }
  return nb ;
}

int nombre_arcs (pgraphe_t g) {
  /*
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6
  */
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
  /*
    /!\ modifie couleur /!\
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6
  */
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

    /!\ modifie couleur /!\
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


// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// FONCTION SUR GRAPHE :
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

// <------------------------------------------------>
// FONCTION SUR GRAPHE A RENDRE :
// <------------------------------------------------>

// QUESTION 1
void afficher_graphe_largeur (pgraphe_t g, int r) {
  /*
    /!\ modifie tmp /!\
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6
  */
  /*
    afficher les sommets du graphe avec un parcours en largeur
  */
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
    printf("%d(c=%d)  ", sommet->label, sommet->couleur);
    sommet->tmp = 1;
    

    for(parc_t arcc = sommet->liste_arcs; arcc != NULL ; arcc = arcc->arc_suivant) {
      if (arcc->dest->tmp == 0) {
        enfiler(file, arcc->dest);
        arcc->dest->tmp = 1;
      }
    }
  }

  // Retour à la ligne
  printf("\n");
}

// QUESTION 2
void afficher_graphe_profondeur_rec (pgraphe_t g) {
  /*
    /!\ modifie tmp /!\
    debuggé avec : gr0, gr1, gr2, gr4, gr5, gr6
  */
  if(g != NULL) {
    g->tmp = 1;
    printf("%d(c=%d)  ", g->label, g->couleur); // pre fix
    for(parc_t arc_courant = g->liste_arcs;arc_courant != NULL;arc_courant = arc_courant->arc_suivant)
      if(arc_courant->dest->tmp == 0)
        afficher_graphe_profondeur_rec(arc_courant->dest);
  }
}
void afficher_graphe_profondeur (pgraphe_t g, int r) {
  /*
    afficher les sommets du graphe avec un parcours en profondeur
    /!\ modifie tmp /!\
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6
  */
  if(g == NULL)
    return;
  init_champ_tmp_sommet(g,0);
  g = chercher_sommet(g,r);
  if(g == NULL)
    return;
  afficher_graphe_profondeur_rec(g);
  printf("\n");
}


















//Fonctions pour algo de dijkstra
int plus_petite_distance (int* Tab, int len, pgraphe_t* File){
  
  //Autre code pour tanter de résoudre le problème de segmentation (ne résout pas le problème)
  /*
  int indice = -1;
  int res = INT_MAX;
  for (int i = 0; i < len; i++)
  {
    if (Tab[i] < res && File[i] != NULL){
      indice = i;
      res = Tab[i];
    }
  }
    return indice;
  */
  

 int indice = 0;
  while (File[indice] == NULL && indice < len)
    indice++;
  
  if (File[indice] == NULL && indice == len)
    return -1;

  int res = *(Tab+indice);

  if (indice == len)
      return res;

  //problème de segmentation fault à certains appels de cette boucle (pas trouvé la cause)
  for (int i = indice + 1; i < len; i++){
    if (Tab[i] < res && File[i] != NULL)
      res = Tab[i];
  }
  return res;

}



int indiceOff (pgraphe_t* Tab,int len, int elem){
  for (int i = 0; i < len; i++){
    if (Tab[i] -> label == elem)
      return i;
  }
  return -1;
}

pgraphe_t* tableau_liste_sommets (pgraphe_t g){
  pgraphe_t* Tab = malloc(sizeof(pgraphe_t) * nombre_sommets (g));
  int i = 0;
  while (g != NULL){
    Tab[i] = g;
    g = g -> sommet_suivant;
    i++;
  }
  return Tab;
}

int tableeau_est_vide (pgraphe_t* Tab, int len){
  for (int i = 0; i < len; i++){
    if (Tab[i] != NULL)
      return 0;
  }
  return 1;
}

// QUESTION 3
void algo_dijkstra (pgraphe_t g, int r) {
  if(g == NULL)
      return;
      
 //init arbre
  init_couleur_sommet (g, INT_MAX);

 //init tableau parent
  int nb_sommets = nombre_sommets (g);
 //init tableau sommets / file
  pgraphe_t* Tableau_sommets = tableau_liste_sommets(g);
 //init tableau distance sommet
  int* Tableau_dist_sommets = malloc(sizeof(int) * nb_sommets);
  for (int i = 0; i < nb_sommets; i++){
    Tableau_dist_sommets[i] = INT_MAX;
  }
  
 //init sommet r
  psommet_t sommet = chercher_sommet (g, r);
  sommet -> couleur = 0;
  int indice = indiceOff(Tableau_sommets, nb_sommets, r);
  if ( indice != -1 )
    Tableau_dist_sommets[indice] = 0;
  // Tant que la file (tableau sommets utilisé comme file) est non vide
  while (!tableeau_est_vide(Tableau_sommets,nb_sommets)){
    
    //vérification
    if (plus_petite_distance(Tableau_dist_sommets, nb_sommets, Tableau_sommets) == -1)
      break;
    //init sommet étudié
    //les distances étant init à INT_MAX, les sommets non encore parcouru ne sont pas pris en compte
    //Au début on commence alors par le sommet de départ r qui a la seul distance < INT_MAX qui vaut 0 (init juste avant)
    g = Tableau_sommets[plus_petite_distance(Tableau_dist_sommets, nb_sommets, Tableau_sommets)];
    //init variables utiles
    parc_t arc = g -> liste_arcs;
    int indice_sommet = indiceOff(Tableau_sommets, nb_sommets, g -> label);
    int indice_dest = indiceOff(Tableau_sommets, nb_sommets, arc -> dest -> label);
    //Tant qu'on a pas fini d'explorer les sommets accessible depuis le sommet en cours d'étude
    while (arc != NULL){
      
      //vérification
      if (indice_sommet == -1 || indice_dest == -1)
          break;

      //Si distance du sommet destination supérieur à distance sommet étudier plus poids du parcours de l'arc  
      if ( Tableau_dist_sommets[indice_dest] > ( Tableau_dist_sommets[indice_sommet] + arc -> poids)){

        //alors on actualise la nouvelle distance optimale du sommet destination
        Tableau_dist_sommets[indice_dest] = (Tableau_dist_sommets[indice_sommet] + arc -> poids);
        g -> couleur = Tableau_dist_sommets[indice_sommet] + arc -> poids;
      }

      //On passe à l'arc suivant
      arc = arc -> arc_suivant;

    }
    //On enlève de la pile le sommet qu'on a fini d'étudier
    //Il ne sera donc plus pris en compte lors de l'appel à plus_petite_distance
    Tableau_sommets [indice_sommet] = NULL;
  }
    free(Tableau_sommets);
    free(Tableau_dist_sommets);
}

void Print_Dijkstra (pgraphe_t g, int r){
  /*
    debuggé avec : considéré juste et sans bug, a le comportement attendu
  */
  if (g== NULL)
   return;

  algo_dijkstra (g,r);
  
  for( pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant)
    printf("%d(distance au point de départ = %d)  ",g_courant->label,g_courant->couleur);
  printf("\n");

}

// QUESTION 9
int graphe_eulerien_rec(pgraphe_t g,psommet_t actuel,pchemin_t c);
int graphe_eulerien(pgraphe_t g) {
  /*
    debuggé avec : tout les graphes
  */
  if(g == NULL)
    return 0;
  // init_couleur_sommet(g,0);
  for(pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant) {
    chemin_t c; c.depart = g_courant;c.liste_arcs = NULL;
    if(graphe_eulerien_rec(g,g_courant,&c))
      return 1; // et c contient le chemin hamiltonien trouvé
  }
  return 0;
}

// QUESTION 10
int graphe_hamiltonien_rec(pgraphe_t g, psommet_t actuel, pchemin_t c);
int graphe_hamiltonien(pgraphe_t g) {
  /*
    /!\ modifie tmp ET couleur /!\
    debuggé avec : tout les graphes
  */
  if(g == NULL)
    return 0;
  init_couleur_sommet(g,0);
  for(pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant) {
    chemin_t c; c.depart = g_courant;c.liste_arcs = NULL;
    if(graphe_hamiltonien_rec(g,g_courant,&c))
      return 1; // et c contient le chemin hamiltonien trouvé
  }
  return 0;
}

// QUESTION 11
int distance(pgraphe_t g, int x, int y) {
  /*
    /!\ modifie tmp /!\
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6
  */
  if(g == NULL)
    return 0;
  pgraphe_t sommet_x = chercher_sommet(g,x);
  pgraphe_t sommet_y = chercher_sommet(g,y);
  if(sommet_x == NULL || sommet_y == NULL)
    return 0;
  distance_min(g,x);
  return sommet_y->tmp;
}

// QUESTION 12
int excentricite(pgraphe_t g, int n) {
  /*
    /!\ modifie tmp /!\
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6
  */
	if(g == NULL)
    return 0;
	distance_min(g,n); //distance_max(g,n);
	int max = 0;
	for(pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant)
		if(g_courant->tmp != INT_MAX && g_courant->tmp > max)
			max = g_courant->tmp;
	return max;
}

// QUESTION 13
int diametre(pgraphe_t g) {
  /*
    /!\ modifie tmp /!\
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6
  */
	if(g == NULL)
    return 0;
	int max = 0,tmp;
	for(pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant) {
		tmp = excentricite(g,g_courant->label);
		if(tmp > max)
			max = tmp;
	}
	return max;
}





// <------------------------------------------------>
// SOUS FONCTION SUR GRAPHE :
// <------------------------------------------------>

int nombre_arc (psommet_t g) {
  /*
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6
  */
  if(g == NULL)
    return 0;
  int nb_arcs = 0 ;
  for(parc_t l = g->liste_arcs ; l != NULL; l = l->arc_suivant)
      nb_arcs = nb_arcs + 1 ;
  return nb_arcs ;
}

void init_champ_tmp_sommet (pgraphe_t g, int x) {
  /*
    /!\ modifie tmp /!\
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6
  */
  for(; g != NULL ; g = g->sommet_suivant)
    g->tmp = x;
  return ;
}

int degre_sortant_sommet (pgraphe_t g, psommet_t s) {
  /*
    Cette fonction retourne le nombre d'arcs sortants 
    du sommet n dans le graphe g
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6
  */
  if(g == NULL || s == NULL)
    return 0;
  int nb_arcs = 0 ;

  for(parc_t arc_courant = s->liste_arcs ; arc_courant != NULL ; arc_courant = arc_courant->arc_suivant)
    nb_arcs++;

  return nb_arcs ;
}

int degre_entrant_sommet (pgraphe_t g, psommet_t s) {
  /*
    Cette fonction retourne le nombre d'arcs entrants 
    dans le noeud n dans le graphe g
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6
  */
  if(g == NULL || s == NULL)
    return 0;
  int nb_arcs = 0 ;

  for (pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant)
    for(parc_t arc_courant = g_courant->liste_arcs ; arc_courant != NULL ; arc_courant = arc_courant->arc_suivant )
      if(arc_courant->dest == s)
        nb_arcs++;

  return nb_arcs ;
}

int degre_maximal_graphe (pgraphe_t g) {
  /*
    Max des degres des sommets du graphe g
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6
  */
 	if(g == NULL)
    return 0;
	int max = 0,tmp;

	for (pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant) {
		tmp = degre_sommet(g,g_courant);
		if(tmp > max)
			max = tmp;
	}

  return max;
}

int degre_minimal_graphe (pgraphe_t g) {
  /*
    Min des degres des sommets du graphe g
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6
  */
	if(g == NULL)
		return 0;
	int min = INT_MAX,tmp;

	for (pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant) {
		tmp = degre_sommet(g,g_courant);
		if(tmp < min)
			min = tmp;
	}

  return min;
}

int independant (pgraphe_t g) { // Ne marche pas
  /* 
    Les aretes du graphe n'ont pas de sommet en commun
    debuggé avec :
    erreur : gr_custom3 
  */
	if(g == NULL)
		return 0;
	int d = degre_maximal_graphe(g);
  return d == 0 || d == 1 ;
}

int complet (pgraphe_t g) { 
  /*
    Toutes les paires de sommet du graphe sont jointes par un arc 
    algo bourrin dégeulasse
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6, gr_custom1
  */
  
	if(g == NULL)
		return 0;
	int nb_sommets = nombre_sommets(g);
  // Tentative 1 :
	// for(pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant)
	// 	if((degre_entrant_sommet(g,g_courant) + degre_sortant_sommet(g,g_courant)) != (nb_sommets-1)*2)
	// 		return 0;
  // return 1;
  // Tentative 2 :
  return (degre_minimal_graphe(g) >= ((nb_sommets-1)*2));
  
}

int regulier (pgraphe_t g) {
  /*
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6, gr_custom1
  */
  /* 
     graphe regulier: tous les sommets ont le meme degre
     g est le ponteur vers le premier sommet du graphe
     renvoie 1 si le graphe est régulier, 0 sinon
  */
  return (g != NULL && degre_minimal_graphe(g) == degre_maximal_graphe(g));
}

void distance_min_rec(pgraphe_t g) {
  /*
    /!\ modifie tmp /!\
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6
  */
  if(g != NULL) {
    int dist;
    for(parc_t arc_courant = g->liste_arcs; arc_courant != NULL; arc_courant = arc_courant->arc_suivant) {
      dist = g->tmp + arc_courant->poids;
      if(arc_courant->dest->tmp > dist) {
				arc_courant->dest->tmp = dist;
				distance_min_rec(arc_courant->dest);
      }
    }
  }
}
void distance_min(pgraphe_t g, int r) {
  /*
    /!\ modifie tmp /!\
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr5, gr6
  */
  if(g == NULL)
    return;
  init_champ_tmp_sommet(g,INT_MAX);
  pgraphe_t gr = chercher_sommet(g,r);
  if(gr == NULL)
    return;
  gr->tmp = 0;
  distance_min_rec(gr);
}

int graphe_hamiltonien_rec(pgraphe_t g, psommet_t actuel, pchemin_t c) {
  /*
    /!\ modifie tmp ET couleur /!\
    debuggé avec : tout les graphes
  */
  if(g == NULL || actuel == NULL)
    return 0;
  actuel->couleur = 1;
  for(parc_t arc_courant = actuel->liste_arcs ; arc_courant != NULL ; arc_courant = arc_courant->arc_suivant) {
    if(arc_courant->dest->couleur == 0) {
      chemin_arc_push(c,*arc_courant);
      if(graphe_hamiltonien_rec(g,arc_courant->dest,c))
        return 1;
      chemin_arc_pop(c);
    }
  }
  if(hamiltonien(g,*c)) {
    return 1;
  }
  actuel->couleur = 0;
  return 0;
}

int graphe_eulerien_rec(pgraphe_t g,psommet_t actuel,pchemin_t c) {
  /*
    debuggé avec : tout les graphes
  */
  if(g == NULL || actuel == NULL)
    return 0;
  for(parc_t arc_courant = actuel->liste_arcs ; arc_courant != NULL ; arc_courant = arc_courant->arc_suivant) {
    chemin_arc_push(c,*arc_courant);
    if(simple(g,*c)) {
      if(graphe_eulerien_rec(g,arc_courant->dest,c))
        return 1;
    }
    chemin_arc_pop(c);
  }
  if(eulerien(g,*c)) {
    return 1;
  }
  return 0;
}

void afficher_graphe_sommet(pgraphe_t g) {
  /*
    debuggé avec : considéré juste et sans bug, a le comportement attendu
  */
  if (g== NULL)
   return;
  for( pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant)
    printf("%d(c=%d,d=%d)\n",g_courant->label,g_courant->couleur,g_courant->tmp);
  // printf("\n");
}



// <------------------------------------------------>
// FONCTION SUR GRAPHE BONUS
// <------------------------------------------------>

void distance_max_rec(pgraphe_t g, psommet_t actuel, chemin_t c) {
  /*
    /!\ modifie tmp /!\
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr6
    erreur dans gr5 (4->2 : d=1 alors que en 2 possible)
  */
  if(actuel != NULL && g != NULL) {
    int dist;
    for(parc_t arc_courant = actuel->liste_arcs; arc_courant != NULL; arc_courant = arc_courant->arc_suivant) {
      dist = actuel->tmp + arc_courant->poids;
      if(arc_courant->dest->tmp < dist) {
				chemin_arc_push(&c,*arc_courant);
				if(elementaire(g,c)) {
					arc_courant->dest->tmp = dist;
					distance_max_rec(g,arc_courant->dest,c);
				}
				chemin_arc_pop(&c);
      }
    }
  }
}
void distance_max(pgraphe_t g, int r) {
  /*
    pire distance
    /!\ modifie tmp /!\
    debuggé avec : gr0, gr1, gr2, gr3, gr4, gr6 -> a le comportement attendu de manière général
    erreur dans gr5 (4->2 : d=1 alors que en 2 possible)
  */
  if(g == NULL)
    return;
  init_champ_tmp_sommet(g,0);
  pgraphe_t gr = chercher_sommet(g,r);
  if(gr == NULL)
    return;
	chemin_t c; c.depart = gr;c .liste_arcs = NULL;
  gr->tmp = 0;
  distance_max_rec(g,gr,c);
}







// |||||||||||||||||||||||||||||||||||||||||||||||||||
// FONCTION SUR CHEMIN :
// |||||||||||||||||||||||||||||||||||||||||||||||||||

// <------------------------------------------------>
// FONCTION SUR CHEMIN A RENDRE
// <------------------------------------------------>

// QUESTION 5
int elementaire (pgraphe_t g, chemin_t c) {
  /*
    debuggé avec : + de 30 chemins, a le comportement attendu
  */
  if(g == NULL)
    return 0;
  pgraphe_t g_courant = c.depart;
  for(parc_t arc_courant = c.liste_arcs ; arc_courant != NULL ;  g_courant = arc_courant->dest, arc_courant = arc_courant->arc_suivant )
    for(parc_t sub_arc_courant = arc_courant ; sub_arc_courant != NULL ; sub_arc_courant = sub_arc_courant->arc_suivant)
      if(sub_arc_courant->dest == g_courant)
        return 0;
  return 1;
}

// QUESTION 6
int simple(pgraphe_t g, chemin_t c) { 
  /*
    debuggé avec : + de 30 chemins, a le comportement attendu
  */
  if(g == NULL)
    return 0;
  psommet_t g_depart = c.depart;
  for(parc_t arc_courant = c.liste_arcs; arc_courant != NULL ; g_depart = arc_courant->dest , arc_courant = arc_courant->arc_suivant){
    psommet_t g_depart_sub = arc_courant->dest;
    for(parc_t sub_arc_courant = arc_courant->arc_suivant ; sub_arc_courant != NULL ; g_depart_sub = sub_arc_courant->dest, sub_arc_courant = sub_arc_courant->arc_suivant)
      if(g_depart == g_depart_sub && arc_courant->dest == sub_arc_courant->dest && arc_courant->poids == sub_arc_courant->poids)
        return 0;
	}
  return 1;
}

// QUESTION 7
int eulerien(pgraphe_t g, chemin_t c) {
  /*
    debuggé avec : gr2 (plien de chemins), a le comportement attendu
  */
  if(g == NULL)
    return 0;
  for(pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant) {
    for(parc_t arc_sommet_courant = g_courant->liste_arcs ; arc_sommet_courant != NULL ; arc_sommet_courant = arc_sommet_courant->arc_suivant) {

			pgraphe_t g_depart = c.depart;
      int nb_occ = 0;
      for(parc_t arc_chemin_courant = c.liste_arcs; arc_chemin_courant != NULL ; arc_chemin_courant = arc_chemin_courant->arc_suivant) {
        if(g_courant == g_depart && arc_sommet_courant->dest == arc_chemin_courant->dest)
          nb_occ++;
				g_depart = arc_chemin_courant->dest;
			}
      if(nb_occ != 1)
        return 0;
    }
  }
  return 1;
}

// QUESTION 8
int hamiltonien(pgraphe_t g, chemin_t c) {
  /*
    /!\ modifie tmp /!\
    debuggé avec : gr_shed2 (plein de chemins), a le comportement attendu
  */
  if(g == NULL)
    return 0;
  // version destructrice de tmp :
  init_champ_tmp_sommet(g,0);
  if(c.depart != NULL)
    (c.depart->tmp)++;
  for(parc_t arc_chemin_courant = c.liste_arcs; arc_chemin_courant != NULL ; arc_chemin_courant = arc_chemin_courant->arc_suivant)
    (arc_chemin_courant->dest->tmp)++;
  for(pgraphe_t g_courant = g ; g_courant != NULL ; g_courant = g_courant->sommet_suivant)
    if(g_courant->tmp != 1)
      return 0;
  return 1;

  // version non destructrice :
  // int nb = 0;
  // if(c.depart != NULL)
  //   nb++;
  // for(parc_t arc_chemin_courant = c.liste_arcs; arc_chemin_courant != NULL ; arc_chemin_courant = arc_chemin_courant->arc_suivant)
  //   nb++;
  // return elementaire(g,c) && nombre_sommets(g) == nb;
}




// <------------------------------------------------>
// SOUS-FONCTION SUR CHEMIN
// <------------------------------------------------>

int longueur(pgraphe_t g, chemin_t c) {
  /*
    debuggé avec : gr4(20 chemins), a le comportement attendu
  */
  if(g == NULL)
    return 0;
  int somme = 0;
  for(parc_t arc_courant = c.liste_arcs ; arc_courant != NULL ; arc_courant = arc_courant->arc_suivant)
    somme += arc_courant->poids;
  return somme;
}

void chemin_arc_push(pchemin_t c, arc_t arc) { 
  /*
    (= fonction apprend)
    debuggé avec : considéré juste et sans bug, a le comportement attendu
  */
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
		arc_courant->arc_suivant = new_arc;
	}
}

void chemin_arc_pop(pchemin_t c) {
  /*
    (= fonction prune)
    debuggé avec : considéré juste et sans bug, a le comportement attendu
  */
	if(c == NULL || c->liste_arcs == NULL )
		return;
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

void print_chemin(chemin_t c) {
  /*
    debuggé avec : considéré juste et sans bug, a le comportement attendu
  */
  if(c.depart != NULL) {
    printf("%d(c=%d,d=%d)", c.depart->label,c.depart->couleur,c.depart->tmp);
    for(parc_t arc_courant = c.liste_arcs ; arc_courant != NULL ; arc_courant = arc_courant->arc_suivant)
      printf("  --%d->  %d(c=%d,d=%d)",arc_courant->poids,arc_courant->dest->label,arc_courant->dest->couleur,arc_courant->dest->tmp);
  }
  printf("\n");
}

void free_chemin(chemin_t c){
  /*
    debuggé avec : a le comportement attendu
  */
  while (c.liste_arcs != NULL){
    chemin_arc_pop(&c);
  }
}

// int nb_arcs(chemin_t c) {
//   int nb_arcs = 0 ;
//   for(parc_t arc_courant = c.liste_arcs, nb_arcs = 0; arc_courant != NULL ; nb_arcs++)
//     arc_courant = arc_courant->arc_suivant ;
//   return nb_arcs ;
// }