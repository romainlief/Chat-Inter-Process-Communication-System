#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>

#define MAX_PSEUDO_LEN 30

int verifier_erreurs(int argc, char* pseudo_utilisateur, char* pseudo_destinataire);

void create_pipe(const char* pipe_path);

void signal_management(int signa);

#endif 
