#include "main.h"

/*
2.2 => terminé
2.3.1 => terminé
2.3.2 => terminé
2.3.3 => mode manuel à faire => Mémoire saturé
2.4 => Terminé
2.5 => terminé
2.6 => Terminée
2.7 => Affiner SIGINT et SIFPIPE 
2.8.1 => terminé  
2.8.2 => empecher le terminal 2 d'ecrire apres suppression pipe
2.8.3 => terminer
2.8.4 => à affiner

*/



char fifo_sender[MAX_LEN_FIFO];  
char fifo_receiver[MAX_LEN_FIFO]; 
int bot_mode    = 0; 
int manuel_mode = 0; 
int verif = 0;  

sharedMemo* shared_memory_initializer(){
    // Shared memory parameters
    const int protection = PROT_READ | PROT_WRITE;  // Allow read and write
    const int visibility = MAP_SHARED| MAP_ANONYMOUS;  // Shared memory
    const int fd = -1;  // No file backing
    const int offset = 0;  // No offset

    // Allocate anonymous memory with no file backing
    sharedMemo* memo = mmap(NULL, 4096, protection, visibility, fd, offset);
    
    // Starting adress of shared memory in RAM
    printf("Pointer address: %p\n", (void*)memo);
    
    // Handle mmap failure properly
    if (memo == MAP_FAILED) {
        perror("mmap failed");
        exit(1);  
    }

    memo->offset = 0;
    return memo;
}

void clean_shared_memo(sharedMemo* memo){
    if (munmap(memo, 4096) == -1){
    perror("munmap()");
  }
}

void write_shared(sharedMemo* memo, const char* str){
    int len = (int)strlen(str) + 1; // Taking into account the '\0' character
    
    // In case no more space
    if (memo->offset + len > 4096){
        printf("Shared memory is full\n");
        return;
    }

    else{
        for (int i = memo->offset - 1; i >= 0; --i){
            memo->data[len + i] = memo->data[i];
        }
        memcpy(&memo->data[0], str, (unsigned long)len);
        memo->offset += len;
    }
}

void read_memo(sharedMemo* memo){
    if (memo->offset == 0){
        printf("The shared memory is empty\n");
        return;
    }

    // Using a large enough buffer to adapt to all cases
    static char ret[4096]; 
    int idx = 0, ret_idx = 0;

    while (idx < memo->offset) {
        if (memo->data[idx] != '\0') {
            ret[ret_idx++] = memo->data[idx];
        } else {
            // End of a string
            ret[ret_idx] = '\0';
            printf("%s", ret);
            ret_idx = 0;  // Reseting for next string
        }
        idx++;
    }
}

char* getString(sharedMemo* memo){
    // Returns the first given string
    if (memo->offset == 0){
        printf("The shared memory is empty\n");
        return NULL;
    }

    int pos = memo->offset - 1;
    while (pos > 0 && memo->data[pos - 1] != '\0') {
        pos--;
    }

    // Retrievieng all characters untill next '\0'
    char* last_string = &memo->data[pos];
    
    // Updating offset to delete the string from memory
    memo->offset = pos;

    return last_string;
}

int verifier_erreurs(int argc, char* pseudo_utilisateur, char* pseudo_destinataire) {
  // Vérification du nombre d'arguments => chat pseudo_utilisateur pseudo_destinataire (obligatoire)
  if (argc < 3) {
    fprintf(stderr, "chat pseudo_utilisateur pseudo_destinataire [--bot] [--manuel]\n");
    return 1;
  }

  if (argc > 5) {
    fprintf(stderr, "Erreur: Trop d'arguments\n");
    return 5;
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
    fclose(stdin);

   } else if (signa == SIGPIPE) {
      printf("Signal SIGPIPE reçu\n");
      exit(1);
   }
}

void concatener_pipes(char* fifo_path, const char* pseudo1, const char* pseudo2) {
    strcpy(fifo_path, BASE_FIFO_PATH);
    strcat(fifo_path, pseudo1);
    strcat(fifo_path, "-");
    strcat(fifo_path, pseudo2);
    strcat(fifo_path, END_FIFO_PATH);
}

void verification_param_optinnel(int argc, char* argv[], int* bot_mode, int* manuel_mode) {

  *bot_mode    = 0; // pointeur vers la variable bot_mode
  *manuel_mode = 0; // pointeur vers la variable manuel_mode

  // boucle qui parcourt les arguments de 3 à argc et si l'argument est --bot et/ou --manuel, on active le mode correspondant
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
  

  sa.sa_handler = signal_management;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGPIPE, &sa, NULL);
   
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

  // Ouverture des pipes
  create_pipe(fifo_sender);
  create_pipe(fifo_receiver);

  
  char temp[BUFFER_SIZE];
  sharedMemo* buffer = shared_memory_initializer();

  pid_t fork_return = fork();
  


  if(fork_return > 0){
    
    int fd_fifo_sender   = open(fifo_sender, O_WRONLY);
    int fd_fifo_receiver = open(fifo_receiver, O_WRONLY);


    while (fgets(temp, sizeof(temp), stdin) != NULL){
      if (!bot_mode) {
        printf("[\x1B[4m%s\x1B[0m] %s",pseudo_utilisateur,temp);
      }

      write(fd_fifo_sender, temp, sizeof(temp));

      
      if(manuel_mode){
        

        while(buffer->offset > 0){
          printf("[\x1B[4m%s\x1B[0m] %s", pseudo_destinataire ,getString(buffer));
        }

      }
    }

    close(fd_fifo_sender);
    close(fd_fifo_receiver);


  } else {

    int fd_fifo_sender   = open(fifo_sender, O_RDONLY);
    int fd_fifo_receiver = open(fifo_receiver, O_RDONLY);
    
    while (read(fd_fifo_receiver, temp, sizeof(temp)) > 0){
        

      if(!manuel_mode){
        if (bot_mode) {
          printf("[%s]: %s",pseudo_destinataire ,temp);
        }
        else {
          printf("[\x1B[4m%s\x1B[0m]: %s",pseudo_destinataire ,temp);
        }
      }

      else {
        write_shared(buffer, temp);
        printf("\a");
        fflush(stdout); // Permet d'émettre son directement
      }
    }

    close(fd_fifo_sender);
    close(fd_fifo_receiver);
  }

  while(buffer->offset > 0){
    printf("[\x1B[4m%s\x1B[0m] %s", pseudo_destinataire ,getString(buffer));
    }


  kill(fork_return, SIGKILL);  // termine l'enfant
  printf("Interuption\n");
  unlink(fifo_sender);
  unlink(fifo_receiver);

  return 0;
}
