#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


#include "main.h"

#define MAX_PSEUDO_LEN 30

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
    */
   // Vérification du nombre d'arguments => chat pseudo_utilisateur pseudo_destinataire (obligatoire)
   if (argc < 3) {
      fprintf(stderr, "chat pseudo_utilisateur pseudo_destinataire [--bot] [--manuel]\n");
      return 1;
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

void test() {
   printf("Test\n");
}

int main(int argc, char* argv[]) {
   // Récupération des pseudos
   char* pseudo_utilisateur = argv[1];
   char* pseudo_destinataire = argv[2];
   char fifo_dir[72] = "/tmp/";  //taille fifo_dir max=36
   char fifo_dir2[72] = "/tmp/";  //taille fifo_dir max=36

   // Vérification des erreurs
   int erreur = verifier_erreurs(argc, pseudo_utilisateur, pseudo_destinataire);
   if (erreur != 0) {
      return erreur;
   }

   // Création path pipe
   strcat(fifo_dir, pseudo_utilisateur);
   strcat(fifo_dir, "-");
   strcat(fifo_dir, pseudo_destinataire);
   strcat(fifo_dir, ".chat");

   strcat(fifo_dir2, pseudo_destinataire);
   strcat(fifo_dir2, "-");
   strcat(fifo_dir2, pseudo_utilisateur);
   strcat(fifo_dir2, ".chat");
   
   printf("%s\n", fifo_dir);
   printf("%s\n", fifo_dir2);


   return 0;
}
