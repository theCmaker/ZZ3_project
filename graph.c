#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

#define SIZE_BUF 100

char buf[SIZE_BUF];

int graph_init_moves(graph_t *G) {
  int res = 0;
  if ((G->moves = (p_spd_t*) malloc((G->mobiles+G->inter_nb)*sizeof(p_spd_t)))) {
    res = 1;
  }
  return res;
}

int graph_load(graph_t *G, char *f) {
  FILE *file = NULL;
  int i;
  int res = 0;
  if ((file = fopen(f,"r"))) {
    fscanf(file,"%d",&G->dep_nb);
    fscanf(file,"%d",&G->mobiles);
    fscanf(file,"%d %lf",&G->inter_nb,&G->inter_spd);
    if (graph_init_moves(G)) {
      i = 0;
      while (!feof(file) && i < G->inter_nb + G->mobiles) {
        fscanf(file,"%lf %lf %lf %lf",&(G->moves[i].x),&(G->moves[i].y),&(G->moves[i].vx),&(G->moves[i].vy));
        ++i;
      }
    } else {
      res = 0;
    }
    fclose(file);
    res = 1;
  }
  return res;
}

void graph_print_info(graph_t G) {
  int i;
  printf("Number of depots:\t%d\n",G.dep_nb);
  printf("Number of mobiles:\t%d\n",G.mobiles);
  printf("Number of sensors:\t%d\n",G.inter_nb);
  printf("Sensors speed:\t%f\n",G.inter_spd);
  printf("Depots information:\n");
  for (i = 0; i < G.dep_nb; i++) {
    printf("%d:\t%+.5f\t%+.5f;\t%+.5f\t%+.5f\n",i,G.moves[i].x,G.moves[i].y,G.moves[i].vx,G.moves[i].vy);
  }
  printf("Mobiles information:\n");
  for (i = 0; i < G.mobiles; i++) {
    printf("%d:\t%+.5f\t%+.5f;\t%+.5f\t%+.5f\n",i,G.moves[G.dep_nb+i].x,G.moves[G.dep_nb+i].y,G.moves[G.dep_nb+i].vx,G.moves[G.dep_nb+i].vy);
  }
}

int graph_save(graph_t G, char *f) {
  return 0;
}

void graph_delete(graph_t *G) {
  free(G->moves);
}
