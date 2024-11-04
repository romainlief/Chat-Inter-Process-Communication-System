#ifndef MAIN_H
#define MAIN_H

// Inclusions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Constantes
#define MAX_PSEUDO_LEN 30 // Longueur max des pseudos dans les consignes
#define MAX_LEN_FIFO   72 // Taille fifo max=72 (5 + 30 + 1 + 30 + 5 + 1)
#define BUFFER_SIZE 256 // Taille du buffer
#define BASE_FIFO_PATH "/tmp/" // Chemin de base des pipes
#define END_FIFO_PATH ".chat" // Fin du chemin des pipes
#define PARAM_BOT "--bot" // Paramètre optionnel bot
#define PARAM_MANUEL "--manuel" // Paramètre optionnel manuel
// Déclarations des fonctions

/**
 * Vérifie les erreurs possibles lors de l'exécution du programme.
 * @param argc Nombre d'arguments
 * @param pseudo_utilisateur Pseudo de l'utilisateur
 * @param pseudo_destinataire Pseudo du destinataire
 * @return 0 si aucune erreur, sinon le code d'erreur.
 */
int verifier_erreurs(int argc, char* pseudo_utilisateur, char* pseudo_destinataire);

/**
 * Crée un pipe nommé.
 * @param pipe_path Chemin du pipe
 */
void create_pipe(const char* pipe_path);

/**
 * Gère les signaux SIGINT et SIGPIPE.
 * @param signa Signal reçu
 */
void signal_management(int signa);

/**
 * Concatène les pseudos pour créer le chemin du pipe.
 * @param fifo_path Chemin du pipe
 * @param pseudo1 Pseudo de l'utilisateur
 * @param pseudo2 Pseudo du destinataire
 */
void concatener_pipes(char* fifo_path, const char* pseudo1, const char* pseudo2);

/**
 * Vérifie si les paramètres optionnels --bot et --manuel sont présents et si d'autres paramètres sont inconnus renvoie une erreur.
 * @param argc Nombre d'arguments
 * @param argv Tableau des arguments
 * @param bot_mode Pointeur vers la variable bot_mode
 * @param manuel_mode Pointeur vers la variable manuel_mode
 */
void verification_param_optinnel(int argc, char* argv[], int* bot_mode, int* manuel_mode);

#endif // MAIN_H
