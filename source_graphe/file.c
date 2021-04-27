#include <stdlib.h>
#include "file.h"

#define TRUE 1
#define FALSE 0

pfile_t creer_file() {
	pfile_t f = (pfile_t) malloc(sizeof(file_t));

	f->tete = -1;
	f->queue = 0;

	return f;
}

int detruire_file(pfile_t f) {
	if (f == NULL)
		return FALSE;

	free(f);
	return TRUE;
}

int file_vide(pfile_t f) {
	if (f == NULL)
		return FALSE;

	return f->tete == -1 ? TRUE : FALSE;
}

int file_pleine(pfile_t f) {
	if (f == NULL)
		return FALSE;

	return f->tete == (f->queue + 1) % MAX_FILE_SIZE ? TRUE : FALSE;
}

void* defiler(pfile_t f) {
	if (f == NULL || file_vide(f))
		return NULL;

	void* res = f->Tab[f->tete];

	if (f->tete == f->queue) // On dépile le dernier élément
		f->tete = -1;
	else
		f->tete = (f->tete + 1) % MAX_FILE_SIZE;

	return res;
}

int enfiler(pfile_t f, void* p) {
	if (f == NULL || file_pleine(f))
		return FALSE;

	if (file_vide(f)) // On enfile le premier élément
		f->tete = f->queue;
	else
		f->queue = (f->queue + 1) % MAX_FILE_SIZE;

	f->Tab[f->queue] = p;

	return TRUE;
}
