#include "memory.h"

// Fonctions

// ############ Mémoire partagée ############
sharedMemo* shared_memory_initializer(size_t memory_size){
    // Paramètres pour initialiser la mémoire partagée
    const int protection = PROT_READ | PROT_WRITE;  // Autoriser la lecture et l'écriture
    const int visibility = MAP_SHARED| MAP_ANONYMOUS;  // Visibilité de la mémoire, qui peut y accéder
    const int fd = -1;  // Pas de fichier utilisé
    const int offset = 0;  // Pas de décalage initial

    // Allouer la mémoire partagée
    sharedMemo* memo = mmap(NULL, memory_size, protection, visibility, fd, offset);
    
    // Vérifier si la mémoire est bien initialisée
    if (memo == MAP_FAILED) {
        perror("mmap failed");
        exit(1);  
    }

    memo->offset = 0;
    return memo;
}

void clean_shared_memo(sharedMemo* memo){
    if (munmap(memo, MAX_MEMORY_SIZE) == -1){
    perror("munmap()");
  }
}

int write_shared(sharedMemo* memo, const char* str){
    int len = (int)strlen(str) + 1; // +1 pour prendre en considération le '\0'
    
    // Si limite d'espace atteint
    if (memo->offset + len > MAX_MEMORY_SIZE){
        printf("Shared memory is full\n");
        return 1;
    }

    else{
        for (int i = memo->offset - 1; i >= 0; --i){
            memo->data[len + i] = memo->data[i];
        }
        memcpy(&memo->data[0], str, (unsigned long)len);
        memo->offset += len;
    }
    return 0;
}

void read_memo(sharedMemo* memo){
    if (memo->offset == 0){
        printf("The shared memory is empty\n");
        return;
    }

    // Utilisation d'un buffer de taille suffisante pour s'adapter à tous les cas
    // Par exemple un string faisant la taille de la mémoire partagée elle-même
    static char ret[MAX_MEMORY_SIZE]; 
    int idx = 0, ret_idx = 0;

    while (idx < memo->offset) {
        if (memo->data[idx] != '\0') {
            ret[ret_idx++] = memo->data[idx];
        } else {
            // Fin du string
            ret[ret_idx] = '\0';
            printf("%s", ret);
            ret_idx = 0;  // Réinitialisation du compteur pour le string suivant
        }
        idx++;
    }
}

char* getString(sharedMemo* memo){
    if (memo->offset == 0){
        printf("The shared memory is empty\n");
        return NULL;
    }

    int pos = memo->offset - 1;
    while (pos > 0 && memo->data[pos - 1] != '\0') {
        pos--;
    }

    // Récupère tous les caractères jusqu'au prochain '\0', jusqu'au prochain string
    char* last_string = &memo->data[pos];
    
    // Mettre à jour le décalage pour effacer le string récupéré de la mémoire
    memo->offset = pos;

    return last_string;
}

// ############ Fonctions mémoire dynamique ############
void etendre_liste(liste_t* liste) {
    liste->taille_reelle *= 2;
    char* new_vals = (char*)malloc(liste->taille_reelle);

    // Vérifier s'il n'y a eu aucun problème lors du malloc
    if (new_vals == NULL) {
        perror("malloc()");
        free(liste->valeurs);
        exit(1);
    }

    memcpy(new_vals, liste->valeurs, liste->taille_logique);
    free(liste->valeurs);
    liste->valeurs = new_vals;
}

liste_t getDynamicString() {
  liste_t liste = {0, INIT_MALLOC_SIZE, NULL};
  liste.valeurs = malloc(liste.taille_reelle);

  if (liste.valeurs == NULL) {
    perror("malloc()");
    exit(1);
  }

  char str[CHUNK] = {0};
  bool flag = false;
  size_t str_len = CHUNK;

  while (str_len >= CHUNK || flag) {
    if (fgets(str, sizeof(str), stdin) ==  NULL) { // Gestion de EOF
      free(liste.valeurs);
      liste.valeurs = NULL;
      return liste;
    }

    str_len = strlen(str);

    if (str_len == (CHUNK - 1) && str[str_len - 1] != '\n') { // Potentiellement encore de l'input à lire
      flag = true; // Pour faire tourner la boucle une fois de plus
    }

    if (flag && str_len < (CHUNK - 1)) {
      flag = false;
    }

    // Étendre la liste si nécessaire
    if (liste.taille_logique + str_len + 1 >= liste.taille_reelle) {
      etendre_liste(&liste);
    }

    // Ajouter la chaîne au buffer
    memcpy(liste.valeurs + liste.taille_logique, str, str_len);
    liste.taille_logique += str_len;
  }

  liste.valeurs[liste.taille_logique] = '\0';
  return liste;
}
