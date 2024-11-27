#ifndef INIT_H
#define INIT_H

// Inclusions
#include "arg_verif.h"
#include "pipe_manager.h"
#include "memory.h"
#include "sig.h"

#include <fcntl.h>


/**
 * @brief lance toutes les fonctions de vérification des arguments
 * 
 * @param argc Nombre d'arguments
 * @param argv Tableau des arguments
 * @param pseudo_utilisateur le pseudo de l'utilisateur
 * @param pseudo_destinataire le pseudo du destinataire
 * @param fifo_sender pointeur vers le fifo sender
 * @param fifo_receiver pointeur vers le fifo receiver
 * @param bot_mode pointeur vers le mode bot
 * @param manuel_mode pointeur vers le mode manuel
 */

void initialiser_et_verifier(int argc, char *argv[], const char *pseudo_utilisateur, const char *pseudo_destinataire, char *fifo_sender, char *fifo_receiver, int *bot_mode, int *manuel_mode);

/**
 * @brief Fonction qui gère le processus parent
 * 
 * @param fd_fifo_sender le file descriptor du fifo sender
 * @param buffer le buffer partagé
 * @param pseudo_utilisateur le pseudo de l'utilisateur
 * @param pseudo_destinataire le pseudo du destinataire
 */
void handle_parent_process(int fd_fifo_sender, sharedMemo *buffer, char *pseudo_utilisateur, char *pseudo_destinataire);

/**
 * @brief Fonction qui gère le processus enfant
 * 
 * @param buffer le buffer partagé
 * @param pseudo_destinataire le pseudo du destinataire
 */
void handle_child_process(sharedMemo *buffer, char *pseudo_destinataire);

#endif // INIT_H
