#include "fork_verif.h"

void handle_fork_error(pid_t fork_return) {
  if (fork_return < 0) {
    perror("fork()");
    exit(6);
  }
}


