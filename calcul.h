#ifndef __CALCUL_H__
#define __CALCUL_H__

/*Calcul la position du mobile idm a la date t du graphe G*/
pos_t compute_position(graph_t* G,int idm,double t);

/*Calcul le temps requis pour intercepter le mobile idm A PARTIR de la date t */
double compute_interception(graph_t* G, pos_t* pos, int idm,int t);

/*Calcul la solution  de l'équation a*cos(x)+b*sin(x) = c pour x inconnu*/
double compute_alpha(double a, double b, double c);


/*Calcul la distance entre deux mobiles a une date t
double compute_distance(graph_t* G,int id1,int id2,double t); utilite ?*/

#endif
