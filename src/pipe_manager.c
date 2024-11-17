#include "pipe_manager.h"

// Fonctions
void concatener_pipes(char *fifo_path, const char *pseudo1,
                      const char *pseudo2) {
  strcpy(fifo_path, BASE_FIFO_PATH);
  strcat(fifo_path, pseudo1);
  strcat(fifo_path, "-");
  strcat(fifo_path, pseudo2);
  strcat(fifo_path, END_FIFO_PATH);
}

void initialiser_pipes(char *fifo_sender, char *fifo_receiver) {
  mkfifo(fifo_sender, 0666);
  mkfifo(fifo_receiver, 0666);
}
