#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "position.h"

  typedef struct _p_spd_t {
    pos_t p;      /* Coord */
    double vx;    /* Vitesse x */
    double vy;    /* Vitesse y */
  } p_spd_t;

  typedef struct _graph_t {
    int      dep_nb;     /* Nb depots */
    int      mob_nb;     /* Nb mobiles */
    int      inter_nb;   /* Nb intercepteurs */
    double   inter_spd;  /* Vitesse intercepteurs */
    pos_t   *dep_pos;    /* Positions des depots */
    p_spd_t *moves;      /* Position et vitesse des mobiles */
  } graph_t;

  /* Lecture depuis un fichier */
  int graph_load(graph_t *, char *);
  /* Ecriture dans un fichier */
  int graph_save(graph_t, char *);
  /* Affichage donnees */
  void graph_print_info(graph_t);
  /* Suppression structure */
  void graph_delete(graph_t *);

#endif
