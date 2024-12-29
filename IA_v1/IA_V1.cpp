/*##################################################################

Créateur : Th3Ph3nix
Date de dernière mise à jour : 09/12/2024

Description : Première version de l'IA sachant jouer au morpion
              
##################################################################*/

#include<iostream>
#include<fstream>
#include<string>
#include<chrono>
using namespace std;

// Récupère des coordonnées i et j et les enregistre dans le fichier texte
void SaveCoord(int i, int j, string file){
    string str_i = to_string(i);
    string str_j = to_string(j);

    // Concaténer les chaînes avec un espace entre elles
    string result = str_i + " " + str_j;

    // Ouvrir le fichier en mode lecture-écriture
    fstream ListeEmplacement(file, ios::in | ios::out);

    if (!ListeEmplacement.is_open()) {
        cerr << "Impossible d'ouvrir le fichier." << endl;
        return;
    }

    string ligne;
    while (getline(ListeEmplacement, ligne)) {
        if (ligne.empty()) {
            // Revenir à la position de la ligne vide
            ListeEmplacement.seekp(ListeEmplacement.tellg());
            ListeEmplacement << result << endl;
            ListeEmplacement.close();
            return;
        }
    }

    // Si aucune ligne vide n'est trouvée, écrire à la fin du fichier
    ListeEmplacement.clear(); // Effacer les indicateurs d'erreur
    ListeEmplacement.seekp(0, ios::end);
    ListeEmplacement << result << endl;
    ListeEmplacement.close();
}

// Parcourt le tableau entier et rajoute l'emplacement des pions s'ils ne sont pas notés
void parcours_entier_plateau(char** tab, char PionEnnemis, int N, char MonPion){
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (tab[i][j] == PionEnnemis) {
                SaveCoord(i, j, "IA_v1/PositionEnnemi.txt");
                cout << "Position pion ennemis en " << i << " " << j << endl;
            }

            else if (tab[i][j] == MonPion) {
                SaveCoord(i, j, "IA_v1/MesPositions.txt");
                cout << "Position de mes pions en " << i << " " << j << endl;
            }
        }
    }
}

// Compte le nombre de mots possibles pour voir si le fichier est bien vide
void VerificationTXT(int &NbEmplacement, string file){
    ifstream ListeEmplacement(file);  // Ouvrir le fichier 

    // Vérifie si le fichier est ouvert correctement et compte le nombre de d'emplacements
    if (ListeEmplacement.is_open()) {
        string coord;
        while (getline(ListeEmplacement, coord)) NbEmplacement++;
        ListeEmplacement.close();  // Ferme le fichier après la lecture
        if (NbEmplacement == 0) cout << "Fichier good" << endl;
        else cout << "Fichier non vide" << endl;
    } else {
        // Si le fichier ne s'ouvre pas, affiche un message d'erreur
        cout << "Jeu HS, requiert l'attention d'un administrateur, code 0" << endl;
        return;
    }
}

// Initialise et clear le fichier texte
void init_clr_fich_txt(){
    ofstream fichierEnn("IA_v1/PositionEnnemi.txt", ios::trunc);
    ofstream fichierMoi("IA_v1/MesPositions.txt", ios::trunc);
}

void jouer(char** tab, int N, int K) {
    char MonPion = 'X'; char PionEnnemis = 'O'; int NbEmplacement = 0; int comptetour = 0;

    // On efface le fichier texte au démarrage
    init_clr_fich_txt();

    // On vérifie si les fichiers sont vide
    VerificationTXT(NbEmplacement, "IA_v1/PositionEnnemi.txt");
    VerificationTXT(NbEmplacement, "IA_v1/MesPositions.txt");

    // Marquer le début du chronométrage
    auto start = chrono::high_resolution_clock::now();

    // On découvre le tableau
    parcours_entier_plateau(tab, PionEnnemis, N, MonPion);

    // Marquer la fin du chronométrage
    auto end = chrono::high_resolution_clock::now();
    
    // Calculer la durée de l'opération
    chrono::duration<double> duration = end - start;
    
    cout << "Durée de l'opération: " << duration.count() << " secondes" << endl;

    // On efface le fichier texte a la fin
    // init_clr_fich_txt();
}