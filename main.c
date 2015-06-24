#include <stdio.h>
#include <math.h>
#include "sln.h"
#include "solution.h"

int main(int argc, char *argv[]) {
  int n;
  graph_t G;
  pos_t pos;
  set_pos(&pos,0,0);
  /*printf(" 0 est fini ? %d ",finite(0.0));
  test_solution();*/
  /*test_soln();*/
  if (argc > 1) {
    if (graph_load(&G,argv[1])) {
      graph_print_info(G);
      n = (argc > 2) ? heuristique_plus_rapide(&G,argv[2]):heuristique_plus_rapide(&G,"");
      printf("%d mobiles interceptes.\n",n);
      n = (argc > 2) ? heuristique_sequence(&G,argv[2]):heuristique_sequence(&G,"");
      printf("%d mobiles interceptes.\n",n);
      /*test_interception(&G, &pos,0,0);*/
      graph_delete(&G);
    }
  } else {
    fprintf(stderr, "Please give a file name as an argument.\n");
  }
  return 0;
}
