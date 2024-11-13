#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMORY_SIZE 4096

// Structures
typedef struct{ // Structure pour la mémoire partagée
                // Permet une gestion avec indexage facile
    int offset;
    char data[4096];
} sharedMemo;

// Déclaration des fonctions
/**
 * Initialise la mémoire partagée.
 * @return Pointeur vers la mémoire partagée
 */ 
sharedMemo* shared_memory_initializer();

/**
 * Libère la mémoire partagée.
 * @param memo Pointeur vers la mémoire partagée
 */
void clean_shared_memo(sharedMemo* memo);

/**
 * Ecrit dans la mémoire partagée.
 * @param memo Pointeur vers la mémoire partagée
 * @param str Chaîne de caractères à écrire
 */
int write_shared(sharedMemo* memo, const char* str);

/**
 * Lit dans la mémoire partagée.
 * @param memo Pointeur vers la mémoire partagée
 */
void read_memo(sharedMemo* memo);

/**
 * Récupère la première chaîne de caractères donnée de la mémoire partagée.
 * @param memo Pointeur vers la mémoire partagée
 * @return Chaîne de caractères
 */
char* getString(sharedMemo* memo);