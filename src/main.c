// Inclusions
#include "main.h"

int main(int argc, char *argv[]) {
  min_argc(argc);

  char *pseudo_utilisateur = argv[1]; // Récupération des pseudos
  char *pseudo_destinataire = argv[2];

  sa.sa_handler = signal_management;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  verifier_erreurs(argc, pseudo_utilisateur, pseudo_destinataire); // Vérification des erreurs
  verification_param_optinnel(argc, argv, &bot_mode, &manuel_mode); // Gestion des options --bot et --manuel

  // Création des deux path pipes en concaténant les pseudos
  concatener_pipes(fifo_sender, pseudo_utilisateur, pseudo_destinataire);
  concatener_pipes(fifo_receiver, pseudo_destinataire, pseudo_utilisateur);

  initialiser_pipes(fifo_sender, fifo_receiver);

  size_t memory_size = 1;
  if (manuel_mode)
  {
    memory_size = MAX_MEMORY_SIZE;
  }
  sharedMemo *buffer = shared_memory_initializer(memory_size);

  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGPIPE, &sa, NULL);

  pid_t fork_return = fork();
  if (fork_return < 0)
  {
    perror("fork()");
    return 6;
  }

  if (fork_return > 0)
  {

    int fd_fifo_sender = open(fifo_sender, O_WRONLY);
    char *tempStr = NULL;
    size_t size = 0;

    if (!manuel_mode)
    {
      sigignore(SIGINT);
    }

    ssize_t code;
    while ((code = getline(&tempStr, &size, stdin)))
    {
      if (code == -1)
      {
        if (vider)
        {
          while (buffer->offset > 0)
          {
            printf("[\x1B[4m%s\x1B[0m] %s", pseudo_destinataire,
                   getString(buffer));
            fflush(stdout);
          }
          vider = false;
          clearerr(stdin);
          continue;
        }
        else
        {
          break;
        }
      }

      char temp[size];
      memcpy(temp, tempStr, size);

      if (!bot_mode)
      {
        printf("[\x1B[4m%s\x1B[0m] %s", pseudo_utilisateur, temp); // S'effectue un nombre indéterminé de fois avant de s'arrêter
        fflush(stdout);
      }

      ssize_t ecriture = write(fd_fifo_sender, temp, sizeof(temp));

      if (ecriture == -1)
      {
        perror("write()");
        break;
      }

      if (manuel_mode || vider)
      {
        while (buffer->offset > 0)
        {
          printf("[\x1B[4m%s\x1B[0m] %s", pseudo_destinataire,
                 getString(buffer));
          fflush(stdout);
        }
        vider = false;
      }

      usleep(10000);
    }
    close(fd_fifo_sender);
  }
  else if (fork_return == 0)
  {
    sigignore(SIGINT);

    char temp[BUFFER_SIZE];
    int fd_fifo_receiver = open(fifo_receiver, O_RDONLY);

    while (read(fd_fifo_receiver, temp, sizeof(temp)) > 0)
    {

      if (!manuel_mode)
      {
        if (bot_mode)
        {
          printf("[%s] %s", pseudo_destinataire, temp);
        }
        else
        {
          printf("[\x1B[4m%s\x1B[0m] %s", pseudo_destinataire, temp);
        }
        fflush(stdout);
      }

      else
      {
        int max = write_shared(buffer, temp);
        printf("\a");

        if (max == 1)
        {
          while (buffer->offset > 0)
          {
            printf("[\x1B[4m%s\x1B[0m] %s", pseudo_destinataire,
                   getString(buffer));
          }
          write_shared(buffer, temp);
        }

        fflush(stdout); // Permet d'émettre son directement
      }
    }
    kill(getppid(), SIGPIPE);

    close(fd_fifo_receiver);
  }

  while (buffer->offset > 0)
  {
    printf("[\x1B[4m%s\x1B[0m] %s", pseudo_destinataire, getString(buffer));
    fflush(stdout);
  }
  unlink(fifo_sender);
  unlink(fifo_receiver);
  clean_shared_memo(buffer);
  return 0;
}
