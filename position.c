#include <stdio.h>
#include "position.h"

void set_pos (pos_t *p, double x, double y) {
  p->x = x;
  p->y = y;
}


void get_pos(pos_t p, double* x, double *y){
  *x = p.x;
  *y = p.y;
}


int is_equal(pos_t p1, pos_t p2,double esp){
  int res = 0;
  if ( ( ((p1.x - esp) <= p2.x) && (p2.x <= (p1.x + esp)) ) && ( ((p1.y - esp) <= p2.y) && (p2.y <= (p1.y + esp)) ) )
  {
    res = 1;
  }
  return res;
}
