#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graph.h"
#include "position.h"
#include "calcul.h"
#include "solution.h"


sol_t* create_sol(int taille){
  sol_t* sol = (sol_t*) malloc(1*sizeof(sol_t));
  if (sol)
  {
    sol->tsol = (csol_t*) malloc(taille*sizeof(csol_t));
    sol->n = 0;
  }
  return sol;
}

void add_mob(sol_t* sol,int id,pos_t pos, double t){
  int i = sol->n ; /*Juste pour que ce soit plus lisible..*/
  
  sol->tsol[i].id_mob = id;
  sol->tsol[i].t = t;
  set_pos(&(sol->tsol[i].pos),pos.x,pos.y);
  sol->n ++;
}

void print_sol(sol_t* sol){
  int i = 0;
  for (i = 0; i < sol->n; i++)
  {
    printf("Id mobile : %d\nPosition d'interception : (%f,%f)\nDate d'interception : %f\n",sol->tsol[i].id_mob,sol->tsol[i].pos.x,sol->tsol[i].pos.y,sol->tsol[i].t);
  }
}

double get_time(sol_t* sol){
  int i = sol->n - 1 ; /*Pareil pour la lisibilite*/
  return sol->tsol[i].t ;
}

void free_sol(sol_t** sol){
  free((*sol)->tsol);
  free((*sol));
  *sol = NULL;
}


void test_solution(){
  sol_t* sol = create_sol(3);
  pos_t pos1;
  pos_t pos2;
  set_pos(&pos1,1,2);
  set_pos(&pos2,3,4);
  add_mob(sol,1,pos1,10);
  add_mob(sol,2,pos2,20);
  print_sol(sol);
  printf("La date finale d'interception est : %f\n",get_time(sol));
  free_sol(&sol);
}
