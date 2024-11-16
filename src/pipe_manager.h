#ifndef PIPE_MANAGER_H
#define PIPE_MANAGER_H

// Inclusions
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Constantes
#define BASE_FIFO_PATH "/tmp/" // Chemin de base des pipes
#define END_FIFO_PATH ".chat"  // Fin du chemin des pipes

/**
 * Concatène les pseudos pour créer le chemin du pipe.
 * @param fifo_path Chemin du pipe
 * @param pseudo1 Pseudo de l'utilisateur
 * @param pseudo2 Pseudo du destinataire
 */
void concatener_pipes(char *fifo_path, const char *pseudo1,
                      const char *pseudo2);

/**
 * Initialise les pipes.
 * @param fifo_sender Chemin du pipe sender
 * @param fifo_receiver Chemin du pipe receiver
 */
void initialiser_pipes(char *fifo_sender, char *fifo_receiver);

#endif // PIPE_MANAGER_H
