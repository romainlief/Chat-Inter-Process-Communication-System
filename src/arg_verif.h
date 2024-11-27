#ifndef ARG_VERIF_H
#define ARG_VERIF_H

// Inclusions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constantes
#define MAX_PSEUDO_LEN 30       // Longueur max des pseudos dans les consignes
#define PARAM_BOT "--bot"       // Paramètre optionnel bot
#define PARAM_MANUEL "--manuel" // Paramètre optionnel manuel

// Variables globales
extern int bot_mode;    // 0 = normal, 1 = mode bot
extern int manuel_mode; // 0 = normal, 1 = mode manuel

// Déclaration des fonctions
/**
 * @brief Vérifie si le nombre d'arguments est suffisant.
 *
 * @param argc Nombre d'arguments
 * @return int 0 si le nombre d'arguments est suffisant, sinon 1.
 */
int min_argc(int argc);

/**
 * Vérifie les erreurs possibles lors de l'exécution du programme.
 * @param argc Nombre d'arguments
 * @param pseudo_utilisateur Pseudo de l'utilisateur
 * @param pseudo_destinataire Pseudo du destinataire
 * @return 0 si aucune erreur, sinon le code d'erreur.
 */
int verifier_erreurs(int argc, const char *pseudo_utilisateur, const char *pseudo_destinataire);

/**
 * Vérifie si les paramètres optionnels --bot et --manuel sont présents et si
 * d'autres paramètres sont inconnus renvoie une erreur.
 * @param argc Nombre d'arguments
 * @param argv Tableau des arguments
 * @param bot_mode Pointeur vers la variable bot_mode
 * @param manuel_mode Pointeur vers la variable manuel_mode
 */
void verification_param_optinnel(int argc, char *argv[], int *bot_mode,
                                 int *manuel_mode);


#endif // ARG_VERIF_H
