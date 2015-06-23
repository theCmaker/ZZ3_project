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
  } else {
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

/* Heuristique H0: le plus rapide a intercepter */
int heuristique_plus_rapide(graph_t *G) {
  int res;                /* Valeur de retour: nb de mobiles interceptes */
  sln_t *sol;             /* Solution */
  int nb_mobiles;         /* Nombre de mobiles restants */
  int idm, i;             /* Indice du mobile a intercepter */
  csln_t *mobcour;
  
  double temps;           /* Duree d'interception totale */
  double tps_min = 0;     /* Duree d'interception minimum */
  double tps;             /* Duree d'interception calculee */

  double alpha;           /* Angle pour interception */
  double alpha_min;       /* Angle pour interception la plus rapide */

  pos_t pos_intercepteur; /* Position d'interception */

  /* Initialisation */
  nb_mobiles = G->mob_nb;
  sol = create_sln(nb_mobiles);
  if (sol) {                 /* Solution créée */
    cpy_pos(G->dep_pos[0],&pos_intercepteur); /* Position initiale intercepteur */
    temps = 0;                                /* Temps initial */

    while (nb_mobiles > 0 && finite(tps_min)) { /* Il reste des mobiles a intercepter */
      i = 0;
      tps_min = 1.0/0.0;     /* Temps defini a +inf, permet de trouver un minimum plus facilement */
      
      while (i < G->mob_nb) { /* Parcours de tous les mobiles restants */
        mobcour = &(sol->tsln[i]); /* Mobile courant */

        if (i != sol->first && mobcour->next == -1 && mobcour->prec == -1) { /* Mobile non intercepte */

          tps = compute_interception(G,&pos_intercepteur,i,temps,&alpha); /* Calcul temps interception */
          if (tps >= 0 && tps < tps_min) {  /* Interception possible et temps inferieur */
            tps_min = tps;      /* Sauvegarde temps minimum */
            idm = i;            /* Sauvegarde indice du min */
            alpha_min = alpha;  /* Sauvegarde angle */
          }
        }
        ++i; /* Mobile suivant */
      }
      if (finite(tps_min)) { /* Mobile interceptable */
        pos_intercepteur = compute_pos_intercep(G,alpha_min,pos_intercepteur,tps_min); /* Calcul position intercepteur */
        temps = temps + tps_min;                    /* Ajout du temps */
        add_mobile(sol,idm,pos_intercepteur,temps); /* Ajout a la solution */
        nb_mobiles--; /* Actualisation nb mobiles restants a traiter */
      }
    }
    printf("Rapport d'interception / Heuristique H0:\n");
    print_sln(sol); /* Affichage */
    free_sln(&sol); /* Libération */
    res = G->mob_nb - nb_mobiles;
  } else {
    res = -1;
  }
  return res;
}

/* Heuristique H1: sequence */
int heuristique_sequence(graph_t *G) {
  int res;                /* Valeur de retour: nb de mobiles interceptes */
  sln_t *sol;             /* Solution */
  int nb_mobiles = 0;     /* Nombre de mobiles interceptes */
  int i;                  /* Indice du mobile a intercepter */
  
  double temps;           /* Duree d'interception totale */
  double tps;             /* Duree d'interception calculee */

  double alpha;           /* Angle pour interception */

  pos_t pos_intercepteur; /* Position d'interception */

  /* Initialisation */
  sol = create_sln(G->mob_nb);
  if (sol) {                 /* Solution créée */
    cpy_pos(G->dep_pos[0],&pos_intercepteur); /* Position initiale intercepteur */
    temps = 0;                                /* Temps initial */

    for (i = 0; i < G->mob_nb; ++i) { /* Il reste des mobiles a intercepter */
      tps = compute_interception(G,&pos_intercepteur,i,temps,&alpha); /* Calcul temps interception */
      if (finite(tps) && tps >= 0) {  /* Interception possible et temps inferieur */
        pos_intercepteur = compute_pos_intercep(G,alpha,pos_intercepteur,tps); /* Calcul position intercepteur */
        temps += tps;                               /* Ajout du temps */
        add_mobile(sol,i,pos_intercepteur,temps); /* Ajout a la solution */
        nb_mobiles++;
      }
    }
    printf("Rapport d'interception / Heuristique H1:\n");
    print_sln(sol); /* Affichage */
    free_sln(&sol); /* Libération */
    res = nb_mobiles;
  } else {
    res = -1;
  }
  return res;
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

