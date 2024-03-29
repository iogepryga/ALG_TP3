#include "fap.h"
#include <stdlib.h>
#include <unistd.h>
#include "graphe.h"

fap creer_fap_vide() { return NULL; }

fap inserer(fap f, pgraphe_t element, float priorite) {
    fap nouveau, courant, precedent;

    /* nouveau maillon */
    nouveau = (fap)malloc(sizeof(struct maillon));
    nouveau->element = element;
    nouveau->priorite = priorite;

    /* insertion en tete */
    if ((f == NULL) || (priorite < f->priorite)) {
        nouveau->prochain = f;
        f = nouveau;
    }

    /* recherche de la bonne position et insertion */
    else {
        precedent = f;
        courant = f->prochain;
        while ((courant != NULL) && (priorite >= courant->priorite)) {
            precedent = courant;
            courant = courant->prochain;
        }
        precedent->prochain = nouveau;
        nouveau->prochain = courant;
    }
    return f;
}

fap extraire(fap f, pgraphe_t* element, float *priorite) {
    fap courant;

    /* extraire le premier element si la fap n'est pas vide */
    if (f != NULL) {
        courant = f;
        *element = courant->element;
        *priorite = courant->priorite;
        f = courant->prochain;
        free(courant);
    }
    return f;
}

// fap extraire_label(fap f, pgraphe_t* element, float *priorite, int label) {
//     if(est_fap_vide(f))
//         return f;
//     if(f->element->label == label)
//         f = extraire(f,element,priorite);
//     fap courant = f->prochain;

//     while(courant->prochain != NULL && courant->prochain->element->label != label) {
//         courant = courant->prochain;
//     }
//     if(courant->prochain != NULL && courant->prochain->element->label == label) {
//         *element = courant->element;
//         *priorite = courant->priorite;
//         courant->prochain = courant->prochain->prochain;
//     }
//     return f;
// }

int est_fap_vide(fap f) { return f == NULL; }

void detruire_fap(fap f) {
    pgraphe_t element;
    float priorite;

    while (!est_fap_vide(f))
        f = extraire(f, &element, &priorite);
}
