#ifndef FORK_VERIF_H
#define FORK_VERIF_H

// Inclusion
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void handle_fork_error(pid_t fork_return);

#endif // FORK_VERIF_H
