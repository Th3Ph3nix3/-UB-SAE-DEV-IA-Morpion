#include"morpion.h"

int main() {

	char ** tab;
	int size = 3 ; //20;
	int nbPion = 3 ; //5;
	
	init_morpion(tab,size);
	
	bool alignement = false;
	bool joueur = false;
	
	int nbPl = 0;
	
	while(!alignement && nbPl < size*size) {
	
		affichage_morpion(tab,size);
	
		char pion;
		(joueur ? pion = 'X' : pion = 'O');
		joueur = not(joueur);
		
		int x = -1; 
		int y = -1;
		if (nbPl%2 == 0){
			do {
				std::cin >> x >> y;
				std::cout<<std::endl;
				
			} while(!estLibre_morpion(tab,size,x,y));
		
			placer_morpion(tab,size,x,y,pion);
			affichage_morpion(tab,size);
		}
		
		else{
			jouer(tab, size, nbPion);
		}
		
		if(pion == 'O') alignement = victoire_morpion(tab,size,nbPion,'O');
		else if(pion == 'X') alignement = victoire_morpion(tab,size,nbPion,'X');
		nbPl++; 
	}

	delete_morpion(tab,size);
}
