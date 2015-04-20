#include <stdio.h>
#include <math.h>
#include "graph.h"
#include "calcul.h"

pos_t compute_position(graph_t* G,int idm,double t){
  pos_t pos;
  double v0x = G->moves[idm].vx;
  double v0y = G->moves[idm].vy;
  double x0 = G->moves[idm].p.x; /*Faire une fonction get_pos ?*/
  double y0 = G->moves[idm].p.y;
  
  set_pos(&pos,x0+t*v0x,y0+t*v0y);
  return pos;
}


double compute_interception(graph_t* G,pos_t* pos, int idm,int t){
  /*Variables liees a l'intercepteur */
  double v1 = G->inter_spd;
  double x1 = pos->x;
  double y1 = pos->y;
  
  /*Variables liees au mobile traite*/
  double v0x = G->moves[idm].vx;
  double v0y = G->moves[idm].vy;
  pos_t tmp = compute_position(G,idm,t);
  double x0 = tmp.x;
  double y0 = tmp.y;
  
  /*Variable liees a l'obtention de l'angle alpha */
  double a = y0-y1;
  double b = x1-x0;
  double c = (a*v0x+b*v0y)/v1;
  double alpha = compute_alpha(a,b,c); /*Nombre compris entre -Pi et Pi ou Pi à priori vu la résolution de l'équation*/
  
  /*Variables liees a l'obtention du temps d'interception*/
  double t1 =  -1;
  double t2;
  double esp = 0.001; /*précision qu'on accorde à la différence entre t1 et t2*/
  
  
  if (alpha != 42) /* Code d'erreur (manque d'inspiration) */
  {
    t1 =  -b/(-v0x+v1*cos(alpha));
    t2 =  a/(-v0y+v1*sin(alpha));
    if (t1 < 0 || t2 < 0 || !((t1-esp) <= t2 && t2 <= (t1+esp))) /*Il est pas possible d'intercepter le mobile avec une date negative*/
    {
      t1 = -1 ; 
    }
  }
  
  return t1;
}

double compute_alpha(double a, double b, double c){
  double res = 42;
  double pi = 4*atan(1.0); /* M_PI n'a pas fonctionne ..*/
  double a2 = pow(a,2);
  double b2 = pow(b,2);
  double c2 = pow(c,2);
  
  if ((c+a)== 0)
  {
    res = pi;
    if ( b != 0 && (a2+b2) != 0)
    {
      res = -2*atan(a/b);
    }
  }
  else
  {
    if ((-b*sqrt(a2+b2-c2)+a2+a*c+b2) != 0)
    {
      res = 2*atan((b-sqrt(a2+b2-c2))/(a+c));
    }
    if ((b*sqrt(a2+b2-c2)+a2+a*c+b2) != 0)
    {
      res = 2*atan((sqrt(a2+b2-c2)+b)/(a+c));
    }
  }
  return res;
}
