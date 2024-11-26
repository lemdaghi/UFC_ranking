#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <locale>

using namespace std;

// Structure Fight pour représenter un combat
struct Fight {
    string link;
    string fighter_1;
    string fighter_2;
    string method;
    string round;
    string time;
    string weight_class;
};

// Fonction pour charger les données depuis le fichier CSV
vector<Fight> loadFightsFromCSV(const string& filename) {
    vector<Fight> fights;
    ifstream file(filename);
    string line;

    if (file.is_open()) {
        // Ignorer la première ligne (en-têtes)
        getline(file, line);

        // Lire ligne par ligne
        while (getline(file, line)) {
            stringstream ss(line);
            Fight fight;
            string value;

            // Récupérer les valeurs séparées par des virgules
            getline(ss, fight.link, ',');
            getline(ss, fight.fighter_1, ',');
            getline(ss, fight.fighter_2, ',');
            getline(ss, fight.method, ',');
            getline(ss, fight.round, ',');
            getline(ss, fight.time, ',');
            getline(ss, fight.weight_class, ',');

            // Ajouter à la liste des combats
            fights.push_back(fight);
        }
        file.close();
    } else {
        cerr << "Impossible d'ouvrir le fichier : " << filename << endl;
    }

    return fights;
}

// Fonction pour calculer le nouvel Elo
double calculateElo(double currentElo, double opponentElo, bool isWinner, int kFactor = 32) {
    double expectedScore = 1.0 / (1.0 + pow(10, (opponentElo - currentElo) / 400.0));
    double actualScore = isWinner ? 1.0 : 0.0;
    return currentElo + kFactor * (actualScore - expectedScore);
}

// Déterminer le facteur K en fonction de la méthode
int determineKFactor(const string& method) {
    if (method.find("KO") != string::npos || method.find("TKO") != string::npos)
        return 40; // KO/TKO
    else if (method.find("SUB") != string::npos)
        return 35; // Soumission
    else
        return 25; // Décision ou autres
}

// Comparateur pour trier les combattants par Elo (ordre décroissant)
bool compareByElo(const pair<string, double>& a, const pair<string, double>& b) {
    return a.second > b.second;
}

// Fonction pour sauvegarder les classements Elo dans un fichier CSV
void saveEloRankingsToCSV(const map<string, double>& eloScores, const string& filename) {
    ofstream file(filename);

    if (file.is_open()) {
        // Écrire l'en-tête du fichier
        file << "Fighter,Elo\n";

        // Écrire les données des combattants
        for (const auto& pair : eloScores) {
            file << pair.first << "," << fixed << setprecision(2) << pair.second << "\n";
        }

        file.close();
        cout << "Classement Elo sauvegardé dans le fichier : " << filename << endl;
    } else {
        cerr << "Impossible d'ouvrir le fichier pour écrire : " << filename << endl;
    }
}

// Fonction de tri par insertion pour gérer une liste triée
void insertSorted(vector<pair<string, double>>& sorted, const pair<string, double>& newEntry) {
    auto it = sorted.begin();
    while (it != sorted.end() && it->second > newEntry.second) {
        ++it;
    }

    // Vérifier si le combattant existe déjà (éviter les doublons)
    for (const auto& entry : sorted) {
        if (entry.first == newEntry.first) {
            return; // Combattant déjà présent
        }
    }

    // Insérer l'entrée à la position correcte
    sorted.insert(it, newEntry);
}

void saveEloRankingsWithInsertionSortToCSV(const map<string, double>& eloScores, const string& filename) {
    ofstream file(filename);

    if (file.is_open()) {
        // Écrire l'en-tête du fichier
        file << "Fighter,Elo\n";

        // Liste triée pour stocker les scores (en mémoire)
        vector<pair<string, double>> sorted;

        for (const auto& pair : eloScores) {
            // Insérer chaque combattant dans la liste triée
            insertSorted(sorted, pair);
        }

        // Écrire la liste triée dans le fichier
        for (const auto& sortedPair : sorted) {
            file << sortedPair.first << "," << fixed << setprecision(2) << sortedPair.second << "\n";
        }

        file.close();
        cout << "Classement Elo sauvegardé dans le fichier (tri par insertion appliqué) : " << filename << endl;
    } else {
        cerr << "Impossible d'ouvrir le fichier pour écrire : " << filename << endl;
    }
}

void saveWinLossRecordsToCSV(const map<string, pair<int, int>>& winLossRecords, const string& filename) {
    ofstream file(filename);

    if (file.is_open()) {
        // Écrire l'en-tête du fichier
        file << "Fighter,Victories,Defeats\n";

        // Écrire les données des combattants
        for (const auto& pair : winLossRecords) {
            file << pair.first << "," << pair.second.first << "," << pair.second.second << "\n";
        }

        file.close();
        cout << "Victoires et défaites sauvegardées dans le fichier : " << filename << endl;
    } else {
        cerr << "Impossible d'ouvrir le fichier pour écrire : " << filename << endl;
    }
}

int main() {
    // Charger les combats depuis le fichier CSV
    vector<Fight> fights = loadFightsFromCSV("ufc_fights.csv");

    // Map pour stocker les scores Elo des combattants
    map<string, double> eloScores;

    // Map pour stocker les victoires et défaites
    map<string, pair<int, int>> winLossRecords;

    // Choisir le mode de calcul
    int choice;
    cout << "Choisissez le mode de calcul Elo :\n";
    cout << "1. Sans tenir compte de la méthode\n";
    cout << "2. En tenant compte de la méthode\n";
    cout << "Entrez votre choix : ";
    cin >> choice;

    // Initialiser chaque combattant avec un Elo de base et 0 victoires/défaites
    for (const auto& fight : fights) {
        if (eloScores.find(fight.fighter_1) == eloScores.end()) {
            eloScores[fight.fighter_1] = 1500;
            winLossRecords[fight.fighter_1] = {0, 0};
        }
        if (eloScores.find(fight.fighter_2) == eloScores.end()) {
            eloScores[fight.fighter_2] = 1500;
            winLossRecords[fight.fighter_2] = {0, 0};
        }
    }

    // Mettre à jour les scores Elo et les victoires/défaites après chaque combat
    for (const auto& fight : fights) {
        bool fighter1Wins = (fight.method.find("win") == string::npos); // le 1er fighter est toujours le gagnant

        double fighter1Elo = eloScores[fight.fighter_1];
        double fighter2Elo = eloScores[fight.fighter_2];

        int kFactor = 32; // Par défaut pour choix 1
        if (choice == 2) {
            kFactor = determineKFactor(fight.method); // Ajuster le K selon la méthode
        }
        // Mettre à jour les scores Elo
        eloScores[fight.fighter_1] = calculateElo(fighter1Elo, fighter2Elo, fighter1Wins, kFactor);
        eloScores[fight.fighter_2] = calculateElo(fighter2Elo, fighter1Elo, !fighter1Wins, kFactor);

        // Mettre à jour les victoires et défaites
        if (fighter1Wins) {
            winLossRecords[fight.fighter_1].first++;  // Victoire pour fighter_1
            winLossRecords[fight.fighter_2].second++; // Défaite pour fighter_2
        } else {
            winLossRecords[fight.fighter_2].first++;  // Victoire pour fighter_2
            winLossRecords[fight.fighter_1].second++; // Défaite pour fighter_1
        }
    }

    // Sauvegarder les résultats dans un fichier CSV
    if (choice == 1) {
        saveEloRankingsWithInsertionSortToCSV(eloScores, "elo_rankings_without_method.csv");
    } else if (choice == 2) {
        saveEloRankingsWithInsertionSortToCSV(eloScores, "elo_rankings_with_method.csv");
    }

    // Sauvegarder les victoires et défaites dans un fichier
    saveWinLossRecordsToCSV(winLossRecords, "win_loss_records.csv");
    return 0;
}
