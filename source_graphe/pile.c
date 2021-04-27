#include <stdlib.h>
#include "pile.h"

#define TRUE 1
#define FALSE 0

ppile_t creer_pile() {
	ppile_t p = (ppile_t) malloc(sizeof(pile_t));

	p->sommet = 0;

	return p;
}

int detruire_pile(ppile_t p) {
	if (p == NULL)
		return FALSE;

	free(p);
	return TRUE;
}	

int pile_vide(ppile_t p) {
	if (p == NULL)
		return FALSE; // Erreur

	return p->sommet == 0 ? TRUE : FALSE;
}

int pile_pleine(ppile_t p) {
	if (p == NULL)
		return FALSE; // Erreur

	return p->sommet == MAX_PILE_SIZE ? TRUE : FALSE;
} 

void* depiler(ppile_t p) {
	if (p == NULL || p->sommet == 0)
		return NULL;

	return p->Tab[--(p->sommet)];
}

int empiler(ppile_t p, void* pn) {
	if (p == NULL || p->sommet == MAX_PILE_SIZE)
		return FALSE;

	p->Tab[(p->sommet)++] = pn;

	return TRUE;
}
