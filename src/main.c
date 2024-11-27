// Inclusions
#include "main.h"


int main(int argc, char *argv[]) {
  min_argc(argc);
  char *pseudo_utilisateur = argv[1]; // Récupération des pseudos
  char *pseudo_destinataire = argv[2];
  struct sigaction sa;
  initialiser_signal_action(&sa, signal_management);
  initialiser_et_verifier(argc, argv, pseudo_utilisateur, pseudo_destinataire, fifo_sender, fifo_receiver, &bot_mode, &manuel_mode);
  size_t memory_size = manuel_mode ? MAX_MEMORY_SIZE : 1;
  sharedMemo *buffer = shared_memory_initializer(memory_size);
  configurer_signaux(&sa);
  pid_t fork_return = fork();
  handle_fork_error(fork_return);

  if (fork_return > 0) {
    int fd_fifo_sender = open(fifo_sender, O_WRONLY);
    handle_parent_process(fd_fifo_sender, buffer, pseudo_utilisateur, pseudo_destinataire);
  }

  else if (fork_return == 0) {
    handle_child_process(buffer, pseudo_destinataire);
  }

  while (buffer->offset > 0) {
    printf("[\x1B[4m%s\x1B[0m] %s", pseudo_destinataire, getString(buffer));
    fflush(stdout);
  }
  cleanning_fifos(fifo_sender, fifo_receiver);
  clean_shared_memo(buffer);
  return 0;
  }
