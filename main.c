#include <stdio.h>
#include <math.h>
#include "graph.h"

int main(int argc, char *argv[]) {
  graph_t G;
  if (argc > 1) {
    if (graph_load(&G,argv[1])) {
      graph_print_info(G);
      graph_delete(&G);
    }
  } else {
    fprintf(stderr, "Please give a file name as an argument.\n");
  }
  return 0;
}
