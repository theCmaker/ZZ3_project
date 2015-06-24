#ifndef __SOLUTION_H__
#define __SOLUTION_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "position.h"
#include "graph.h"
#include "calcul.h"

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

/* Heuristique H0: le plus rapide a intercepter */
int heuristique_plus_rapide(graph_t *, char *);

/* Heuristique H1: sequence */
int heuristique_sequence(graph_t *, char*);

/*Fonction de test unitaire de toutes les fonctions de solutions*/
void test_soln();

/*Fonction de generation de graphique TikZ*/
void tikz_output(char *, int, sln_t *, graph_t *);

/*Fonction de generation de tableau LaTeX*/
void tex_table_output(char *, int, sln_t *);

/*Fonction de generation de graphique de comparaison TikZ*/
void tikz_compare(char *, int, sln_t *, graph_t *);
#endif
