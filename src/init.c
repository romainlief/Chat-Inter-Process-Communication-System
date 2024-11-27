#include "init.h"

void initialiser_et_verifier(int argc, char *argv[], const char *pseudo_utilisateur, const char *pseudo_destinataire, char *fifo_sender, char *fifo_receiver, int *bot_mode, int *manuel_mode) {
  verifier_erreurs(argc, pseudo_utilisateur, pseudo_destinataire);
  verification_param_optinnel(argc, argv, bot_mode, manuel_mode);
  concatener_pipes(fifo_sender, pseudo_utilisateur, pseudo_destinataire);
  concatener_pipes(fifo_receiver, pseudo_destinataire, pseudo_utilisateur);
  initialiser_pipes(fifo_sender, fifo_receiver);
}
