#include <stdio.h>
#include <stdlib.h>
#include "labyrinthAPI.h"
#include "clientAPI.h"


typedef struct {   // une structure qui permet d'y placer la taille du labyrinthe reçu par getLabyrinthe
	int sizeX;
	int sizeY;
} size_lab;


// structure qui permet stocker une tuile
typedef struct { // 1 pour un Mur et 0 pour une issue 
	int tuileN; // cote nord
	int tuileE; // cote est
	int tuileS; // cote sud
	int tuileW; // cote ouest
	int tuileT; // numero du tresor
} t_tuile;

//structure qui stocke la position du joueur et le numéro du trésor
typedef struct {
	int posX;
	int posY;
	int tresor;
} joueur_stat;

// Fonction qui permet de mettre la suite de nombre de laby (remplit par getlabyrinthe) dans un tableau de tuile (chaque tuile est un t_tuile)
void remplit_tuile_tab (t_tuile* tuile_lab,int* laby,size_lab* taille){
	for (int j=0;j<(taille->sizeX)*(taille->sizeY); j++) { // on remplit notre tableau
		tuile_lab[j].tuileN=laby[j*5];
		tuile_lab[j].tuileE=laby[(j*5)+1];  
		tuile_lab[j].tuileS=laby[(j*5)+2];
		tuile_lab[j].tuileW=laby[(j*5)+3];
		tuile_lab[j].tuileT=laby[(j*5)+4];
		}
}	

void affiche_tuile_tab (t_tuile* tuile_lab,int* laby,size_lab* taille){	
	for (int i=0; i<(taille->sizeY); i++){   // équivalent de l'option debug mais avec notre représentation
		for (int j=0; j<(taille->sizeX); j++){
			printf("%d",tuile_lab[(i*(taille->sizeX)+j)].tuileN); 
			printf("%d",tuile_lab[(i*(taille->sizeX)+j)].tuileE);
			printf("%d",tuile_lab[(i*(taille->sizeX)+j)].tuileS);
			printf("%d",tuile_lab[(i*(taille->sizeX)+j)].tuileW);
			printf("%d ",tuile_lab[(i*(taille->sizeX)+j)].tuileT);	
		}
		printf("\n");
	} 
}

/* on remplit un tableau où chaque tuile est représenté par un ensemble de 9 cases 
_ _ _  les case dans les coins sont par défault des obstables et les case du haut centre, bas centre, droite gauche
_ _ _	sont des obstacles en fontions des tuiles_lab qui contient les tuiles
_ _ _	
*/

// cette fonction a pour but d'avoir un tableau où chaque mure est défini par un -1 et chaque case vide par un 0
//elle va permet de réutiliser les fonctions créer dans le tp labyrinthe sans avoir a changer les programmes du tp labyrinthe
 void remplit_tab_affiche (int tab_affiche[39][39],size_lab* taille,t_tuile* tuile_lab ) {
	
	
	for (int i=0; i<3*(taille->sizeY) ; i++){
		for (int j=0; j<3*(taille->sizeX) ; j++){
			tab_affiche[i][j]=0;
		}
	}
		
	/*for (int i=0; i<3*(taille->sizeY) ; i++){  //affichage du tableau
		for (int j=0; j<3*(taille->sizeX) ; j++){
			printf("%d ",tab_affiche[i][j]);
		}
		printf("\n");
	}*/
	
	
	
	int a=0;
	for (int i=0; i<(taille->sizeY);i++){
		for (int j=0; j<(taille->sizeX) ; j++){
			tab_affiche[3*i][3*j]=-1; //coin haut gauche
			tab_affiche[3*i][3*j+2]=-1;//coin haut droite
			tab_affiche[3*i+2][3*j]=-1;// coin bas gauche
			tab_affiche[3*i+2][3*j+2]=-1;// coin bas doite
			
			if (tuile_lab[a].tuileN==1){
				tab_affiche[3*i][3*j+1]=-1;
			}
			if (tuile_lab[a].tuileE==1){
				tab_affiche[3*i+1][3*j+2]=-1;
			}
			if (tuile_lab[a].tuileS==1){
				tab_affiche[3*i+2][3*j+1]=-1;
			}
			if (tuile_lab[a].tuileW==1){
				tab_affiche[3*i+1][3*j]=-1;
			}
			a++;				
		}
	}
	 
	for (int i=0; i<3*(taille->sizeY) ; i++){  // petit affichage ave des # pour mur et - pour un vide, donne une représentation proche de printlabyrinthe mais à partie de notre propre représentation
		for (int j=0; j<3*(taille->sizeX) ; j++){
			if(tab_affiche[i][j]==-1){
				printf("# ");
			}
			if(tab_affiche[i][j]==0){
				printf("- ");
			}

		}
		printf ("\n");
	}

}

int main (void) {

	
	size_lab *taille; //structure contenant la taille du laby
	taille= malloc(2*sizeof(int)); //allocation de la taille du lab
	

	char labyrinthNom[50];
	connectToServer("172.105.76.204",1234,"Yeogeuch");
	waitForLabyrinth("TRAINING DONTMOVE timeout=1000 display=debug margin=0 start=1 seed=0xecba1d  ",labyrinthNom,&(taille->sizeX),&(taille->sizeY));;
	
	//seed=0x000002

	
	t_tuile *joueur_stat_moi;
	joueur_stat_moi=malloc(3*sizeof(int));
	
	t_tuile *joueur_stat_adv;
	joueur_stat_adv=malloc(3*sizeof(int));
	
	t_tuile *Xtra_tuile;
	Xtra_tuile=malloc(5*sizeof(int));
	
	int* laby;
	laby=malloc( ((taille->sizeX)*(taille->sizeY)*5)*sizeof(int) );
	
		
	int indic_joueur=getLabyrinth(laby,&(Xtra_tuile->tuileN),&(Xtra_tuile->tuileE),&(Xtra_tuile->tuileS),&(Xtra_tuile->tuileW),&(Xtra_tuile->tuileT));
	
	t_tuile *tuile_lab; //tableau contenant l'ensemble des tuiles du plateau
	tuile_lab=malloc( (taille->sizeX)*(taille->sizeY)*sizeof(t_tuile) ); 
	
	// l'etat des joueur : NORMAL_MOVE = 0,WINNING_MOVE = 1,LOOSING_MOVE = -1
	t_return_code stat_moi=0;   
	t_return_code stat_adversaire=0;
	// declaration des variable type t_move (mien + adversaire)
	t_move *mouv_moi;
	t_move *mouv_adversaire;
	
	remplit_tuile_tab (tuile_lab,laby,taille);  
	printLabyrinth();
	
	int tab_affiche[39][39]; //  tab_Affichage: un tableau de 39*39 (taille maximale de labyrinthe) cf fontion remplit_tab_affiche
	remplit_tab_affiche (tab_affiche,taille,tuile_lab);
	
	
// la boucle de jeu	
/*	while ((stat_moi==0)&&(stat_adversaire==0)){ 
		if (indic_joueur==1){ //affiche laby, MAJ stat, donne le tour au joueur;
			printLabyrinth();
			printf("c'est au tour du joueur adversaire de jouer,veuillez patientez...\n");
			stat_adversaire = getMove(mouv_adversaire);
			
			 printf(" %d %d %d %d %d\n",(mouv_adversaire->insert),(mouv_adversaire->number),(mouv_adversaire->rotation),(mouv_adversaire->x),(mouv_adversaire->y));
			
			indic_joueur--;
			
		}
		if (indic_joueur==0){
			printLabyrinth();
			printf("C'est a votre tour de jouer\n");
			printf ("cote endroi rotation x y\n");
		     scanf (" %d %d %d %d %d",&(mouv_moi->insert),&(mouv_moi->number),&(mouv_moi->rotation),&(mouv_moi->x),&(mouv_moi->y));
			stat_moi=sendMove(mouv_moi);
			indic_joueur++;
		}
		if (stat_moi==1){
			printf("Fin de partie: vous avez gagne\n");
		}
		if (stat_moi==-1){
			printf("Fin de partie: vous avez perdu\n");
		}	
		if (stat_adversaire==1){
			printf("Fin de partie:l'adversaire a gagne\n");
		}
		if (stat_adversaire==-1){
		printf("Fin de partie: l'adversaire a perdu\n");
		}	
	
	}        
*/	

	
	printf("tailleX=%d et tailleY=%d\n",taille->sizeX, taille->sizeY);// affiche taille du labyrinthe
	printf("MurNord=%d MurEst=%d MurSud=%d MurOuest=%d tresor=%d\n",Xtra_tuile->tuileN,Xtra_tuile->tuileE,Xtra_tuile->tuileS,Xtra_tuile->tuileW,Xtra_tuile->tuileT);
	
////////////  Fontion mise à jour qui permet en théorie de mettre à jour notre représentration du tableau à jour	

//void mise_a_jour ( t_move *mouv,size_lab* taille,t_tuile* tuile_lab )
 
//	int rotation=mouv->rotation;
//	int cote = mouv->insert;
//	int endroit mouv->number;	
//	

/*	if(indic_joueur==1){
	joueur_stat_adv->posX=mouv->x;
	joueur_stat_adv->posX=mouv->y;
	}
	if(indic_joueur==0){
	joueur_stat_moi->posX=mouv->x;
	joueur_stat_moi->posX=mouv->y;
	}
*/
	
// permet d'effectuer la rotation de la tuile en trop avant de l'insérer	
/*	int tempN=Xtra_tuile->tuileN;
	int tempE=Xtra_tuile->tuileE;
	int tempS=Xtra_tuile->tuileS;
	int tempW=Xtra_tuile->tuileW;
	
	//scanf ("%d", &rotation);
	if (rotation==1){
		Xtra_tuile->tuileN=tempW;
		Xtra_tuile->tuileE=tempN;
		Xtra_tuile->tuileS=tempE;
		Xtra_tuile->tuileW=tempS;
	}
	if (rotation==2){
		Xtra_tuile->tuileN=tempS;
		Xtra_tuile->tuileE=tempW;
		Xtra_tuile->tuileS=tempN;
		Xtra_tuile->tuileW=tempE;
	}
	if (rotation==3){
		Xtra_tuile->tuileN=tempE;
		Xtra_tuile->tuileE=tempS;
		Xtra_tuile->tuileS=tempW;
		Xtra_tuile->tuileW=tempN;
	}
	*/	
	
	
	t_tuile *temp_tuile;
	temp_tuile=malloc(5*sizeof(int));
	
	int cote;
	int endroit;
	//scanf ("%d %d", &cote,&endroit);
	
	if (cote==0){ // pour insertion par le haut
		temp_tuile->tuileN=tuile_lab[endroit+(taille->sizeX)*((taille->sizeY)-1)].tuileN; // on copie la tuile qui va sortir
		temp_tuile->tuileE=tuile_lab[endroit+(taille->sizeX)*((taille->sizeY)-1)].tuileE; // du plateau
		temp_tuile->tuileS=tuile_lab[endroit+(taille->sizeX)*((taille->sizeY)-1)].tuileS;
		temp_tuile->tuileW=tuile_lab[endroit+(taille->sizeX)*((taille->sizeY)-1)].tuileW;
		temp_tuile->tuileT=tuile_lab[endroit+(taille->sizeX)*((taille->sizeY)-1)].tuileT;		
			
		for (int i= ((taille->sizeY)-1); i>0; i--){ // on décale les tuiles
		tuile_lab[endroit+((taille->sizeY)*(i))].tuileN=tuile_lab[endroit+((taille->sizeY)*(i-1))].tuileN;
		tuile_lab[endroit+((taille->sizeY)*(i))].tuileE=tuile_lab[endroit+((taille->sizeY)*(i-1))].tuileE;
		tuile_lab[endroit+((taille->sizeY)*(i))].tuileS=tuile_lab[endroit+((taille->sizeY)*(i-1))].tuileS;
		tuile_lab[endroit+((taille->sizeY)*(i))].tuileW=tuile_lab[endroit+((taille->sizeY)*(i-1))].tuileW;
		tuile_lab[endroit+((taille->sizeY)*(i))].tuileT=tuile_lab[endroit+((taille->sizeY)*(i-1))].tuileT;
		}
		
		tuile_lab[endroit].tuileN=Xtra_tuile->tuileN; // on insere la tuile 
		tuile_lab[endroit].tuileE=Xtra_tuile->tuileE;
		tuile_lab[endroit].tuileS=Xtra_tuile->tuileS;
		tuile_lab[endroit].tuileW=Xtra_tuile->tuileW;
		tuile_lab[endroit].tuileT=Xtra_tuile->tuileT;
		
		Xtra_tuile->tuileN=temp_tuile->tuileN; // la tuile qu'on a sortie devient la tuile en plus
		Xtra_tuile->tuileE=temp_tuile->tuileE;
		Xtra_tuile->tuileS=temp_tuile->tuileS;
		Xtra_tuile->tuileW=temp_tuile->tuileW;
		Xtra_tuile->tuileT=temp_tuile->tuileT;
	}

	if (cote==2){ // pour insertion par le bas
		temp_tuile->tuileN=tuile_lab[endroit].tuileN; // on copie la tuile qui va sortir
		temp_tuile->tuileE=tuile_lab[endroit].tuileE; // du plateau
		temp_tuile->tuileS=tuile_lab[endroit].tuileS;
		temp_tuile->tuileW=tuile_lab[endroit].tuileW;
		temp_tuile->tuileT=tuile_lab[endroit].tuileT;	
		
/*		printf("%d",temp_tuile->tuileN);
		printf("%d",temp_tuile->tuileE);
		printf("%d",temp_tuile->tuileS);
		printf("%d",temp_tuile->tuileW);
		printf("%d",temp_tuile->tuileT);	
*/

			
		for (int i= 0; i<(taille->sizeY)-1; i++){ // on décale les tuiles
		
//		printf("%d->",endroit+((taille->sizeY)*(i+1)));
//		printf("%d ",endroit+((taille->sizeY)*(i)));
		
		
		tuile_lab[endroit+((taille->sizeY)*(i))].tuileN=tuile_lab[endroit+((taille->sizeY)*(i+1))].tuileN;
		tuile_lab[endroit+((taille->sizeY)*(i))].tuileE=tuile_lab[endroit+((taille->sizeY)*(i+1))].tuileE;
		tuile_lab[endroit+((taille->sizeY)*(i))].tuileS=tuile_lab[endroit+((taille->sizeY)*(i+1))].tuileS;
		tuile_lab[endroit+((taille->sizeY)*(i))].tuileW=tuile_lab[endroit+((taille->sizeY)*(i+1))].tuileW;
		tuile_lab[endroit+((taille->sizeY)*(i))].tuileT=tuile_lab[endroit+((taille->sizeY)*(i+1))].tuileT;
		}
		
		tuile_lab[endroit+(taille->sizeX)*((taille->sizeY)-1)].tuileN=Xtra_tuile->tuileN; // on insere la tuile 
		tuile_lab[endroit+(taille->sizeX)*((taille->sizeY)-1)].tuileE=Xtra_tuile->tuileE;
		tuile_lab[endroit+(taille->sizeX)*((taille->sizeY)-1)].tuileS=Xtra_tuile->tuileS;
		tuile_lab[endroit+(taille->sizeX)*((taille->sizeY)-1)].tuileW=Xtra_tuile->tuileW;
		tuile_lab[endroit+(taille->sizeX)*((taille->sizeY)-1)].tuileT=Xtra_tuile->tuileT;
		
		Xtra_tuile->tuileN=temp_tuile->tuileN; // la tuile qu'on a sortie devient la tuile en plus
		Xtra_tuile->tuileE=temp_tuile->tuileE;
		Xtra_tuile->tuileS=temp_tuile->tuileS;
		Xtra_tuile->tuileW=temp_tuile->tuileW;
		Xtra_tuile->tuileT=temp_tuile->tuileT;
	}

	if (cote==3){ // pour insertion par la gauche
		temp_tuile->tuileN=tuile_lab[(taille->sizeY)*endroit+((taille->sizeX)-1)].tuileN; // on copie la tuile qui va sortir
		temp_tuile->tuileE=tuile_lab[(taille->sizeY)*endroit+((taille->sizeX)-1)].tuileE; // du plateau
		temp_tuile->tuileS=tuile_lab[(taille->sizeY)*endroit+((taille->sizeX)-1)].tuileS;
		temp_tuile->tuileW=tuile_lab[(taille->sizeY)*endroit+((taille->sizeX)-1)].tuileW;
		temp_tuile->tuileT=tuile_lab[(taille->sizeY)*endroit+((taille->sizeX)-1)].tuileT;	
		
/*		printf("%d",temp_tuile->tuileN);
		printf("%d",temp_tuile->tuileE);
		printf("%d",temp_tuile->tuileS);
		printf("%d",temp_tuile->tuileW);
		printf("%d",temp_tuile->tuileT);	
*/		

		for (int i= ((taille->sizeX)-1); i>0; i--){ // on décale les tuiles


		tuile_lab[endroit*((taille->sizeX)+(i))].tuileN=tuile_lab[endroit*((taille->sizeX)+(i-1))].tuileN;
		tuile_lab[endroit*((taille->sizeX)+(i))].tuileE=tuile_lab[endroit*((taille->sizeX)+(i-1))].tuileE;
		tuile_lab[endroit*((taille->sizeX)+(i))].tuileS=tuile_lab[endroit*((taille->sizeX)+(i-1))].tuileS;
		tuile_lab[endroit*((taille->sizeX)+(i))].tuileW=tuile_lab[endroit*((taille->sizeX)+(i-1))].tuileW;
		tuile_lab[endroit*((taille->sizeX)+(i))].tuileT=tuile_lab[endroit*((taille->sizeX)+(i-1))].tuileT;
		}
		
		tuile_lab[endroit*(taille->sizeX)].tuileN=Xtra_tuile->tuileN; // on insere la tuile 
		tuile_lab[endroit*(taille->sizeX)].tuileE=Xtra_tuile->tuileE;
		tuile_lab[endroit*(taille->sizeX)].tuileS=Xtra_tuile->tuileS;
		tuile_lab[endroit*(taille->sizeX)].tuileW=Xtra_tuile->tuileW;
		tuile_lab[endroit*(taille->sizeX)].tuileT=Xtra_tuile->tuileT;
		
		Xtra_tuile->tuileN=temp_tuile->tuileN; // la tuile qu'on a sortie devient la tuile en plus
		Xtra_tuile->tuileE=temp_tuile->tuileE;
		Xtra_tuile->tuileS=temp_tuile->tuileS;
		Xtra_tuile->tuileW=temp_tuile->tuileW;
		Xtra_tuile->tuileT=temp_tuile->tuileT;

	}
		
		
		if (cote==1){ // pour insertion par la droite
		temp_tuile->tuileN=tuile_lab[endroit*(taille->sizeX)].tuileN; // on copie la tuile qui va sortir
		temp_tuile->tuileE=tuile_lab[endroit*(taille->sizeX)].tuileE; // du plateau
		temp_tuile->tuileS=tuile_lab[endroit*(taille->sizeX)].tuileS;
		temp_tuile->tuileW=tuile_lab[endroit*(taille->sizeX)].tuileW;
		temp_tuile->tuileT=tuile_lab[endroit*(taille->sizeX)].tuileT;	
				

		for (int i= 0; i<(taille->sizeX)-1; i++){ // on décale les tuiles
		

		tuile_lab[endroit+((taille->sizeX)*(i))].tuileN=tuile_lab[endroit+((taille->sizeX)*(i+1))].tuileN;
		tuile_lab[endroit+((taille->sizeX)*(i))].tuileE=tuile_lab[endroit+((taille->sizeX)*(i+1))].tuileE;
		tuile_lab[endroit+((taille->sizeX)*(i))].tuileS=tuile_lab[endroit+((taille->sizeX)*(i+1))].tuileS;
		tuile_lab[endroit+((taille->sizeX)*(i))].tuileW=tuile_lab[endroit+((taille->sizeX)*(i+1))].tuileW;
		tuile_lab[endroit+((taille->sizeX)*(i))].tuileT=tuile_lab[endroit+((taille->sizeX)*(i+1))].tuileT;
		}
		
		tuile_lab[endroit*(taille->sizeY)+((taille->sizeX)-1)].tuileN=Xtra_tuile->tuileN; // on insere la tuile 
		tuile_lab[endroit*(taille->sizeY)+((taille->sizeX)-1)].tuileE=Xtra_tuile->tuileE;
		tuile_lab[endroit*(taille->sizeY)+((taille->sizeX)-1)].tuileS=Xtra_tuile->tuileS;
		tuile_lab[endroit*(taille->sizeY)+((taille->sizeX)-1)].tuileW=Xtra_tuile->tuileW;
		tuile_lab[endroit*(taille->sizeY)+((taille->sizeX)-1)].tuileT=Xtra_tuile->tuileT;
		
		Xtra_tuile->tuileN=temp_tuile->tuileN; // la tuile qu'on a sortie devient la tuile en plus
		Xtra_tuile->tuileE=temp_tuile->tuileE;
		Xtra_tuile->tuileS=temp_tuile->tuileS;
		Xtra_tuile->tuileW=temp_tuile->tuileW;
		Xtra_tuile->tuileT=temp_tuile->tuileT;


	}
			

	closeConnection();
	
	free(laby);
	free(taille);
	free(Xtra_tuile);
	free(tuile_lab);
	free(temp_tuile);

	return 0;
}


