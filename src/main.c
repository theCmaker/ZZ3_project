#include <stdio.h>
#include <math.h>
#include "sln.h"
#include "solution.h"

int main(int argc, char *argv[]) {
  int n;
  int *tab;
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
      tab = (int*) malloc (G.mob_nb*sizeof(int));
      if (tab) {
        for (n = 0; n < G.mob_nb; ++n) {
          tab[n] = n;
        }
        n = (argc > 2) ? heuristique_sequence(&G,argv[2],tab,G.mob_nb):heuristique_sequence(&G,"",tab,G.mob_nb);
        printf("%d mobiles interceptes.\n",n);
      } else {
        fprintf(stderr, "Memory error, could not build mobile sequence for H1\n");
      }
      graph_delete(&G);
    }
  } else {
    fprintf(stderr, "Please give a file name as an argument.\n");
  }
  return 0;
}
