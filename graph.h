#ifndef __GRAPH_H__
#define __GRAPH_H__

  typedef struct _p_spd_t {
    double x;
    double y;
    double vx;
    double vy;
  } p_spd_t;

  typedef struct _graph_t {
    int      dep_nb;
    int      mobiles;
    int      inter_nb;
    double   inter_spd;
    p_spd_t *moves;
  } graph_t;

  int graph_load(graph_t *, char *);
  int graph_save(graph_t, char *);
  void graph_print_info(graph_t);
  void graph_delete(graph_t *);

#endif
