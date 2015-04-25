#ifndef __POSITION_H__
#define __POSITION_H__

typedef struct _pos_t {
  double x;
  double y;
} pos_t;

void set_pos (pos_t *p, double x, double y);
void get_pos(pos_t p, double* x, double *y);

/*Indique si deux positions sont egales a esp pres*/
int is_equal(pos_t p1, pos_t p2, double esp);

#endif
