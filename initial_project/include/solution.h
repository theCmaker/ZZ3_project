#ifndef __SOLUTION_H__
#define __SOLUTION_H__

#include "graph.h"
#include "position.h"

  typedef struct cellule {
    int id_mob; /*Identifiant du mobile*/
    pos_t pos; /*Position ou le mobile a ete intercepte*/
    double t; /*Date a laquelle le mobile a ete intercepte*/
  } csol_t;


  typedef struct solution {
    int n; /*Nombre de mobile interceptes*/
    csol_t* tsol; /*Tableau contenant les mobiles interceptes avec des informations en plus*/
  } sol_t;

/*Cree et initialise la structure de solution avec une taille taille*/
sol_t* create_sol(int taille);

/*Ajoute les informations du mobile en fin de tableau de solution*/
void add_mob(sol_t* sol,int id,pos_t pos, double t);

/*Affiche les informations de la solution*/
void print_sol(sol_t* sol);

/*Renvoie la date finale (donc le temps requis) pour intercepter l'ensemble des mobiles contenus dans la structure*/
double get_time(sol_t* sol);

/*Libere la structure et met le pointeur à NULL*/
void free_sol(sol_t** sol);

/*Fonction de test unitaire de toutes les fonctions de solutions*/
void test_solution();

#endif
