#ifndef MEMORY_H
#define MEMORY_H

// Inclusions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <stdbool.h>

// Constantes
#define MAX_MEMORY_SIZE 4096
#define INIT_MALLOC_SIZE 8
#define CHUNK 8 // Chunk de bytes lu d'un coup

// Structures
typedef struct { // Structure pour la mémoire partagée
                 // Permet une gestion avec indexage facile
  int offset;
  char data[4096];
} sharedMemo;

typedef struct { // Structure pour gérer l'allocation dynamique de mémoire
  size_t taille_logique; // taille 'actuelle'
  size_t taille_reelle;
  char* valeurs;
} liste_t;

// Déclaration des fonctions
/**
 * Initialise la mémoire partagée.
 * @return Pointeur vers la mémoire partagée
 */
sharedMemo *shared_memory_initializer(size_t memory_size);

/**
 * Libère la mémoire partagée.
 * @param memo Pointeur vers la mémoire partagée
 */
void clean_shared_memo(sharedMemo *memo);

/**
 * Ecrit dans la mémoire partagée.
 * @param memo Pointeur vers la mémoire partagée
 * @param str Chaîne de caractères à écrire
 */
int write_shared(sharedMemo *memo, const char *str);

/**
 * Lit dans la mémoire partagée.
 * @param memo Pointeur vers la mémoire partagée
 */
void read_memo(sharedMemo *memo);

/**
 * Récupère la première chaîne de caractères donnée de la mémoire partagée.
 * @param memo Pointeur vers la mémoire partagée
 * @return Chaîne de caractères
 */
char *getString(sharedMemo *memo);

/**
 * Double la taille de la mémoire dynamique.
 * @param liste Struct représentant la mémoire dynamique: ses valeurs, sa taille réelle et logique
 */
void etendre_liste(liste_t* liste);

/**
 * Récupère un input de taille variable.
 * @return Une instance de liste_t de taille variable en fonction des inputs
 */
liste_t getDynamicString();

#endif // MEMORY_H
