#include"morpion.h"

int main() {
	srand(time(NULL));
	char ** tab;
	int size = 10;
	int nbPion = 5;
	
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
			placer_morpion(tab,size, x, y, pion);
			affichage_morpion(tab,size);
		}
		
		else{
			cout << "L'IA réfléchi..." << endl;
			jouer(tab, size, nbPion);
			cout << "L'IA a fini de jouer" << endl;
		}
		
		if(pion == 'O')alignement = victoire_morpion(tab,size,nbPion,'O');
		else if(pion == 'X') alignement = victoire_morpion(tab,size,nbPion,'X');
		
		nbPl++; 
	}
	affichage_morpion(tab,size);
	delete_morpion(tab,size);
}
