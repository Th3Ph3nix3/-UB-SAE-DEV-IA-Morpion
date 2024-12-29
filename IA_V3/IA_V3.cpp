/*##################################################################

Créateur : Th3Ph3nix
Date de dernière mise à jour : 28/12/2024

Description : Deuxième version de l'IA sachant jouer au morpion
              
##################################################################*/

#include <iostream>
#include <chrono>
#include <cstdlib> // Module pour random
#include <vector> // Module pour gérer les tableaux

#include"morpion.h"

using namespace std;

int debut = 0;
char MonPion = 'X';
char PionEnnemis = 'O';
bool première_manche = true;
const int Num = 7; // Taille du tableau
char enregistre_pion[Num][Num];

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
/*
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
*/

// Algorithme pour le morpion 3x3
const int SCORE_VICTOIRE = 1000;
const int SCORE_DEFAITE = -1000;
const int SCORE_EGALITE = 0;
const int PROFONDEUR_MAX = 6;
char pionHumain = 'O';
char pionIA = 'X';

int compteurElagage = 0;

struct Mouvement {
    int x;
    int y;
    int score;
};

// Vérifie si la partie est une égalité (aucun mouvement possible)
bool estEgalite(char** plateau, int taillePlateau) {
    for (int i = 0; i < taillePlateau; i++) {
        for (int j = 0; j < taillePlateau; j++) {
            if (plateau[i][j] == ' ') { // S'il reste une case vide, la partie n'est pas terminée
                return false;
            }
        }
    }
    return true; // Partie terminée
}

// Compte le nombre de pions alignés dans une direction donnée
int compterAlignes(char** plateau, int taillePlateau, int i, int j, int di, int dj, char pion) {
    int alignes = 0;
    while (i >= 0 && i < taillePlateau && j >= 0 && j < taillePlateau && plateau[i][j] == pion) {
        alignes++;
        i += di;
        j += dj;
    }
    return alignes;
}

// Évalue une position en fonction des alignements possibles
int evaluerHeuristique(char** plateau, int taillePlateau, char pionIA, char pionHumain) {
    int score = 0;

    // Parcourt chaque case du plateau
    for (int i = 0; i < taillePlateau; i++) {
        for (int j = 0; j < taillePlateau; j++) {
            if (plateau[i][j] == ' ') { // Case vide, position stratégique possible

                // Évalue les alignements pour l'IA
                score += compterAlignes(plateau, taillePlateau, i, j, 1, 0, pionIA);  // Vertical vers le bas
                score += compterAlignes(plateau, taillePlateau, i, j, 0, 1, pionIA);  // Horizontal vers la droite
                score += compterAlignes(plateau, taillePlateau, i, j, 1, 1, pionIA);  // Diagonale vers le bas-droite
                score += compterAlignes(plateau, taillePlateau, i, j, 1, -1, pionIA); // Diagonale vers le bas-gauche

                // Évalue les alignements pour l'adversaire (blocage)
                score -= compterAlignes(plateau, taillePlateau, i, j, 1, 0, pionHumain);  // Vertical vers le bas
                score -= compterAlignes(plateau, taillePlateau, i, j, 0, 1, pionHumain);  // Horizontal vers la droite
                score -= compterAlignes(plateau, taillePlateau, i, j, 1, 1, pionHumain);  // Diagonale vers le bas-droite
                score -= compterAlignes(plateau, taillePlateau, i, j, 1, -1, pionHumain); // Diagonale vers le bas-gauche
            }
        }
    }

    return score;
}

// Implémente l'algorithme Minimax avec élagage alpha-bêta
int minimax(char** plateau, int taillePlateau, int K, int profondeur, int alpha, int beta, bool maximiser) {
    bool victoireIA = victoire_morpion(plateau, taillePlateau, K, pionIA);
    bool victoireHumain = victoire_morpion(plateau, taillePlateau, K, pionHumain);
    bool egalite = estEgalite(plateau, taillePlateau);
    int score = 0;

    // Conditions de fin
    if (victoireIA || victoireHumain || egalite || profondeur >= PROFONDEUR_MAX) {
        if (victoireIA) {
            return SCORE_VICTOIRE - profondeur;
        }
        if (victoireHumain) {
            return SCORE_DEFAITE + profondeur;
        }
        if (egalite) {
            return SCORE_EGALITE;
        }
        if (profondeur >= PROFONDEUR_MAX) {
            return evaluerHeuristique(plateau, taillePlateau, pionIA, pionHumain);
        }
    }

    // Tour de l'IA
    if (maximiser) {
        int meilleurScore = numeric_limits<int>::min();
        for (int i = 0; i < taillePlateau; i++) {
            for (int j = 0; j < taillePlateau; j++) {   
                if (plateau[i][j] == ' ') {
                    plateau[i][j] = pionIA;
                    score = minimax(plateau, taillePlateau, K, profondeur + 1, alpha, beta, false);
                    plateau[i][j] = ' ';

                    meilleurScore = max(meilleurScore, score);
                    alpha = max(alpha, score);

                    if (beta <= alpha) {   
                        compteurElagage++;
                        break;
                    }
                }  
            }
        }
        return meilleurScore;
    } else { // Tour de l'humain
        int meilleurScore = numeric_limits<int>::max();
        for (int i = 0; i < taillePlateau; i++) {
            for (int j = 0; j < taillePlateau; j++) {
                if (plateau[i][j] == ' ') {
                    plateau[i][j] = pionHumain;
                    score = minimax(plateau, taillePlateau, K, profondeur + 1, alpha, beta, true);
                    plateau[i][j] = ' ';

                    meilleurScore = min(meilleurScore, score);
                    beta = min(beta, score);
    
                    if (beta <= alpha) {
                        compteurElagage++;
                        break;
                    }
                }
            }
        }
        return meilleurScore;
    }
}

// Trouve le meilleur mouvement pour l'IA
void trouverMeilleurMouvement(char** plateau, int taillePlateau, int K, char pion) {
    int meilleurScore = numeric_limits<int>::min();
    int alpha = numeric_limits<int>::min();
    int beta = numeric_limits<int>::max();
    int score = 0;
    int profondeur = 0;
    Mouvement mouvement;

    // Tour de l'IA
    for (int i = 0; i < taillePlateau; i++) {
        for (int j = 0; j < taillePlateau; j++) {
            // Si la case est libre
            if (plateau[i][j] == ' ') {
                plateau[i][j] = pion;
                score = minimax(plateau, taillePlateau, K, profondeur, alpha, beta, false);
                plateau[i][j] = ' ';

                if (score > meilleurScore) {
                    meilleurScore = score;
                    mouvement.x = i;
                    mouvement.y = j;
                }
            }
        }
    }
    plateau[mouvement.x][mouvement.y] = pion;
}



// Méthode a suivre pour Tous les autres taille de morpion
// On choisie une coordonnée au hasard
int coordzone(int N){
    srand(time(NULL));
    int coord_x = rand() % N;
    srand(time(NULL));
    int coord_y = rand() % N;
    return coord_x, coord_y;
}

// On vérifie si la zone choisie est vide
bool estZoneLibre(char** plateau, int taillePlateau, int K, int L, int x, int y) {
    // Parcourt la zone de taille KxL autour de (x, y)
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < L; j++) {
            int xi = x + i; // Coordonnée dans la direction verticale
            int yj = y + j; // Coordonnée dans la direction horizontale

            // Vérifie si la position est hors du plateau
            if (xi < 0 || xi >= taillePlateau || yj < 0 || yj >= taillePlateau) {
                return false; // La zone sort du plateau
            }

            // Vérifie si la case n'est pas libre
            if (plateau[xi][yj] != ' ') {
                return false; // La zone n'est pas entièrement libre
            }
        }
    }

    return true; // Toutes les cases dans la zone sont libres
}


void jouer(char** tab, int N, int K) {
    
    // parcours_entier_plateau(tab, PionEnnemis, N, MonPion);
    // int maxAlignement = trouver_alignement_max(tab, N, PionEnnemis);
    // cout << "Le plus long alignement de ennemi est : " << maxAlignement << endl;
    if(N == 3){
        trouverMeilleurMouvement(tab, N, 3, MonPion);
    }
}















// // Début du chronométrage
// auto start = chrono::high_resolution_clock::now();
// // Fin du chronométrage
// auto end = chrono::high_resolution_clock::now();

// // Calcul de la durée de l'opération
// chrono::duration<double> duration = end - start;
// cout << "Durée de l'opération: " << duration.count() << " secondes" << endl;