#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graph.h"
#include "position.h"
#include "calcul.h"
#include "sln.h"


/*Cree et initialise la structure de solution avec une taille taille*/
sln_t* create_sln(int taille){
  sln_t* sol = (sln_t*) malloc(1*sizeof(sln_t));
  int i;
  
  if (sol)
  {
    sol->tsln = (csln_t*) malloc(taille*sizeof(csln_t));
    if (sol->tsln)
    {
      sol->first = -1;
      sol->last = -1;
      for (i = 0; i < taille; i++) /*Initialisation des champs*/
      {
	sol->tsln[i].id_mob = i;
	sol->tsln[i].next = -1;
	sol->tsln[i].prec = -1;
      }
    }else
    {
      free(sol);
    }
  }
  return sol;
}

/*Ajoute les informations du mobile en fin de solution*/
void add_mobile(sln_t* sol,int id,pos_t pos, double t){
  
  /*Ajout des donnees*/
  sol->tsln[id].t = t;
  set_pos(&(sol->tsln[id].pos),pos.x,pos.y);
  
  if (sol->first == -1) /*Si la liste chaine est vide*/
  {
    sol->first = id;
    sol->last = id;
  }else
  {
    /*Mise a jour du chainage*/
    sol->tsln[sol->last].next = id;
    sol->tsln[id].prec = sol->last;
    sol->last = id;
  }
}

/*Affiche les informations de la solution dans l'ordre de la liste chainee*/
void print_sln(sln_t* sol){
  int cour = sol->first;
  
  while (cour != -1)
  {
    printf("Id mobile : %d\nPosition d'interception : (%f,%f)\nDate d'interception : %f\n",sol->tsln[cour].id_mob,sol->tsln[cour].pos.x,sol->tsln[cour].pos.y,sol->tsln[cour].t);
    cour = sol->tsln[cour].next;
  }
}

/*Renvoie la date finale (donc le temps requis) pour intercepter l'ensemble des mobiles contenus dans la solution*/
double get_ftime(sln_t* sol){
  double ftime = -1;
  
  if (sol->last != -1)
  {
    ftime = sol->tsln[sol->last].t;
  }
  return ftime;
}

/*Libere la structure et met le pointeur à NULL*/
void free_sln(sln_t** sol){
  free((*sol)->tsln);
  free((*sol));
  *sol = NULL;
}

/*Fonction de test unitaire de toutes les fonctions de solutions*/
void test_soln(){
  sln_t* sol = create_sln(3);
  pos_t pos1;
  pos_t pos2;
  set_pos(&pos1,1,2);
  set_pos(&pos2,3,4);
  add_mobile(sol,0,pos1,10);
  add_mobile(sol,2,pos2,20);
  print_sln(sol);
  printf("La date finale d'interception est : %f\n",get_ftime(sol));
  free_sln(&sol);
}

