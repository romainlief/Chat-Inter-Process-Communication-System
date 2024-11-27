#ifndef SIG_H
#define SIG_H

// Inclusions
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>


// Variables globales
extern int manuel_mode;
extern bool vider;


// Constantes
#define MAX_LEN_FIFO 72 // Taille fifo max=72 (5 + 30 + 1 + 30 + 5 + 1)
#define BUFFER_SIZE 256 // Taille du buffer


// Variables globales
extern char fifo_sender[MAX_LEN_FIFO];   // Chemin du pipe sender
extern char fifo_receiver[MAX_LEN_FIFO]; // Chemin du pipe receiver

/**
 * @brief Gère les signaux SIGINT et SIGPIPE.
 * 
 * @param signa Signal reçu
 */
void signal_management(int signa);

/**
 * @brief Initialise la structure sigaction.
 * 
 * @param sa la structure sigaction
 * @param handler le handler du signal
 */
void initialiser_signal_action(struct sigaction *sa, void (*handler)(int));

/**
 * @brief Configure les signaux SIGINT et SIGPIPE.
 * 
 * @param sa la structure sigaction
 */
void configurer_signaux(struct sigaction *sa);

/**
 * @brief unilink les deux fifos.
 * 
 * @param fifo_sender le fifo sender
 * @param fifo_receiver le fifo receiver
 */
void cleanning_fifos(const char *fifo_sender, const char *fifo_receiver);

#endif // SIG_H
