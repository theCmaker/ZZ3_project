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
int heuristique_plus_rapide(graph_t *G, char *logdir) {
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
    printf("\nRapport d'interception / Heuristique H0:\n");
    print_sln(sol); /* Affichage */
    if (logdir[0] != '\0') {
      tikz_output(logdir,0,sol,G);
      tex_table_output(logdir,0,sol);
      tikz_compare(logdir,0,sol,G);
    }
    free_sln(&sol); /* Libération */
    res = G->mob_nb - nb_mobiles;
  } else {
    res = -1;
  }
  return res;
}

/* Heuristique H1: sequence */
int heuristique_sequence(graph_t *G, char *logdir) {
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
    printf("\nRapport d'interception / Heuristique H1:\n");
    print_sln(sol); /* Affichage */
    if (logdir[0] != '\0') {
      tikz_output(logdir,1,sol,G);
      tex_table_output(logdir,1,sol);
      tikz_compare(logdir,1,sol,G);
    }
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

void tex_table_output(char *logdir, int h, sln_t *sol) {
  char *file;
  FILE *f = NULL;
  int cour;

  file = (char*) malloc((strlen(logdir)+16)*sizeof(char));
  if (file) {
    if (logdir[strlen(logdir)-1] == '/') {
      sprintf(file,"%sh%d_table.tex",logdir,h);
    } else {
      sprintf(file,"%s/h%d_table.tex",logdir,h);
    }
    f = fopen(file, "w+");
    if (f) {
      fprintf(f, "\\begin{tabular}{|c|c|c|}\n");
      fprintf(f, "  \\hline\\textbf{\\No mobile} & \\textbf{Position interception} & \\textbf{Date interception (u.t.)} \\\\ \\hline \n");
      cour = sol->first;
      while (cour != -1) {
        fprintf(f,"  %d\t& $\\coord{%.3f}{%.3f}$\t & $%.4f$ \\\\ \\hline\n",sol->tsln[cour].id_mob,sol->tsln[cour].pos.x,sol->tsln[cour].pos.y,sol->tsln[cour].t);
        cour = sol->tsln[cour].next;
      }
      fprintf(f, "\\end{tabular}\n");
      fclose(f);
    } else {
      fprintf(stderr, "tex_table_output: output file error\n");
    }
    free(file);
  } else {
    fprintf(stderr, "tex_table_output: memory error\n");
  }
}

void get_bounds(sln_t *sol, graph_t *G, double *xmin, double *xmax, double *ymin, double* ymax) {
  int cour, i;
  cour = sol->first;
  while (cour != -1) {
    if (sol->tsln[cour].pos.x < *xmin) {
      *xmin = sol->tsln[cour].pos.x;
    } else if (sol->tsln[cour].pos.x > *xmax) {
      *xmax = sol->tsln[cour].pos.x;
    }
    if (sol->tsln[cour].pos.y < *ymin) {
      *ymin = sol->tsln[cour].pos.y;
    } else if (sol->tsln[cour].pos.y > *ymax) {
      *ymax = sol->tsln[cour].pos.y;
    }
    cour = sol->tsln[cour].next;
  }
  for (i = 0; i < G->mob_nb; ++i) {
    if (G->moves[i].p.x < *xmin) {
      *xmin = G->moves[i].p.x;
    } else if (G->moves[i].p.x > *xmax) {
      *xmax = G->moves[i].p.x;
    }
    if (G->moves[i].p.y < *ymin) {
      *ymin = G->moves[i].p.y;
    } else if (G->moves[i].p.y > *ymax) {
      *ymax = G->moves[i].p.y;
    }
  }
}

void tikz_output(char *logdir, int h, sln_t *sol, graph_t *G) {
  char *file;
  FILE *f = NULL;
  int cour;
  int i;
  double xmin, xmax, ymin, ymax;
  pos_t intercepteur;

  file = (char*) malloc((strlen(logdir)+16)*sizeof(char));
  if (file) {
    /* Creation chemin de fichier */
    if (logdir[strlen(logdir)-1] == '/') {  /* avec slash */
      sprintf(file,"%sh%d_graph.tex",logdir,h);
    } else {                                /* sans slash */
      sprintf(file,"%s/h%d_graph.tex",logdir,h);
    }

    /* Ouverture fichier */
    f = fopen(file, "w+");
    if (f) {
      cpy_pos(G->dep_pos[0],&intercepteur); /* Position initiale intercepteur */
      xmin = intercepteur.x; ymin = intercepteur.y; xmax = intercepteur.x; ymax = intercepteur.y;
      get_bounds(sol, G, &xmin, &xmax, &ymin, &ymax);

      fprintf(f, "\\draw[grided,step=1.0,thin] (%f,%f) grid (%f,%f);\n",floor(xmin),floor(ymin),xmax,ymax); /* Grille */
      fprintf(f,"\\draw (%f,0) -- coordinate (x axis mid) (%f,0);\n",floor(xmin), xmax); /* Abscisses */
      fprintf(f,"\\draw (0,%f) -- coordinate (y axis mid) (0,%f);\n",floor(ymin), ymax); /* Ordonnees */
      fprintf(f, "\\foreach \\x in {%d,...,%d}\n  \\draw (\\x,1pt) -- (\\x,-3pt) node[anchor=north] {\\x};\n",(int)floor(xmin), (int)floor(xmax)); /* Etiquettes abscisses */
      fprintf(f, "\\foreach \\y in {%d,...,%d}\n  \\draw (1pt,\\y) -- (-3pt,\\y) node[anchor=east] {\\y};\n",(int)floor(ymin), (int)floor(ymax));  /* Etiquettes ordonnees */

      fprintf(f,"\\node[interceptor] (I%d) at (%f,%f) {\\interceptor};\n",0,G->dep_pos[0].x,G->dep_pos[0].y); /* Position initiale intercepteur */

      /* Positions initiales, vitesses, et noms des mobiles */
      for (i = 0; i < G->mob_nb; ++i) {
        fprintf(f,"\\node[mobile,anchor=center] (M%d) at (%f,%f) {\\mobile};\n",i,G->moves[i].p.x,G->moves[i].p.y);
        fprintf(f,"\\node[mobile] at (M%d.south east) {$M_%d$};\n",i,i);
        if (G->moves[i].vx != 0 || G->moves[i].vy != 0) {
          fprintf(f,"\\draw[speed] (M%d.center) -- ($ (M%d.center) + (%f,%f) $);\n",i,i,G->moves[i].vx,G->moves[i].vy);
        }
      }

      /* Tracé de la trajectoire de l'intercepteur */
      cour = sol->first;
      while (cour != -1) {
        if (G->moves[sol->tsln[cour].id_mob].vx != 0 || G->moves[sol->tsln[cour].id_mob].vy != 0) {
          fprintf(f,"\\draw[camino] (M%d) -- (%f,%f);\n",sol->tsln[cour].id_mob,sol->tsln[cour].pos.x,sol->tsln[cour].pos.y);
        }
        fprintf(f,"\\draw[interceptor] (%f,%f) -- (%f,%f);\n",intercepteur.x,intercepteur.y,sol->tsln[cour].pos.x,sol->tsln[cour].pos.y);
        fprintf(f,"\\node[interceptor] at (%f,%f) {\\mobile};\n",sol->tsln[cour].pos.x,sol->tsln[cour].pos.y);
        fprintf(f, "\\node[caught] at (M%d) {\\mobile};\n", sol->tsln[cour].id_mob);
        /*fprintf(f,"Id mobile : %d\nPosition d'interception : (%f,%f)\nDate d'interception : %f\n",sol->tsln[cour].id_mob,sol->tsln[cour].pos.x,sol->tsln[cour].pos.y,sol->tsln[cour].t);*/
        cpy_pos(sol->tsln[cour].pos,&intercepteur);
        cour = sol->tsln[cour].next;
      }

      /* Date d'arrivée */
      fprintf(f, "\\draw[interceptor] (%f,%f) node[anchor=south %s] {$t=%.3f \\text{ u.t.}$};\n", intercepteur.x,intercepteur.y,(intercepteur.x - 2 < xmin)?"west":"east",get_ftime(sol));
      fclose(f);
    } else {
      fprintf(stderr, "tikz_output: output file error\n");
    }
    free(file);
  } else {
    fprintf(stderr, "tikz_output: memory error\n");
  }
}

void tikz_compare(char *logdir, int h, sln_t *sol, graph_t *G) {
  char *file;
  FILE *f = NULL;
  int cour, i;
  double t_prec;

  file = (char*) malloc((strlen(logdir)+16)*sizeof(char));
  if (file) {
    if (logdir[strlen(logdir)-1] == '/') {
      sprintf(file,"%sh_comp.tex",logdir);
    } else {
      sprintf(file,"%s/h_comp.tex",logdir);
    }
    f = fopen(file, "a+");
    if (f) {
      if (h == 0) {
        fprintf(f,"\\draw (0,0) -- coordinate (x axis mid) (%d,0);\n", G->mob_nb); /* Abscisses */
        fprintf(f,"\\foreach \\x in {0,...,%d}\n  \\draw (\\x,1pt) -- (\\x,-3pt) node[anchor=north] {\\x};\n", G->mob_nb); /* Etiquettes abscisses */
        fprintf(f,"\\draw (0,0) -- coordinate (y axis mid) (0,%f);\n", ceil(get_ftime(sol))); /* Ordonnees */
        fprintf(f,"\\foreach \\y in {0,%d,...,%d}\n  \\draw (1pt,\\y) -- (-3pt,\\y) node[anchor=east] {\\y};\n", (((int)round(ceil(get_ftime(sol))/10))>0)?(int)round(ceil(get_ftime(sol))/10):1, (int)ceil(get_ftime(sol)));  /* Etiquettes ordonnees */
        fprintf(f,"\\draw (%f,-2) node[anchor=north] {Nombre de mobiles interceptés};\n", ((double) G->mob_nb)/2.);
        fprintf(f,"\\draw (-0.75,%f) node[rotate=90,anchor=south] {Temps nécessaire (u.t)};\n", ceil(get_ftime(sol))/2.);
      }
      fprintf(f,"\\draw[grided,step=1.0,thin] (0,0) grid (%d,%f);\n",G->mob_nb,ceil(get_ftime(sol))); /* Grille */

      cour = sol->first;
      i = 1;
      t_prec = 0;
      fprintf(f,"\\node[h%d] at (0,0) {\\cross};\n",h);
      while (cour != -1) {
        fprintf(f,"\\draw[h%d] (%d,%f) -- (%d,%f);\n",h,i-1,t_prec,i,sol->tsln[cour].t);
        fprintf(f,"\\node[h%d] at(%d,%f) {\\cross};\n",h,i,sol->tsln[cour].t);
        t_prec = sol->tsln[cour].t;
        ++i;
        cour = sol->tsln[cour].next;
      }
      fclose(f);
    } else {
      fprintf(stderr, "tikz_compare: output file error\n");
    }
    free(file);
  } else {
    fprintf(stderr, "tikz_compare: memory error\n");
  }
}