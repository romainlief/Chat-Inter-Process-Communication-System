// Inclusions
#include "main.h"
#include "arg_verif.h"
#include "memory.h"
#include "pipe_manager.h"


// Variables globales
char fifo_sender[MAX_LEN_FIFO];
char fifo_receiver[MAX_LEN_FIFO];


// Fonctions
void signal_management(int signa) {
  if (signa == SIGINT) {
    fclose(stdin);
  } else if (signa == SIGPIPE) { // Ce cas n'arrive jamais, mais au cas ou nous le laissons
    fclose(stdin);
  }
}


int main(int argc, char *argv[]) {
  if (min_argc(argc)) {
    return 1;
  }
  char *pseudo_utilisateur = argv[1]; // Récupération des pseudos
  char *pseudo_destinataire = argv[2];

  sa.sa_handler = signal_management;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  // Vérification des erreurs
  int erreur = verifier_erreurs(argc, pseudo_utilisateur, pseudo_destinataire);
  if (erreur != 0) {
    return erreur;
  }

  // Gestion des options --bot et --manuel
  verification_param_optinnel(argc, argv, &bot_mode, &manuel_mode);

  // Création des deux path pipes en concaténant les pseudos
  concatener_pipes(fifo_sender, pseudo_utilisateur, pseudo_destinataire);
  concatener_pipes(fifo_receiver, pseudo_destinataire, pseudo_utilisateur);

  initialiser_pipes(fifo_sender, fifo_receiver);

  char temp[BUFFER_SIZE];
  size_t memory_size = 1;
  if (manuel_mode) {
    memory_size = MAX_MEMORY_SIZE;
  } 
  sharedMemo *buffer = shared_memory_initializer(memory_size);

  pid_t fork_return = fork();

  if (fork_return > 0) {
    sigaction(SIGINT, &sa, NULL);

    int fd_fifo_sender = open(fifo_sender, O_WRONLY);
    int fd_fifo_receiver = open(fifo_receiver, O_WRONLY);

    while (fgets(temp, sizeof(temp), stdin) != NULL) {

      if (!bot_mode) {
        printf("[\x1B[4m%s\x1B[0m] %s", pseudo_utilisateur, temp);
        fflush(stdout);
      }

      ssize_t ecriture = write(fd_fifo_sender, temp, sizeof(temp));
      if (ecriture == -1) {
        perror("Erreur d'écriture");
        break;
      }

      if (manuel_mode) {
        while (buffer->offset > 0) {
          printf("[\x1B[4m%s\x1B[0m] %s", pseudo_destinataire,
                 getString(buffer));
          fflush(stdout);
        }
      }
    }

    write(fd_fifo_sender, "\0", sizeof("\0"));

    close(fd_fifo_sender);
    close(fd_fifo_receiver);

  } else {
    sigaction(SIGPIPE, &sa, NULL);
    int fd_fifo_sender = open(fifo_sender, O_RDONLY);
    int fd_fifo_receiver = open(fifo_receiver, O_RDONLY);

    while (read(fd_fifo_receiver, temp, sizeof(temp)) > 0 &&
           !(strcmp(temp, "\0") == 0)) {
      if (!manuel_mode) {
        if (bot_mode) {
          printf("[%s]: %s", pseudo_destinataire, temp);
        } else {
          printf("[\x1B[4m%s\x1B[0m]: %s", pseudo_destinataire, temp);
        }
        fflush(stdout);
      }

      else {
        int max = write_shared(buffer, temp);
        printf("\a");

        if (max == 1) {
          while (buffer->offset > 0) {
            printf("[\x1B[4m%s\x1B[0m] %s", pseudo_destinataire,
                   getString(buffer));
          }
          write_shared(buffer, temp);
        }

        fflush(stdout); // Permet d'émettre son directement
      }
      fd_fifo_sender = open(fifo_sender, O_RDONLY);
      fd_fifo_receiver = open(fifo_receiver, O_RDONLY);
    }

    close(fd_fifo_sender);
    close(fd_fifo_receiver);
  }

  while (buffer->offset > 0) {
    printf("[\x1B[4m%s\x1B[0m] %s", pseudo_destinataire, getString(buffer));
    fflush(stdout);
  }
  kill(fork_return, SIGINT);
  printf("Interuption\n");
  unlink(fifo_sender);
  unlink(fifo_receiver);

  return 0;
}
