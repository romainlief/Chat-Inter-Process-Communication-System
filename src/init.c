#include "init.h"
#include "global.h"

void initialiser_et_verifier(int argc, char *argv[], const char *pseudo_utilisateur, const char *pseudo_destinataire, char *fifo_sender, char *fifo_receiver, int *bot_mode, int *manuel_mode) {
  verifier_erreurs(argc, pseudo_utilisateur, pseudo_destinataire);
  verification_param_optinnel(argc, argv, bot_mode, manuel_mode);
  concatener_pipes(fifo_sender, pseudo_utilisateur, pseudo_destinataire);
  concatener_pipes(fifo_receiver, pseudo_destinataire, pseudo_utilisateur);
  initialiser_pipes(fifo_sender, fifo_receiver);
}


void handle_parent_process(int fd_fifo_sender, sharedMemo *buffer, char *pseudo_utilisateur, char *pseudo_destinataire) {
  char *tempStr = NULL;
  size_t size = 0;
  if (!manuel_mode) {
    signal(SIGINT, SIG_IGN);
  }
  ssize_t code;
  while ((code = getline(&tempStr, &size, stdin))) {
    if (code == -1) {
      if (vider) {
        while (buffer->offset > 0) {
          printf("[\x1B[4m%s\x1B[0m] %s", pseudo_destinataire, getString(buffer));
          fflush(stdout);
        }
        vider = false;
        clearerr(stdin);
        continue;
      } else {
        break;
      }
    }
    char temp[size];
    memcpy(temp, tempStr, size);

    if (!bot_mode) {
      printf("[\x1B[4m%s\x1B[0m] %s", pseudo_utilisateur, temp);
      fflush(stdout);
    }
    ssize_t ecriture = write(fd_fifo_sender, temp, sizeof(temp));
    handle_write_error(ecriture);

    if (manuel_mode || vider) {
      while (buffer->offset > 0) {
        printf("[\x1B[4m%s\x1B[0m] %s", pseudo_destinataire, getString(buffer));
        fflush(stdout);
      }
      vider = false;
    }
    usleep(10000);
  }
  free(tempStr);
  close(fd_fifo_sender);
}


void handle_child_process(sharedMemo *buffer, char *pseudo_destinataire) {
  signal(SIGINT, SIG_IGN);
  char temp[BUFFER_SIZE];
  int fd_fifo_receiver = open(fifo_receiver, O_RDONLY);

  while (read(fd_fifo_receiver, temp, sizeof(temp)) > 0) {
    if (!manuel_mode) {
      if (bot_mode) {
        printf("[%s] %s", pseudo_destinataire, temp);
      } else {
        printf("[\x1B[4m%s\x1B[0m] %s", pseudo_destinataire, temp);
      }
      fflush(stdout);
    } else {
      int max = write_shared(buffer, temp);
      printf("\a");
      if (max == 1) {
        while (buffer->offset > 0) {
          printf("[\x1B[4m%s\x1B[0m] %s", pseudo_destinataire, getString(buffer));
        }
        write_shared(buffer, temp);
      }
      fflush(stdout);
    }
  }
  kill(getppid(), SIGPIPE);
  close(fd_fifo_receiver);
}
