// Inclusions
#include "main.h"


int main(int argc, char *argv[]) {
  min_argc(argc);
  char *pseudo_utilisateur = argv[1]; // Récupération des pseudos
  char *pseudo_destinataire = argv[2];
  struct sigaction sa; // Initialisation struct sigaction
  initialiser_signal_action(&sa, signal_management); // Initialisation des signaux
  initialiser_et_verifier(argc, argv, pseudo_utilisateur, pseudo_destinataire, fifo_sender, fifo_receiver, &bot_mode, &manuel_mode); // Initialisation et vérification des paramètres
  size_t memory_size = manuel_mode ? MAX_MEMORY_SIZE : 1; // Initialisation de la taille de la mémoire partagée
  sharedMemo *buffer = shared_memory_initializer(memory_size); // Initialisation de la mémoire partagée
  configurer_signaux(&sa); // Configuration des signaux
  pid_t fork_return = fork(); // Création du processus fils
  handle_fork_error(fork_return); // Gestion des erreurs de fork

  if (fork_return > 0) {
    int fd_fifo_sender = open(fifo_sender, O_WRONLY);
    handle_parent_process(fd_fifo_sender, buffer, pseudo_utilisateur, pseudo_destinataire); // Gestion du processus parent
  }
  else if (fork_return == 0) {
    handle_child_process(buffer, pseudo_destinataire); // Gestion du processus fils
  }

  while (buffer->offset > 0) { // Affichage des messages restants dans le buffer
    printf("[\x1B[4m%s\x1B[0m] %s", pseudo_destinataire, getString(buffer));
    fflush(stdout);
  }
  cleanning_fifos(fifo_sender, fifo_receiver); // Nettoyage des fifos
  clean_shared_memo(buffer); // Nettoyage de la mémoire partagée
  return 0;
  }
