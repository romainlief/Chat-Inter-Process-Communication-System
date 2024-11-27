#include "sig.h"


int manuel_mode;
bool vider;
char fifo_sender[MAX_LEN_FIFO];  
char fifo_receiver[MAX_LEN_FIFO];

void signal_management(int signa) {
  if (signa == SIGINT) {
    if(manuel_mode){
      vider = true;
    }
    else{
      fclose(stdin);
      unlink(fifo_receiver);
      unlink(fifo_sender);
      exit(4);
    }
  } else if (signa == SIGPIPE) {
    // printf("pipe\n");
    fclose(stdin);

  }
}
