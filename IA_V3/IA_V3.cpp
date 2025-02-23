/*##################################################################

Créateur : Th3_Ph3nix
Date de dernière mise à jour : 19 / 01 /2025

Description : Troisième version de l'IA sachant jouer au morpion
              
##################################################################*/

#include <iostream>
#include <chrono>
#include <cstdlib> // Module pour random
#include <vector> // Module pour gérer les tableaux
#include <fstream> //Module pour ouvrir les fichiers
#include <sstream>
#include <string>

using namespace std;

char MonPion = 'X';
char PionEnnemis = 'O';
bool patternCharge = false;
bool patternCroix = false;
int coord_x;
int coord_y;
int taillePattern;
bool methodeDeux = false;
bool methodeTrois = false;
string pattern;
vector<pair<int, int>> coordPattern;

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

bool victoireMorpion(char** tab, int size, int nbpion, char pion) {

	for(int i = 0; i < size; ++i) {
	
		for(int j = 0; j < size; ++j) if (tab[i][j] == pion) {
		
			int align = 0, i2, j2; 
			
			i2 = i; align = 0;
			while( i2 < size && (i2-i) < nbpion ) { align = align + (tab[i2][j] == pion); i2++; }
			if(align == nbpion) return true;
			
			j2 = j; align = 0;
			while( j2 < size && (j2-j) < nbpion ) { align = align + (tab[i][j2] == pion); j2++; }
			if(align == nbpion) return true;
			
			j2 = j; i2 = i; align = 0;
			while( i2 < size && (j2-j) < nbpion && j2 < size && (j2-j) < nbpion) { align = align + (tab[i2][j2] == pion); i2++; j2++; }
			if(align == nbpion) return true;
			
			j2 = j; i2 = i; align = 0;
			while( i2 < size && (j2-j) < nbpion && j2 >=0 && (j-j2) < nbpion) { align = align + (tab[i2][j2] == pion); i2++; j2--; }
			if(align == nbpion) return true;
			
		}
	}
	
	return false;
}

// Implémente l'algorithme Minimax avec élagage alpha-bêta
int minimax(char** plateau, int taillePlateau, int K, int profondeur, int alpha, int beta, bool maximiser) {
    bool victoireIA = victoireMorpion(plateau, taillePlateau, K, pionIA);
    bool victoireHumain = victoireMorpion(plateau, taillePlateau, K, pionHumain);
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
// Permet de séléctionner le pattern qu'on va jouer
string selectionPattern(int K) {
    ifstream stringPattern;
    
    // Sélectionne le fichier en fonction de N
    if ((K == 4) || (K == 5)) {
        stringPattern.open("IA_V3/Pattern/4-5_pattern.txt");
        taillePattern = 4;
    } else if ((K == 6) || (K == 7)) {
        stringPattern.open("IA_V3/Pattern/6-7_pattern.txt");
        taillePattern = 6;
    } else if ((K == 8) || (K == 9)) {
        stringPattern.open("IA_V3/Pattern/8-9_pattern.txt");
        taillePattern = 8;
    } else {
        cout << "Erreur : taille non supportée" << endl;
        return "ERREUR 1";
    }

    // Vérifie si le fichier s'est ouvert correctement
    if (!stringPattern.is_open()) {
        cout << "Erreur d'ouverture du fichier" << endl;
        return "ERREUR 2";
    }

    string ligne;
    vector<string> lignes;

    // Lit toutes les lignes du fichier et les stocke dans un vecteur
    while (getline(stringPattern, ligne)) {
        lignes.push_back(ligne);
    }

    // Ferme le fichier après lecture
    stringPattern.close();

    // Vérifie si le fichier contient des lignes
    if (lignes.empty()) {
        cout << "Erreur : fichier vide" << endl;
        return "ERREUR 3";
    }

    if (patternCroix) {
        // Retourne la dernière ligne du fichier
        return lignes.back();
    } 
    else {
        // Tire une ligne aléatoire
        int index = rand() % (lignes.size()-1);
        if (index == lignes.size()-1){
            patternCroix = true;
        }
        return lignes[index];
    }
}

// On choisie une coordonnée au hasard qui sera le centre de la zone qu'on va jouer
void coordzone(int N) {
    coord_x = rand() % N;
    coord_y = rand() % N;
}

// On vérifie si la zone choisie est vide
bool estZoneLibre(char** tab, int N) {
    // Vérification des cases de la forme
    for (const auto& coord : coordPattern) {
        int x = coord_x + coord.first;
        int y = coord_y + coord.second;

        // Vérifie que la case est dans les limites et qu'elle est vide
        if (x < 0 || x >= N || y < 0 || y >= N || tab[x][y] == PionEnnemis) {
            return false; // Une case de la forme est occupée ou hors limite
        }
    }
    if (tab[coord_x][coord_y] == PionEnnemis){
        return false;
    }
    return true; // La forme et sa zone environnante sont vides
}


// Fonction pour segmenter une chaîne de caractères en fonction d'un délimiteur
vector<string> segmenter(const string& chaine, char delimiteur) {
    vector<string> segments;
    stringstream ss(chaine);
    string segment;

    // Découpage par le délimiteur
    while (getline(ss, segment, delimiteur)) {
        segments.push_back(segment);
    }
    return segments;
}

// Fonction pour convertir une chaîne de la forme "x,y" en un tuple (x, y)
pair<int, int> convertirEnCoordonnees(const string& coordString) {
    stringstream ss(coordString);
    string x, y;

    // Séparation autour de la virgule
    if (!getline(ss, x, ',') || !getline(ss, y, ',')) {
        cout << "Erreur : Le format de la chaîne est invalide. Chaîne reçue : " << coordString << endl;
        return {-1, -1}; // Coordonnées par défaut en cas d'erreur
    }

    try {
        // Convertit les chaînes en entiers
        int coordX = stoi(x);
        int coordY = stoi(y);

        return {coordX, coordY};
    } catch (const invalid_argument& e) {
        cout << "Erreur : Conversion en entier échouée. Valeur reçue : " << coordString << endl;
        return {-1, -1}; // Coordonnées par défaut en cas d'erreur
    } catch (const out_of_range& e) {
        cout << "Erreur : Valeur hors des limites pour une conversion en entier. Chaîne reçue : " << coordString << endl;
        return {-1, -1}; // Coordonnées par défaut en cas d'erreur
    }
}

// Fonction placant le pion de l'IA en fonction des coordonnées sélectionnée dans le pattern
void placePion(int N, char MonPion, char PionEnnemis, char** tab, int K){
    int essai = 0;
    if (taillePattern == 0){
        if (patternCroix){
            pattern = selectionPattern(K);
            taillePattern = 5;
        }
        else{
            tab[coord_x][coord_y] = MonPion;
        }
    }

    placeInvalide:
    essai++;
    
    int coordAction = rand() % taillePattern;
    if (essai >= 15){
        if (methodeDeux == false){
            methodeDeux = true;
            return;
        }
        else{
            methodeTrois = true;
            return;
        }
    }

    // Accéder aux coordonnées et modifier le tableau
    int x = coordPattern[coordAction].first;  // Récupérer la ligne
    int y = coordPattern[coordAction].second; // Récupérer la colonne
    if ((coord_x + x < 0) || (coord_x + x >= N) || (coord_y + y < 0) || (coord_y + y >= N) || (tab[coord_x + x][coord_y + y] != ' ')) {
        goto placeInvalide;
    }


    if (tab[coord_x + x][coord_y + y] == PionEnnemis){
        patternCroix = false;
        pattern = selectionPattern(K);
        vector<string> segments = segmenter(pattern, '/');
        for (const string& segment : segments) {
            coordPattern.push_back(convertirEnCoordonnees(segment));
        }
        
        while (estZoneLibre(tab, N) != true){
            coordzone(N);
        }
        goto placeInvalide;
    }

    // Place le caractère
    tab[x + coord_x][y + coord_y] = MonPion;

    //Efface la coordonnée qu'on viens de placer
    coordPattern.erase(coordPattern.begin() + coordAction);
    taillePattern--;
}

// Fonction verifiant qu'un type de pion n'est pas sur le point de gagner
bool verifierVictoire(char** tab, int N, char pion, int K) {
    // Vérifie les lignes
    for (int i = 0; i < N; ++i) {
        int compteur = 0;
        for (int j = 0; j < N; ++j) {
            if (tab[i][j] == pion) {
                compteur++;
            }
            else {
                compteur = 0;
            }
            if (compteur == K){
                return true; // Adversaire ou moi pouvons gagner ici
            }
        }
    }

    // Vérifie les colonnes
    for (int j = 0; j < N; ++j) {
        int compteur = 0;
        for (int i = 0; i < N; ++i) {
            if (tab[i][j] == pion) {
                compteur++;
            }
            else {
                compteur = 0;
            }
            if (compteur == K){
                return true; // Adversaire ou moi pouvons gagner ici
            }
        }
    }

    // Vérifie les diagonales principales
    for (int d = 0; d <= N - (N - 1); ++d) {
        int compteur1 = 0, compteur2 = 0;
        for (int i = 0; i < N; ++i) {
            if (i + d < N && tab[i][i + d] == pion) {
                compteur1++;
            }
            else {
                compteur1 = 0;
            }
            if (i + d < N && tab[i + d][i] == pion) {
                compteur2++;
            }
            else {
                compteur2 = 0;
            }
            if (compteur1 == K || compteur2 == K){
                return true; // Adversaire ou moi pouvons gagner ici
            }
        }
    }

    // Vérifie les diagonales secondaires
    for (int d = 0; d <= N - (N - 1); ++d) {
        int compteur = 0;
        for (int i = 0; i < N; ++i) {
            if (i + d < N && tab[i][N - 1 - (i + d)] == pion) {
                compteur++;
            }
            else {
                compteur = 0;
            }
            if (compteur == K){
                return true; // Adversaire ou moi pouvons gagner ici
            }
        }
    }

    return false; // Aucun alignement gagnant trouvé
}

// Fonction qui grace a vérifier victoire permet de bloquer l'ennemi si il risque de gagner
bool bloquerAdversaireAnticipe(char** tab, int N, int K) {
    // Parcourt chaque case vide pour simuler un coup de l'adversaire
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (tab[i][j] == ' ') { // Case vide trouvée
                // Simule un coup de l'adversaire
                tab[i][j] = PionEnnemis;

                // Vérifie si ce coup crée une victoire immédiate
                if (verifierVictoire(tab, N, PionEnnemis, K)) {
                    // Bloque directement ce coup
                    tab[i][j] = MonPion;
                    return true;
                }

                // Vérifie les coups suivants de l'adversaire après ce premier coup
                for (int x = 0; x < N; ++x) {
                    for (int y = 0; y < N; ++y) {
                        if (tab[x][y] == ' ') { // Case vide pour le deuxième coup
                            // Simule un deuxième coup de l'adversaire
                            tab[x][y] = PionEnnemis;

                            // Si ce deuxième coup permet de gagner, bloque le premier
                            if (verifierVictoire(tab, N, PionEnnemis, K)) {
                                // Annule les simulations
                                tab[x][y] = MonPion;
                                tab[i][j] = ' ';
                                return true; // Bloque en anticipation
                            }

                            // Annule le deuxième coup simulé
                            tab[x][y] = ' ';
                        }
                    }
                }

                // Annule le premier coup simulé
                tab[i][j] = ' ';
            }
        }
    }

    return false; // Aucun coup bloquant trouvé
}

// Fonction qui si le plateau est bien rempli va chercher un endroit qui lui ferait gagner
bool coupVictoire(char** tab, int N, int K) {
    // Parcourt chaque case vide pour simuler un coup que l'IA ferait
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (tab[i][j] == ' ') { // Case vide trouvée
                // Simule un coup de lui
                tab[i][j] = MonPion;

                // Vérifie si ce coup crée une victoire immédiate
                if (verifierVictoire(tab, N, MonPion, K)) {
                    // L'IA place son pion pour la victoire
                    tab[i][j] = MonPion;
                    return true;
                }

                // Vérifie ses coups suivants après ce premier coup
                for (int x = 0; x < N; ++x) {
                    for (int y = 0; y < N; ++y) {
                        if (tab[x][y] == ' ') { // Case vide pour le deuxième coup
                            // Simule un deuxième coup à lui
                            tab[x][y] = MonPion;

                            // Si ce deuxième coup permet de gagner, alors il joue comme il peut
                            if (verifierVictoire(tab, N, MonPion, K)) {
                                // Annule les simulations
                                tab[x][y] = ' ';
                                tab[i][j] = MonPion;
                                return true; // Bloque en anticipation
                            }

                            // Annule le deuxième coup simulé
                            tab[x][y] = ' ';
                        }
                    }
                }

                // Annule le premier coup simulé
                tab[i][j] = ' ';
            }
        }
    }

    return false; // Aucun coup bloquant trouvé
}

// Fonction qui cherche de facon non aléatoire un endroit où placer son paterne
void trouverBonneZone(char** tab, int N, int K){
    int nbCaseVide = 0;
    int essai = 0;
    // Compte le nombre de case vide
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (tab[i][j] != PionEnnemis && tab[i][j] != MonPion){
                nbCaseVide++;
            }
        }
    }
    // Va placer un pion dans la zone libre en fonction de son pattern mais si on ne trouve pas assez rapidement on change de stratégie
    while (estZoneLibre(tab, N) != true) {
        int numCase = rand() % N;
        while (nbCaseVide != 0){
            for (int i = 0; i < N; i++){
                for (int j = 0; j < N; j++){
                    if (tab[i][j] != PionEnnemis && tab[i][j] != MonPion){
                        nbCaseVide--;
                        coord_x = i;
                        coord_y =j;
                    }
                }
            }
        }
        patternCroix = false; 
        pattern = selectionPattern(K);
        vector<string> segments = segmenter(pattern, '/');
        for (const string& segment : segments) {
            coordPattern.push_back(convertirEnCoordonnees(segment));
        }
        essai++;
        if(essai >= 50){
            methodeTrois = true;
            return;
        }
    } 
}

// Algorythme si le jeu est plus grand que 3x3
void procedureJeu(char** tab, int N, int K){
    int essai = 0;
    if (bloquerAdversaireAnticipe(tab, N, K)){
        return;
    }
    // Si aucun blocage n'est nécessaire

    // Méthode 1 on cherche une zone aléatoirement pour poser en fonction d'un pattern
    if(methodeDeux == false && methodeTrois == false){
        while (estZoneLibre(tab, N) != true && essai != 20){
            essai++;
            if(essai >= 25){
                methodeDeux = true;
                goto methode2;
            }
            patternCroix = false; 
            pattern = selectionPattern(K);
            vector<string> segments = segmenter(pattern, '/');
            for (const string& segment : segments) {
                coordPattern.push_back(convertirEnCoordonnees(segment));
            }
            coordzone(N);
        }
        placePion(N, MonPion, PionEnnemis, tab, K);
    }

    // Méthode 2 On cherche de facon stratégique une zone pour poser en fonction d'un pattern
    else if(methodeDeux == true && methodeTrois == false){
        methode2:
        trouverBonneZone(tab, N, K);
        if (methodeTrois == true){
            goto methode3;
        }
        else{
            placePion(N, MonPion, PionEnnemis, tab, K);
        }
    }
    // Méthode 3 On cherche les endroits gagnant quand le plateau est bien rempli
    else if(methodeDeux == true && methodeTrois == true){
        methode3:
        if(coupVictoire(tab, N, K)){
        }
        else{
            for (int i = 0; i < N; i++){
                for (int j = 0; j < N; j++){
                    if(tab[i][j] == ' '){
                        tab[i][j] = MonPion;
                        return;
                    }
                }
            }
        }
    }
    // Erreur dans les méthodes
    else{
        cout << "Erreur methode revoir fonctionnement" << endl;
        cout << "methode 2 = " << methodeDeux << endl;
        cout << "methode 3 = " << methodeTrois << endl;
        return;
    }
}



void jouer(char** tab, int N, int K) {
    // Début du chronométrage
    auto start = chrono::high_resolution_clock::now();
    // Pour un plateau 3x3
    if(N == 3){
        trouverMeilleurMouvement(tab, N, 3, MonPion);
    }
    //  Pour un jeu où on doit alligné entre 3 et 9 pion
    if((K < 9) && (K > 3)){
        if(patternCharge == false){
            cout << "AVEZ VOUS BIEN PENSÉ À INITIALISÉ LA FONCTION RANDOM DANS LE MAIN ?" << endl;
            pattern = selectionPattern(K);
            vector<string> segments = segmenter(pattern, '/');
            for (const string& segment : segments) {
                coordPattern.push_back(convertirEnCoordonnees(segment));
                // cout << "5" << endl;
            }
            coordzone(N);
            patternCharge = true;
        }
        procedureJeu(tab, N, K);
    }
    // Pour tous les autres cas de jeu
    else{
        trouverMeilleurMouvement(tab, N, K, MonPion);
    }
    // Fin du chronométrage
    auto end = chrono::high_resolution_clock::now();

    // Calcul de la durée de l'opération
    chrono::duration<double> duration = end - start;
    cout << "Durée de l'opération: " << duration.count() << " secondes" << endl;
}