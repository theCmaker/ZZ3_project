#include <stdio.h>
#include <math.h>
#include "graph.h"
#include "calcul.h"
#include "position.h"
#include "solution.h"
#include "sln.h"

int main(int argc, char *argv[]) {
  graph_t G;
  pos_t pos;
  set_pos(&pos,0,0);
  /*printf(" 0 est fini ? %d ",finite(0.0));
  test_solution();*/
  test_soln();
  if (argc > 1) {
    if (graph_load(&G,argv[1])) {
      graph_print_info(G);
      test_interception(&G, &pos,0,0);
      graph_delete(&G);
    }
  } else {
    fprintf(stderr, "Please give a file name as an argument.\n");
  }
  return 0;
}
