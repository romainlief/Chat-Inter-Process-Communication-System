#include "global.h"
#include "constant.h"

struct sigaction sa;
int manuel_mode;
bool vider;

char fifo_sender[MAX_LEN_FIFO];
char fifo_receiver[MAX_LEN_FIFO];
