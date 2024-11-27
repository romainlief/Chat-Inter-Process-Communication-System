#ifndef INIT_H
#define INIT_H

// Inclusions
#include "arg_verif.h"
#include "pipe_manager.h"

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

#endif // INIT_H
