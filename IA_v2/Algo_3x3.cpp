/*##################################################################

Créateur : Th3Ph3nix
Date de dernière mise à jour : 28/12/2024

Description : Algorithme a suivre par l'IA lors dun morpion 3x3
              
##################################################################*/

#include <iostream>

#include "../morpion.h"
// #include "Include/IA_to_Algo.hpp"

using namespace std;

const int WIN_SCORE = 1000;
const int LOSS_SCORE = -1000;
const int TIE_SCORE = 0;
const int DEPTH_MAX = 6;
char pionHuman = 'O';
char pionAI = 'X';

int prunning_count = 0;

struct Move {
    int x;
    int y;
    int score;
};

bool isTie(char** tab, int boardSize){
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            if (tab[i][j] == ' ') // if there's one case empty, then the game's not ended
            {
                return false;
            }
        }
    }
    return true; // end game
}

int countAligned(char** tab, int boardSize, int i, int j, int di, int dj, char pion) {
    int align = 0;
    while (i >= 0 && i < boardSize && j >= 0 && j < boardSize && tab[i][j] == pion) {
        align++;
        i += di;
        j += dj;
    }
    return align;
}

int evaluateHeuristic(char** tab, int boardSize, char pionAI, char pionHuman) {
    int score = 0;

    // Parcourt chaque case
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (tab[i][j] == ' ') { // Case vide, potentielle position stratégique

                // Évalue les alignements pour l'IA
                score += countAligned(tab, boardSize, i, j, 1, 0, pionAI);  // Vertical vers le bas
                score += countAligned(tab, boardSize, i, j, 0, 1, pionAI);  // Horizontal vers le bas
                score += countAligned(tab, boardSize, i, j, 1, 1, pionAI);  // Diagonale vers le bas-droite
                score += countAligned(tab, boardSize, i, j, 1, -1, pionAI); // Diagonale vers le bas-gauche

                // Évalue les alignements pour l'adversaire (blocage)
                score -= countAligned(tab, boardSize, i, j, 1, 0, pionHuman);  // Vertical vers le bas
                score -= countAligned(tab, boardSize, i, j, 0, 1, pionHuman);  // Horizontal vers le bas
                score -= countAligned(tab, boardSize, i, j, 1, 1, pionHuman);  // Diagonale vers le bas-droite
                score -= countAligned(tab, boardSize, i, j, 1, -1, pionHuman); // Diagonale vers le bas-gauche
            }
        }
    }

    return score;
}

int minimax(char** tab, int boardSize, int K, int depth, int alpha, int beta, bool isMaximizing){
    bool resultAI = victoire_morpion(tab, boardSize, K, pionAI);
    bool resultHuman = victoire_morpion(tab, boardSize, K, pionHuman);
    bool resultTie = isTie(tab, boardSize);
    int score = 0;

    // End condition
    if (resultAI == true || resultHuman == true || resultTie == true || depth >= DEPTH_MAX)
    {
        if (resultAI)
        {   
            //std::cout << "eifjesoifjseoifj1" << std::endl;
            return WIN_SCORE - depth;
        }
        if (resultHuman){
            //std::cout << "eifjesoifjseoifj2" << std::endl;
            return LOSS_SCORE + depth;
        }
        if (resultTie){
            //std::cout << "eifjesoifjseoifj3" << std::endl;
            return TIE_SCORE;
        }
        if (depth >= DEPTH_MAX)
        {
            //std::cout << "fefsji" << std::endl;
            return evaluateHeuristic(tab, boardSize, pionAI, pionHuman);
        }
    }

    // IA turn
    if (isMaximizing)
    {
        int bestScore = std::numeric_limits<int>::min();
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {   
                if (tab[i][j] == ' ')
                {
                    tab[i][j] = pionAI;
                    score = minimax(tab, boardSize, K, depth + 1, alpha, beta, false);
                    tab[i][j] = ' ';

                    bestScore = std::max(bestScore, score);

                    alpha = std::max(alpha, score);

                    if (beta <= alpha)
                    {   
                        prunning_count++;
                        break;
                    }
                }  
            }
        }
        return bestScore;
    }
    else
    {
        int bestScore = std::numeric_limits<int>::max();
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                if (tab[i][j] == ' ')
                {
                    tab[i][j] = pionHuman;
                    score = minimax(tab, boardSize, K, depth + 1, alpha, beta, true);
                    tab[i][j] = ' ';

                    bestScore = std::min(bestScore, score);

                    beta = std::min(beta, score);
    
                    if (beta <= alpha)
                    {
                        prunning_count++;
                        break;
                    }
                }
            }
        }
        return bestScore;
    }
}

void getBestMove(char** tab, int boardSize, int K, char pion){
    int bestScore = std::numeric_limits<int>::min();
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();
    int score = 0;
    int depth = 0;
    Move move;

    // AI turn
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            
            //std::cout << "(" << i << ", " << j << ") : " << tab[i][j] << std::endl;
            
            // if the spot is available
            if (tab[i][j] == ' ')
            {
                tab[i][j] = pion;
                
                score = minimax(tab, boardSize, K, depth, alpha, beta, false);
                
                //std::cout << "score : " << score << "\n" << std::endl;
                
                tab[i][j] = ' ';
                if (score > bestScore)
                {
                    bestScore = score;
                    move.x = i;
                    move.y = j;
                }

                std::cout << "oui" << std::endl;
            }
        }
    }
    tab[move.x][move.y] = pion;
}