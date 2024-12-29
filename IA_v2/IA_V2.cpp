/*##################################################################

Créateur : Th3Ph3nix
Date de dernière mise à jour : 28/12/2024

Description : Deuxième version de l'IA sachant jouer au morpion
              
##################################################################*/

#include <iostream>
#include <chrono>
#include <cstdlib> // Module pour random
#include <vector> // Module pour gérer les tableaux

#include "Include/Algo_to_IA_2.hpp"

using namespace std;

int debut = 0;

bool première_manche = true;
const int Num = 7; // Taille du tableau

char enregistre_pion[Num][Num];

// Déclaration de la structure Coord


// Initialisation automatique via un constructeur global
class Initializer {
public:
    Initializer() {
        for (int i = 0; i < Num; ++i) {
            for (int j = 0; j < Num; ++j) {
                enregistre_pion[i][j] = ' '; 
            }
        }
        cout << "Tableau initialise automatiquement." << endl;
    }
};

Initializer init;

void parcours_entier_plateau(char** tab, char PionEnnemis, int N, char MonPion) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if ((tab[i][j] == PionEnnemis) && (enregistre_pion[i][j] == ' ')) {
                enregistre_pion[i][j] = PionEnnemis;
                // cout << "Nouvelle position pion ennemis en " << i << " " << j << endl;
            }
            if ((tab[i][j] == MonPion) && (enregistre_pion[i][j] == ' ')) {
                enregistre_pion[i][j] = MonPion;
                // cout << "Nouvelle position de mon pion en " << i << " " << j << endl;
            }
        }
    }
}

// Test si le plateau de taille NxN donné en paramètre est vide
bool plateau_est_vide(int N, char** tab){
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (tab[i][j] != ' ') {
                return false;
            }
        }
    }
    return true;
}

// Fonction pour compter l'alignement depuis une position donnée dans une direction
int compter_alignement(char** tab, int N, int x, int y, int dx, int dy, char pion) {
    int count = 0;

    // Tant que les coordonnées sont valides et que la case contient le pion recherché
    while (x >= 0 && x < N && y >= 0 && y < N && tab[x][y] == pion) {
        count++;    // Incrémenter le compteur
        x += dx;    // Avancer dans la direction X
        y += dy;    // Avancer dans la direction Y
    }

    return count;
}

// Fonction pour trouver le plus long alignement dans le tableau
int trouver_alignement_max(char** tab, int N, char pion) {
    int maxAlignement = 0;

    // Liste des 8 directions possibles
    int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},    // Haut, Bas, Gauche, Droite
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}   // Diagonales
    };

    // Parcours du tableau entier
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (tab[i][j] == pion) { // Si on trouve un pion
                // Explorer toutes les directions à partir de cette position
                for (int d = 0; d < 8; ++d) {
                    int dx = directions[d][0];
                    int dy = directions[d][1];

                    // Compter l'alignement dans la direction actuelle
                    int alignement = compter_alignement(tab, N, i, j, dx, dy, pion);

                    // Mettre à jour la longueur maximale
                    if (alignement > maxAlignement) {
                        maxAlignement = alignement;
                    }
                }
            }
        }
    }

    return maxAlignement;
}


char MonPion = 'X';
char PionEnnemis = 'O';

void jouer(char** tab, int N, int K) {
    
    parcours_entier_plateau(tab, PionEnnemis, N, MonPion);
//  int maxAlignement = trouver_alignement_max(tab, N, PionEnnemis);
//  cout << "Le plus long alignement de ennemi est : " << maxAlignement << endl;
    if(N == 3){
        // partie3x3(tab, MonPion, PionEnnemis);
        getBestMove(tab, N, 3, MonPion);
    }
}















// // Début du chronométrage
// auto start = chrono::high_resolution_clock::now();
// // Fin du chronométrage
// auto end = chrono::high_resolution_clock::now();

// // Calcul de la durée de l'opération
// chrono::duration<double> duration = end - start;
// cout << "Durée de l'opération: " << duration.count() << " secondes" << endl;