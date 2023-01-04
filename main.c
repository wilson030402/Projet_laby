#include <stdio.h>
#include <stdlib.h>
#include "labyrinthAPI.h"
#include "clientAPI.h"



//void PlayMove ( int* lab, int* tileN, int* tileE, int* tileS, int* tileW, int* tileItem){


int main (void) {

	
	int TailleX;
	int TailleY;
	char labyrinthNom[50];
	connectToServer("172.105.76.204",1234,"Yeogeuch");
	
	waitForLabyrinth("TRAINING DONTMOVE",labyrinthNom,&TailleX,&TailleY);
	
	int tuileN;
	int tuileE;
	int tuileS;
	int tuileW;
	int tuileItem;
	
	int* laby;
	laby=malloc( (TailleX*TailleY*5)*sizeof(int) );
		
	getLabyrinth(laby, &tuileN, &tuileE, &tuileS, &tuileW, &tuileItem);
	printLabyrinth();
	printf ("%d %d %d %d %d\n", tuileN, tuileE,tuileS,tuileW,tuileItem);


	closeConnection();

	return 0;
}
