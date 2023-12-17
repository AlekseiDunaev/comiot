#include "header/uciio.h"

int get_config_entry (char *path, char *buffer) {
  struct uci_context *c;
  struct uci_ptr ptr;
     
  c = uci_alloc_context();
  if (uci_lookup_ptr(c, &ptr, path, true) != UCI_OK) {
    uci_perror (c, "XXX");
    return 1;
  }

  strcpy(buffer, ptr.o->v.string);
  uci_free_context (c);
  return 0;
}
