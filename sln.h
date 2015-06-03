#ifndef __SOLUTION_H__
#define __SOLUTION_H__

#include "graph.h"
#include "position.h"

typedef struct cellule {
  int id_mob; /*Identifiant du mobile*/
  pos_t pos; /*Position ou le mobile a ete intercepte*/
  double t; /*Date a laquelle le mobile a ete intercepte*/
  int next; /*Indice de l'élément suivant, -1 par defaut*/
  int prec; /*Indice de l'élément précédent, -1 par defaut*/
} csln_t;

typedef struct sln {
  int first; /*Indice du premier mobile intercepte*/
  int last; /*Indice du dernier mobile intercepte*/
  csln_t* tsln; /*Tableau de solution*/
} sln_t;

/*Cree et initialise la structure de solution avec une taille taille*/
sln_t* create_sln(int taille);

/*Ajoute les informations du mobile en fin de solution*/
void add_mobile(sln_t* sol,int id,pos_t pos, double t);

/*Affiche les informations de la solution dans l'ordre de la liste chainee*/
void print_sln(sln_t* sol);

/*Renvoie la date finale (donc le temps requis) pour intercepter l'ensemble des mobiles contenus dans la solution*/
double get_ftime(sln_t* sol);

/*Libere la structure et met le pointeur à NULL*/
void free_sln(sln_t** sol);

/*Fonction de test unitaire de toutes les fonctions de solutions*/
void test_soln();

#endif
