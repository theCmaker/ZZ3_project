#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

#define SIZE_BUF 100


void get_next_line(char *buf, FILE *file){
  do {
    fgets(buf,SIZE_BUF,file);
  } while (buf[0] == '#'); /* Ignorer les commentaires, i.e. lignes commencant par # */
}

int graph_init_moves(graph_t *G) {
  int res = 0;
  /* Allocation mobiles */
  if ((G->moves = (p_spd_t*) malloc((G->mob_nb)*sizeof(p_spd_t)))) {
    res = 1;
    /* Allocation depots */
    if (!(G->dep_pos = (pos_t*) malloc((G->dep_nb)*sizeof(pos_t)))) {
      res = 0;
      free(G->moves);
    }
  }
  return res;
}

int graph_load(graph_t *G, char *f) {
  char buf[SIZE_BUF];
  FILE *file = NULL;
  int i;
  int res = 0;
  double x, y;
  if ((file = fopen(f,"r"))) {

    /* Nb depots */
    get_next_line(buf,file);
    sscanf(buf,"%d",&G->dep_nb);

    /* Nb mobiles */
    get_next_line(buf,file);
    sscanf(buf,"%d",&G->mob_nb);

    /* Nb intercepteurs / vitesse */
    get_next_line(buf,file);
    sscanf(buf,"%d %lf",&G->inter_nb,&G->inter_spd);

    if (graph_init_moves(G)) { /* allocation reussie */
      res = 1;

      /* Coord depots */
      for (i = 0; i < G->dep_nb; ++i) {
        get_next_line(buf,file);
        sscanf(buf,"%lf %lf",&x,&y);
        set_pos(&(G->dep_pos[i]),x,y);
      }

      /* Coord mobiles / vitesse mobiles */
      for (i = 0; i < G->mob_nb; ++i) {
        get_next_line(buf,file);
        sscanf(buf,"%lf %lf %lf %lf",&x,&y,&(G->moves[i].vx),&(G->moves[i].vy));
        set_pos(&(G->moves[i].p),x ,y);
      }
    }
    fclose(file);
  }
  return res;
}

void graph_print_info(graph_t G) {
  int i;
  printf("Number of depots:\t%d\n",G.dep_nb);
  printf("Number of mobiles:\t%d\n",G.mob_nb);
  printf("Number of sensors:\t%d\n",G.inter_nb);
  printf("Sensors speed:\t%f\n",G.inter_spd);
  printf("Depots information:\n");
  for (i = 0; i < G.dep_nb; ++i) {
    printf("%d:\t%+.5f\t%+.5f\n",i,G.dep_pos[i].x,G.dep_pos[i].y);
  }
  printf("Mobiles information:\n");
  for (i = 0; i < G.mob_nb; ++i) {
    printf("%d:\t%+.5f\t%+.5f;\t%+.5f\t%+.5f\n",i,G.moves[i].p.x,G.moves[i].p.y,G.moves[i].vx,G.moves[i].vy);
  }
}

int graph_save(graph_t G, char *f) {
  int res = 0;
  int i;
  FILE *file;
  if ((file = fopen(f,"w+"))) {
    res = 1;
    /* Depots */
    fprintf(file, "%d\n",G.dep_nb);
    /* Mobiles */
    fprintf(file, "%d\n",G.mob_nb);
    /* Interceptors */
    fprintf(file, "%d %f\n",G.inter_nb, G.inter_spd);
    /* Depots positions */
    for (i = 0; i < G.dep_nb; ++i) {
      fprintf(file,"%f %f\n",G.dep_pos[i].x,G.dep_pos[i].y);
    }
    /* Mobiles positions and speeds */
    for (i = 0; i < G.mob_nb; ++i) {
      fprintf(file,"%f %f %f %f\n",G.moves[i].p.x,G.moves[i].p.y,G.moves[i].vx,G.moves[i].vy);
    }
    fclose(file);
  }
  return res;
}

void graph_delete(graph_t *G) {
  free(G->dep_pos); /* Depots */
  free(G->moves);   /* Mobiles */
}
