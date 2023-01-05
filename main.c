#include <stdio.h>
#include <stdlib.h>
#include "labyrinthAPI.h"
#include "clientAPI.h"


int main (void) {

	
	int TailleX;
	int TailleY;
	char labyrinthNom[50];
	connectToServer("172.105.76.204",1234,"Yeogeuch");
	waitForLabyrinth("TRAINING DONTMOVE timeout=1000 display=debug margin=1 start=1 seed=0x000002",labyrinthNom,&TailleX,&TailleY);
	
	int tuileN;
	int tuileE;
	int tuileS;
	int tuileW;
	int tuileItem;
	
	int* laby;
	laby=malloc( (TailleX*TailleY*5)*sizeof(int) );
		
	int indic_joueur=getLabyrinth(laby, &tuileN, &tuileE, &tuileS, &tuileW, &tuileItem);

	//printf ("%d %d %d %d %d\n\n", tuileN, tuileE,tuileS,tuileW,tuileItem);
	
	
	// l'etat des joueur : NORMAL_MOVE = 0,WINNING_MOVE = 1,LOOSING_MOVE = -1
	t_return_code stat_moi=0; 
	t_return_code stat_adversaire=0;
	
	// declaration des variable type t_move (mien + adversaire)
	t_move *mouv_moi;
	t_move *mouv_adversaire;
	printf("%d",indic_joueur);
	
	
	while ((stat_moi==0)&&(stat_adversaire==0)){ 
		if (indic_joueur==1){ //affiche laby, MAJ stat, donne le tour au joueur;
			printLabyrinth();
			printf("c'est au tour du joueur adversaire de jouer,veuillez patientez...\n");
			stat_adversaire = getMove(mouv_adversaire);
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


	closeConnection();

	return 0;
}
