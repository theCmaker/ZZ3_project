#include "calcul.h"


pos_t compute_position(graph_t* G,int idm,double t){
  pos_t pos;
  double x0,y0;
  double v0x = G->moves[idm].vx;
  double v0y = G->moves[idm].vy;
  get_pos(G->moves[idm].p,&x0,&y0);
  set_pos(&pos,x0+t*v0x,y0+t*v0y);
  return pos;
}


double compute_interception(graph_t* G,pos_t* pos, int idm,int t, double* alpha){
  /*Variables liees a l'intercepteur */
  double v1 = G->inter_spd;
  double x1,y1;
  get_pos(*pos,&x1,&y1);
  
  /*Variables liees au mobile traite*/
  double v0x = G->moves[idm].vx;
  double v0y = G->moves[idm].vy;
  pos_t tmp = compute_position(G,idm,t);
  double x0,y0;
  get_pos(tmp,&x0,&y0);

  /*Variable liees a l'obtention de l'angle alpha */
  double a = y0-y1;
  double b = x1-x0;
  double c = (a*v0x+b*v0y)/v1;
  *alpha = compute_alpha(a,b,c); /*Nombre compris entre -Pi et Pi ou Pi à priori vu la résolution de l'équation*/

  /*Variables liees a l'obtention du temps d'interception*/
  double t1, t2;
  double esp = 0.001;
  double tres = -1;
  pos_t pos1, pos2, posm; /*Calcul de position pour verifier l'equation*/
  int ind1 = 0; /*Indicateurs pour savoir si l'interception fonctionne*/
  
  /************ AJOUT PILOU *************/
  /* Suggestion: tester si les positions ne sont pas egales avant de calculer un angle. c'est rare mais ca peut arriver, notamment en faisant des tests sans faire attention (mobiles qui convergent) */
  if (is_equal(*pos,tmp,esp)) {
    return 0;
  }
  /************ FIN AJOUT ***************/
  
  /*Affichage des variables uniquement si le mode DEBUG est active*/
  AFFICHER(v1);
  AFFICHER(x1);
  AFFICHER(y1);
  AFFICHER(v0x);
  AFFICHER(v0y);
  AFFICHER(x0);
  AFFICHER(y0);
  AFFICHER(a);
  AFFICHER(b);
  AFFICHER(c);
  AFFICHER(*alpha);
  
  
  if (*alpha != 42) /* Code d'erreur (manque d'inspiration) */
  {
    t1 =  -b/(-v0x+v1*cos(*alpha));
    t2 =  a/(-v0y+v1*sin(*alpha));
    AFFICHER(t1);
    AFFICHER(t2);
    
    /*Pour choisir la bonne date, il suffit de prendre celle qui marche et si les deux fonctionnent, on prend la date la plus faible*/
    
    /* On gere d'abord la premiere date*/
    if (finite(t1) && t1 > 0) /*Creer un warning car finite n'existe pas en ansi*/
    {
      if (is_equal(pos1,posm,esp)) /*Fonction d'egalite de deux position ?*/
      {
      	tres = t1;
      	ind1 = 1;
      }
    }
    
    /*Puis la 2eme*/
    if (finite(t2) && t2 > 0)
    {
      pos2 = compute_pos_intercep(G,*alpha,*pos,t2);
      posm = compute_position(G,idm,t2+t);
      
      AFFICHER(pos2.x)
      AFFICHER(pos2.y)
      AFFICHER(posm.x)
      AFFICHER(posm.y)
      
      if (is_equal(pos2,posm,esp)) /*Fonction d'egalite de deux position ?*/
      {
      	tres = t2;
      	if (ind1 && t2 > t1) /*On regarde si t1 a marche aussi et s'il etait plus faible, on le choisit*/
      	{
      	  tres = t1;
      	}
      }
    }
  }
    
  return tres;
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

void test_interception(graph_t* G, pos_t* pos, int idm,double t){
  double date, alpha;
  pos_t position;
  date = compute_interception(G,pos,idm,t,&alpha);
  printf("Date vaut : %f\n\n",date);
  position = compute_pos_intercep(G,alpha,*pos,date);
  printf("Verification : \n");
  printf("La position de l'intercepteur a la date t = %f est %f %f\n",date,position.x,position.y);
  position = compute_position(G,0,date);
  printf("La position du mobile %d a la date t = %f est %f %f\n",idm,date,position.x,position.y);
}

pos_t compute_pos_intercep(graph_t* G, double alpha, pos_t pos, double t){
  pos_t pos_res;
  AFFICHER(pos.x);
  AFFICHER(cos(alpha));
  AFFICHER(G->inter_spd);
  AFFICHER(t);
  set_pos(&pos_res,pos.x + t*cos(alpha)*G->inter_spd, pos.y + t*sin(alpha)*G->inter_spd);
  return pos_res;
}







