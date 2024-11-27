#ifndef GLOBAL_H
#define GLOBAL_H

#include <signal.h>
#include <stdbool.h>

extern struct sigaction sa;
extern int manuel_mode;
extern bool vider;

extern char fifo_sender[];
extern char fifo_receiver[];

#endif // GLOBALS_H
