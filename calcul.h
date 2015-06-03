#ifndef __CALCUL_H__
#define __CALCUL_H__

#define DEBUG

#ifndef DEBUG
#define AFFICHER(X)
#endif

#ifdef DEBUG
#define AFFICHER(X) printf("La variable "#X" vaut %f \n",X);
#endif

/*Calcul la position du mobile idm a la date t du graphe G*/
pos_t compute_position(graph_t* G,int idm,double t);

/*Calcul la position de l'intercepteur a partir d'un angle alpha, d'une position initiale et d'une date t*/
pos_t compute_pos_intercep(graph_t* G, double alpha, pos_t pos, double t);

/*Calcul le temps requis pour intercepter le mobile idm A PARTIR de la date t  et de la position pos*/
double compute_interception(graph_t* G, pos_t* pos, int idm,int t, double* alpha);

/*Calcul la solution  de l'équation a*cos(x)+b*sin(x) = c pour x inconnu*/
double compute_alpha(double a, double b, double c);

/*Fonction de test unitaire de l'interception d'un mobile*/
void test_interception(graph_t* G, pos_t* pos, int idm,double t);


/*Calcul la distance entre deux mobiles a une date t
double compute_distance(graph_t* G,int id1,int id2,double t); utilite ?*/

#endif
