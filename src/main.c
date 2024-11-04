#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define MAX_PSEUDO_LEN 30 //longueur max des pseudos dans les consignes
#define MAX_LEN_FIFO   72 //taille fifo max=72 (5 + 30 + 1 + 30 + 5 + 1)
#define BASE_FIFO_PATH "/tmp/" //chemin de base des pipes
#define END_FIFO_PATH ".chat" //fin du chemin des pipes
#define PARAM_BOT "--bot" //paramètre optionnel bot
#define PARAM_MANUEL "--manuel" //paramètre optionnel manuel


int verifier_erreurs(int argc, char* pseudo_utilisateur, char* pseudo_destinataire) {
  /**
  * Vérifie les erreurs possibles lors de l'exécution du programme.
  * argc : nombre d'arguments
  * pseudo_utilisateur : pseudo de l'utilisateur
  * pseudo_destinataire : pseudo du destinataire
  * Retourne 0 si aucune erreur, sinon le code d'erreur.
  * Retourne 1 : nombre d'arguments insuffisant
  * Retourne 2 : pseudo trop long
  * Retourne 3 : pseudo contient des caractères invalides
  * Retourne 4 : trop d'arguments
  */
  // Vérification du nombre d'arguments => chat pseudo_utilisateur pseudo_destinataire (obligatoire)
  if (argc < 3) {
    fprintf(stderr, "chat pseudo_utilisateur pseudo_destinataire [--bot] [--manuel]\n");
    return 1;
  }

  if (argc > 5) {
    fprintf(stderr, "Trop d'arguments\n");
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
  /**
   * Crée un pipe nommé.
   * pipe_path : chemin du pipe
   * Retourne void
   */
  if (mkfifo(pipe_path, 0666) == -1) {
    perror("Erreur lors de la création du pipe");
    exit(1);
  }
}

void signal_management(int signa) {
  /**
   * Gère les signaux SIGINT et SIGPIPE.
   * signa : signal reçu
   * Retourne void
   */
   if (signa == SIGINT) {
      printf("Signal SIGINT reçu\n");
      //close(); pipe1
      //close(); pipe2
      exit(0);
   }  
   else if (signa == SIGPIPE)
   {
      printf("Signal SIGPIPE reçu\n");
      //close(); pipe1
      //close(); pipe2
      exit(1);
   }
}

void concatener_pipes(char* fifo_path, const char* pseudo1, const char* pseudo2) {
    /**
     * Concatène les pseudos pour créer le chemin du pipe.
     * fifo_path : chemin du pipe
     * pseudo1 : pseudo de l'utilisateur
     * pseudo2 : pseudo du destinataire
     * Retourne void
     */
    strcpy(fifo_path, BASE_FIFO_PATH);
    strcat(fifo_path, pseudo1);
    strcat(fifo_path, "-");
    strcat(fifo_path, pseudo2);
    strcat(fifo_path, END_FIFO_PATH);
}

void verification_param_optinnel(int argc, char* argv[], int* bot_mode, int* manuel_mode) {
  /**
   * Vérifie si les paramètres optionnels --bot et --manuel sont présent et si d'autres paramètres sont inconnus renvoie une erreur.
   * argc : nombre d'arguments
   * argv : tableau des arguments
   * bot_mode : pointeur vers la variable bot_mode
   * manuel_mode : pointeur vers la variable manuel_mode
   * Retourne void
   */
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
   
  char fifo_sender[MAX_LEN_FIFO];  
  char fifo_receiver[MAX_LEN_FIFO];  

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
  int fd_fifo_sender   = open(fifo_sender, O_WRONLY);
  int fd_fifo_receiver = open(fifo_receiver, O_RDONLY);
  // Gestion des erreurs
  if (fd_fifo_sender == -1 || fd_fifo_receiver == -1) {
    perror("Erreur lors de l'ouverture des pipes");
    unlink(fifo_sender); //supprimer les pipes
    unlink(fifo_receiver);
    exit(1);
  }
  
  signal(SIGINT, signal_management);
  signal(SIGPIPE, signal_management);

  // utiliser fork()

  // scan for messages

  // TODO Supprimer les pipes (2.4 consignes)

  // Fermeture des pipes
  close(fd_fifo_sender);
  close(fd_fifo_receiver);
  unlink(fifo_sender);
  unlink(fifo_receiver);

  return 0;
}
