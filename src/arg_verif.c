#include "arg_verif.h"

// Variables globales
int bot_mode = 0;
int manuel_mode = 0;

int min_argc(int argc) {
  if (argc < 3) {
    fprintf(stderr,
            "chat pseudo_utilisateur pseudo_destinataire [--bot] [--manuel]\n");
    exit(1);
  }
  return 0;
}

int verifier_erreurs(int argc, const char *pseudo_utilisateur, const char *pseudo_destinataire) 
 {
  if (argc > 5) {
    fprintf(stderr, "Erreur: Trop d'arguments\n");
    exit(5);
  }

  // Vérification de la longueur des pseudos
  if (strlen(pseudo_utilisateur) > MAX_PSEUDO_LEN ||
      strlen(pseudo_destinataire) > MAX_PSEUDO_LEN) {
    fprintf(stderr,
            "Erreur : Pseudo trop long, maximum 30 octets/caractères.\n");
    exit(2);
  }

  // Caractères interdits dans les pseudos
  const char *invalid_chars = "/-[]";
  if (strpbrk(pseudo_utilisateur, invalid_chars) ||
      strpbrk(pseudo_destinataire, invalid_chars)) {
    fprintf(stderr, "Erreur : Pseudo contient des caractères invalides.\n");
     exit(3);
  }

  // Si les pseudos sont "." ou ".." => erreur
  if (strcmp(pseudo_utilisateur, ".") == 0 ||
      strcmp(pseudo_utilisateur, "..") == 0 ||
      strcmp(pseudo_destinataire, ".") == 0 ||
      strcmp(pseudo_destinataire, "..") == 0) {
    fprintf(stderr, "Erreur : Pseudo invalide.\n");
    exit(3);
  }

  return 0;
}

void verification_param_optinnel(int argc, char *argv[], int *bot_mode,
                                 int *manuel_mode) {

  *bot_mode = 0;    // pointeur vers la variable bot_mode
  *manuel_mode = 0; // pointeur vers la variable manuel_mode

  // boucle qui parcourt les arguments de 3 à argc et si l'argument est --bot
  // et/ou --manuel, on active le mode correspondant
  for (int i = 3; i < argc; i++) {
    if (strcmp(argv[i], PARAM_BOT) == 0) {
      *bot_mode = 1;
    } else if (strcmp(argv[i], PARAM_MANUEL) == 0) {
      *manuel_mode = 1;
    } else {
      fprintf(stderr, "Option inconnue : %s\n", argv[i]);
      exit(1);
    }
  }
}



