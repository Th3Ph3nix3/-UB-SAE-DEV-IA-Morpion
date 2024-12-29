#ifndef __IA_V2__
#define __IA_V2__

/*
 * Compte le nombre de pion directemen aligné
 * tab : le tableau NxN
 * N : la taille du tableau
 * x: coordonnée dans la hauteur
 * y : coordonnée dans la largeur
 * dx : coordonnée x de départ
 * dy : coordonnée y de départ
 * pion : notre type de pion X ou O
*/
int compter_alignement(char** tab, int N, int x, int y, int dx, int dy, char pion);


#endif


