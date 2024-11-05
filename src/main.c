#include "main.h"

char fifo_sender[MAX_LEN_FIFO];  
char fifo_receiver[MAX_LEN_FIFO]; 
int verif = 0; 

int verifier_erreurs(int argc, char* pseudo_utilisateur, char* pseudo_destinataire) {
  // Vérification du nombre d'arguments => chat pseudo_utilisateur pseudo_destinataire (obligatoire)
  if (argc < 3) {
    fprintf(stderr, "chat pseudo_utilisateur pseudo_destinataire [--bot] [--manuel]\n");
    return 1;
  }

  if (argc > 5) {
    fprintf(stderr, "Erreur: Trop d'arguments\n");
    return 4;
  }

  // Vérification de la longueur des pseudos
  if (strlen(pseudo_utilisateur) > MAX_PSEUDO_LEN || strlen(pseudo_destinataire) > MAX_PSEUDO_LEN) {
    fprintf(stderr, "Erreur : Pseudo trop long, maximum 30 octets/caractères.\n");
    return 2;
  }

  // Caractères interdits dans les pseudos
  const char* invalid_chars = "/-[]";
  if (strpbrk(pseudo_utilisateur, invalid_chars) || strpbrk(pseudo_destinataire, invalid_chars)) {
    fprintf(stderr, "Erreur : Pseudo contient des caractères invalides.\n");
    return 3;
  }

  // Si les pseudos sont "." ou ".." => erreur
  if (strcmp(pseudo_utilisateur, ".") == 0 || strcmp(pseudo_utilisateur, "..") == 0 ||
    strcmp(pseudo_destinataire, ".") == 0 || strcmp(pseudo_destinataire, "..") == 0) {
    fprintf(stderr, "Erreur : Pseudo invalide.\n");
    return 3;
  }

  return 0;
}

void create_pipe(const char* pipe_path) {
  if (mkfifo(pipe_path, 0666) == -1) {
    printf("PIPE DEJA EXISTANT\n"); // TODO bonne pratique
  }
}

void signal_management(int signa) {
   if (signa == SIGINT) {
      printf("Signal SIGINT reçu\n");
      unlink(fifo_sender);
      unlink(fifo_receiver);
      verif = 1;
      exit(1);
      // close(pipe1);
      // close(pipe2);
   }  
   else if (signa == SIGPIPE)
   {
      printf("Signal SIGPIPE reçu\n");
      // close(pipe1);
      // close(pipe2);
      exit(1);
   }
   return;
}

void concatener_pipes(char* fifo_path, const char* pseudo1, const char* pseudo2) {
    strcpy(fifo_path, BASE_FIFO_PATH);
    strcat(fifo_path, pseudo1);
    strcat(fifo_path, "-");
    strcat(fifo_path, pseudo2);
    strcat(fifo_path, END_FIFO_PATH);
}

void verification_param_optinnel(int argc, char* argv[], int* bot_mode, int* manuel_mode) {

  *bot_mode    = 0;
  *manuel_mode = 0;

  for (int i = 3; i < argc; i++) {
    if (strcmp(argv[i], PARAM_BOT) == 0) {
      *bot_mode    = 1;
    } else if (strcmp(argv[i], PARAM_MANUEL) == 0) {
      *manuel_mode = 1;
    } else {
      fprintf(stderr, "Option inconnue : %s\n", argv[i]);
      exit(1);
    }
  }

  if (*bot_mode == 1) {
    printf("Mode bot activé\n");
  }  
  if (*manuel_mode == 1) {
    printf("Mode manuel activé\n");
  }
}


// TODO paramètre optionnel (2.3 consignes)
int main(int argc, char* argv[]) {

  // Récupération des pseudos
  char* pseudo_utilisateur  = argv[1];
  char* pseudo_destinataire = argv[2];
  int bot_mode    = 0;
  int manuel_mode = 0;

  signal(SIGINT, signal_management);
  signal(SIGPIPE, signal_management);
   
  // char fifo_sender[MAX_LEN_FIFO];  
  // char fifo_receiver[MAX_LEN_FIFO];  

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

  printf("%s\n", fifo_sender);
  printf("%s\n", fifo_receiver);

  printf("pseudo_utilisateur : %s\n", pseudo_utilisateur);
  printf("pseudo_destinataire : %s\n", pseudo_destinataire);

  create_pipe(fifo_sender);
  create_pipe(fifo_receiver);

  // Ouverture des pipes

  char buffer[BUFFER_SIZE];
  pid_t fork_return = fork();

  if(fork_return > 0){
    
    int fd_fifo_sender   = open(fifo_sender, O_WRONLY);
    int fd_fifo_receiver = open(fifo_receiver, O_WRONLY);


    while ((fd_fifo_receiver != -1) && (fgets(buffer, sizeof(buffer), stdin) != NULL)){
      write(fd_fifo_sender, buffer, sizeof(buffer)); 
      
      // fd_fifo_sender   = open(fifo_sender, O_WRONLY);
      // fd_fifo_receiver = open(fifo_receiver, O_WRONLY);
    }

    close(fd_fifo_sender);
    close(fd_fifo_receiver);


  } else {

    int fd_fifo_sender   = open(fifo_sender, O_RDONLY);
    int fd_fifo_receiver = open(fifo_receiver, O_RDONLY);
    
    while ((fd_fifo_receiver != -1) && (read(fd_fifo_receiver, buffer, sizeof(buffer)) > 0) ){
      if (bot_mode == 1) {
        printf("[%s]: %s",pseudo_destinataire ,buffer);
      }
      else {
        printf("[\x1B[4m%s\x1B[0m]: %s",pseudo_destinataire ,buffer);
      }
      
      // fd_fifo_sender   = open(fifo_sender, O_RDONLY);
      // fd_fifo_receiver = open(fifo_receiver, O_RDONLY);

    }
    close(fd_fifo_sender);
    close(fd_fifo_receiver);
  }
    
  printf("Interuption");
  // if (fd_fifo_sender == -1 || fd_fifo_receiver == -1) {
  //   perror("Erreur lors de l'ouverture des pipes");
  //   unlink(fifo_sender); //supprimer les pipes
  //   unlink(fifo_receiver);
  //   exit(1);
  // }
  
  unlink(fifo_sender);
  unlink(fifo_receiver);

  return 0;
}
