#include <X11/Xlib.h>			/* pour XLib */
#include <X11/keysym.h>			/* pour les touches XLib */
#include <X11/Xatom.h>			/* pour le message WM_DELETE_WINDOW */
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "affichage.h"
#include "lecture.h"
#define TAILLE_X	101
#define TAILLE_Y	61


/*Question 3 :commande pour compilation et édition de lien:
gcc -Wall -o labyrinthe.o -c labyrinthe.c 
gcc -o laby.out affichage.o labyrinthe.o -lX11 */




/* Fonction qui permet de se déplacer d'une case dans une direction d
entré : une case (int tab[2]) et une direction (int d) 
sortie: pas de sortie(on change directement les coordonnées de la case d'entrée)*/
void case_voisine (int case_actuel[2], int d) {
	if (d==0) {
		case_actuel[1]--;
	}
	if (d==1) {
		case_actuel[0]++;
	}
	if (d==2) {
		case_actuel[1]++;
	}
	if (d==3){
		case_actuel[0]--;
	}
}      
/* Si on utilise cette fonctiion pour une case en extrémité du tableau, on passera à une case à l'extérieur du tableau qui n'a pas été défini, il y a de très forte chance d'avoir un SegFault */



	

/*Fonction qui permet de faire une expansion pour une case : tous les cases voisines d'une case qui sont vides sont incrémantés par rapport à r
*/
        				
void expansion_1_case (int lab[TAILLE_X][TAILLE_Y],int depart[2],int arrive[2]) {	
	int position[2]; // on créer une variable locale pour pas modifier la tableau depart placé en entrée
	position[0]=depart[0];
	position[1]=depart[1];
		for (int a=0;a<4;a++){
		position[0]=depart[0];
		position[1]=depart[1];
			case_voisine (position,a);
			if (lab[position[0]][position[1]] == 0) {
				lab[position[0]][position[1]]=lab[depart[0]][depart[1]]+1 ;
			}
		}
	     				
}        			

/*Cette fonction va nous permettre da faire l'expansion pour toute les cases du tableau. Pour cela on fait une expansion pour une case situé à une distance r de la case de départ,cette distance sera incrémenté à chaque tour de la boucle   */

void expansion (int lab[TAILLE_X][TAILLE_Y],int depart[2],int arrive[2]) {
	for (int z=1; z<6162 ; z++){   // 101*61=6161 la distance entre deux cases ne peut pas excéder le nombre de case, pour être plus exacte il aurait fallu utiliser TAILLE_X*TAILLE_Y
		for(int i=0;i<TAILLE_Y;i++){
        		for(int j=0;j<TAILLE_X;j++){
        			if (lab[j][i] == z ) {  //on fait une "expansion locale" pour chaque situé à une distance r qui est incrémenté (qui correspond à z dans la fonction)
        			int depart_locale[2];
        			depart_locale[0]=j;
        			depart_locale[1]=i;
        			expansion_1_case (lab,depart_locale,arrive);
        			}
        		}
        	}
        }
}

/*Fonction qui permet de faire la remontée à paritr de la case d'arrive jusqu'à atteindre la case de depart ( donc r=2), Si l'expansion n'a pas pu se faire, donc lab[arrive[0]][arrive[1]] = 0 donc on ne rentre pas dans la boucle*/
void de_expansion (int lab[TAILLE_X][TAILLE_Y],int depart[2],int arrive[2]) {

	for (int r=lab [arrive[0]][arrive[1]]; r>1 ; r--){
		int position2[2];
		position2[0]=arrive[0];
		position2[1]=arrive[1];
		for (int a=0;a<4;a++){
			position2[0]=arrive[0];
			position2[1]=arrive[1];
			case_voisine (position2,a);
			if (lab[position2[0]][position2[1]] == r-1){
				lab[position2[0]][position2[1]]=-2;
				break;
			}
		}
		arrive[0]=position2[0];
		arrive[1]=position2[1];
	}
	
}	

/*Fonction de netttoyage qui permet de remmetre à 0 si ce n'est ni un obstacle ni le chemin de remonté*/
     			
void nettoyage (int lab[TAILLE_X][TAILLE_Y]) {

	for(int i=0;i<TAILLE_Y;i++){
        	for(int j=0;j<TAILLE_X;j++){
        		if ( (lab[j][i] != -1) && (lab[j][i] != -2) ){
        			lab[j][i]=0;
        		}
        	}
        }

}
	 

int main(void ) {
	
	int depart[2];
	int arrive[2];
	int lab[TAILLE_X][TAILLE_Y];
	laby_constr("laby1.txt",lab,arrive,depart);
	
	
	int depart0[2]= {depart[0],depart[1]};
	int arrive0[2]= {arrive[0],arrive[1]};
	
	//les cases de depart et arivee, on les affiches pour vérifier qu'on s'est pas trompé
	printf ("la case de depart est : %d %d\n",depart0[0],depart0[1]);
	printf ("la case de arrive est : %d %d\n",arrive0[0],arrive0[1]);
	
	lab[depart[0]][depart[1]] =1; // on initialise la case de départ à r=1;
	
	expansion (lab,depart,arrive); // phase d'expansion
	
	de_expansion (lab,depart,arrive) ; // phase de remontée
		
	depart[0]=depart0[0];
	depart[1]=depart0[1];
	arrive[0]=arrive0[0];
	arrive[1]=arrive0[1];
	
  	nettoyage(lab);
	
	//affichage principal de la fonction
	initAffichage();
	afficheLabyrinthe( lab, depart,arrive,0);
	
	return 0;
}
