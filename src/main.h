#ifndef MAIN_H
#define MAIN_H

// Inclusions
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "arg_verif.h"
#include "memory.h"
#include "pipe_manager.h"
#include <stdbool.h>

// Constantes
#define MAX_LEN_FIFO 72 // Taille fifo max=72 (5 + 30 + 1 + 30 + 5 + 1)
#define BUFFER_SIZE 256 // Taille du buffer

// Structures
struct sigaction sa; // Structure pour la gestion des signaux

// Variables globales
extern char fifo_sender[MAX_LEN_FIFO];   // Chemin du pipe sender
extern char fifo_receiver[MAX_LEN_FIFO]; // Chemin du pipe receiver

// Déclaration des fonctions
/**
 * Gère les signaux SIGINT et SIGPIPE.
 * @param signa Signal reçu
 */
void signal_management(int signa);

#endif // MAIN_H
