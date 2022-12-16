#include <stdio.h>
#include <stdlib.h>
#include "labyrinthAPI.h"
#include "clientAPI.h"


int main (void) {

	
	int TailleX;
	int TailleY;
	char* labyrinthNom;
	connectToServer("172.105.76.204",1234,"Wilson");
	
	
	waitForLabyrinth("TRAINING DONTMOVE",labyrinthNom,&TailleX,&TailleY);
	
	//getLabyrinth(int* lab, int* tileN, int* tileE, int* tileS, int* tileW, int* tileItem);



	closeConnection();
	printf("Arrive ici\n");
	return 0;
}
