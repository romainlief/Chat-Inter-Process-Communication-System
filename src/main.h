#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>

#define MAX_PSEUDO_LEN 30
#define Max_LEN_FIFO   72

int verifier_erreurs(int argc, char* pseudo_utilisateur, char* pseudo_destinataire);

void create_pipe(const char* pipe_path);

void signal_management(int signa);

void concatener_pipes(char* fifo_path, const char* pseudo1, const char* pseudo2);

#endif 
