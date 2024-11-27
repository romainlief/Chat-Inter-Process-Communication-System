#include "sig.h"
#include "global.h"




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

void initialiser_signal_action(struct sigaction *sa, void (*handler)(int)) {
    sa->sa_handler = handler;
    sigemptyset(&sa->sa_mask);
    sa->sa_flags = 0;
}

void configurer_signaux(struct sigaction *sa) {
    sigaction(SIGINT, sa, NULL);
    sigaction(SIGPIPE, sa, NULL);
}


void cleanning_fifos(const char *fifo_sender, const char *fifo_receiver) {
    unlink(fifo_sender);
    unlink(fifo_receiver);
}
