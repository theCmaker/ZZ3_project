#ifndef __GRAPH_H__
#define __GRAPH_H__

  typedef struct _pos_t {
    double x;
    double y;
  } pos_t;

  typedef struct _p_spd_t {
    pos_t p;
    double vx;
    double vy;
  } p_spd_t;

  typedef struct _graph_t {
    int      dep_nb;
    int      mob_nb;
    int      inter_nb;
    double   inter_spd;
    pos_t   *dep_pos;
    p_spd_t *moves;
  } graph_t;

  int graph_load(graph_t *, char *);
  int graph_save(graph_t, char *);
  void graph_print_info(graph_t);
  void graph_delete(graph_t *);

#endif
