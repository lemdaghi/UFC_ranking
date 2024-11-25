// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <vector>
// #include <string>
// #include <map>
// #include <cmath>
// #include <algorithm>
// using namespace std;

// struct Fight {
//     string link;
//     string fighter_1;
//     string fighter_2;
//     string method;
//     string round;
//     string time;
//     string weight_class;
// };

// // Fonction pour charger les données depuis le fichier CSV
// vector<Fight> loadFightsFromCSV(const string& filename) {
//     vector<Fight> fights;
//     ifstream file(filename);
//     string line;

//     if (file.is_open()) {
//         // Ignorer la première ligne (en-têtes)
//         getline(file, line);

//         // Lire ligne par ligne
//         while (getline(file, line)) {
//             stringstream ss(line);
//             Fight fight;
//             string value;

//             // Récupérer les valeurs séparées par des virgules
//             getline(ss, fight.link, ',');
//             getline(ss, fight.fighter_1, ',');
//             getline(ss, fight.fighter_2, ',');
//             getline(ss, fight.method, ',');
//             getline(ss, fight.round, ',');
//             getline(ss, fight.time, ',');
//             getline(ss, fight.weight_class, ',');

//             // Ajouter à la liste des combats
//             fights.push_back(fight);
//         }
//         file.close();
//     } else {
//         cerr << "Impossible d'ouvrir le fichier : " << filename << endl;
//     }

//     return fights;
// }

// // Fonction pour calculer le nouvel Elo
// double calculateElo(double currentElo, double opponentElo, bool isWinner, int kFactor = 32) {
//     double expectedScore = 1.0 / (1.0 + pow(10, (opponentElo - currentElo) / 400.0));
//     double actualScore = isWinner ? 1.0 : 0.0;
//     return currentElo + kFactor * (actualScore - expectedScore);
// }

// // Comparateur pour trier les combattants par Elo (ordre décroissant)
// bool compareByElo(const pair<string, double>& a, const pair<string, double>& b) {
//     return a.second > b.second; // Trier en ordre décroissant
// }

// int main() {
//     // Carte pour stocker les scores Elo
//     map<string, double> eloScores = {
//         {"Fighter A", 1500},
//         {"Fighter B", 1600},
//         {"Fighter C", 1400}
//     };

//     // Convertir le map en vector pour trier
//     vector<pair<string, double>> sortedElo(eloScores.begin(), eloScores.end());

//     // Trier les combattants par Elo
//     sort(sortedElo.begin(), sortedElo.end(), compareByElo);

//     // Afficher les combattants triés par Elo
//     cout << "Classement Elo :" << endl;
//     for (const auto& pair : sortedElo) {
//         cout << "Fighter: " << pair.first << ", Elo: " << pair.second << endl;
//     }

//     return 0;
// }


// int main() {
//     // Charger les combats depuis le fichier CSV
//     vector<Fight> fights = loadFightsFromCSV("ufc_fights.csv");

//     // // Afficher les données
//     // for (const auto& fight : fights) {
//     //     cout << "Fight Details:" << endl;
//     //     cout << "Link: " << fight.link << endl;
//     //     cout << "Fighter 1: " << fight.fighter_1 << endl;
//     //     cout << "Fighter 2: " << fight.fighter_2 << endl;
//     //     cout << "Method: " << fight.method << endl;
//     //     cout << "Round: " << fight.round << endl;
//     //     cout << "Time: " << fight.time << endl;
//     //     cout << "Weight Class: " << fight.weight_class << endl;
//     //     cout << "---------------------" << endl;
//     // }

//     // Carte pour stocker les scores Elo des combattants
//     map<string, double> eloScores;

//     // Initialiser chaque combattant avec un Elo de base (par exemple, 1500)
//     for (const auto& fight : fights) {
//         if (eloScores.find(fight.fighter_1) == eloScores.end())
//             eloScores[fight.fighter_1] = 1500;
//         if (eloScores.find(fight.fighter_2) == eloScores.end())
//             eloScores[fight.fighter_2] = 1500;
//     }

//     // Mettre à jour les scores Elo après chaque combat
//     for (const auto& fight : fights) {
//         // Déterminer le gagnant en fonction de la méthode
//         bool fighter1Wins = (fight.method.find("win") != string::npos);

//         // Mettre à jour les scores
//         double fighter1Elo = eloScores[fight.fighter_1];
//         double fighter2Elo = eloScores[fight.fighter_2];

//         eloScores[fight.fighter_1] = calculateElo(fighter1Elo, fighter2Elo, fighter1Wins);
//         eloScores[fight.fighter_2] = calculateElo(fighter2Elo, fighter1Elo, !fighter1Wins);
//     }

//     // Afficher les scores Elo finaux
//     cout << "Final Elo Rankings:" << endl;
//     for (const auto& [fighter, elo] : eloScores) {
//         cout << fighter << ": " << elo << endl;
//     }

//     return 0;
// }



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

// Comparateur pour trier les combats par round (en ordre croissant)
bool compareByRound(const Fight& a, const Fight& b) {
    return stoi(a.round) < stoi(b.round);
}

// Comparateur pour trier par catégorie de poids (ordre alphabétique)
bool compareByWeightClass(const Fight& a, const Fight& b) {
    return a.weight_class < b.weight_class;
}

// Comparateur pour trier les combattants par Elo (ordre décroissant)
bool compareByElo(const pair<string, double>& a, const pair<string, double>& b) {
    return a.second > b.second;
}

// Fonction pour sauvegarder les classements Elo dans un fichier CSV
void saveEloRankingsToCSV(const std::map<std::string, double>& eloScores, const std::string& filename) {
    std::ofstream file(filename);

    if (file.is_open()) {
        // Écrire l'en-tête du fichier
        file << "Fighter,Elo\n";

        // Écrire les données des combattants
        for (const auto& pair : eloScores) {
            file << pair.first << "," << std::fixed << std::setprecision(2) << pair.second << "\n";
        }

        file.close();
        std::cout << "Classement Elo sauvegardé dans le fichier : " << filename << std::endl;
    } else {
        std::cerr << "Impossible d'ouvrir le fichier pour écrire : " << filename << std::endl;
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
        bool fighter1Wins = (fight.method.find("win") != string::npos);

        double fighter1Elo = eloScores[fight.fighter_1];
        double fighter2Elo = eloScores[fight.fighter_2];

        // Mettre à jour les scores Elo
        eloScores[fight.fighter_1] = calculateElo(fighter1Elo, fighter2Elo, fighter1Wins);
        eloScores[fight.fighter_2] = calculateElo(fighter2Elo, fighter1Elo, !fighter1Wins);

        // Mettre à jour les victoires et défaites
        if (fighter1Wins) {
            winLossRecords[fight.fighter_1].first++;  // Victoire pour fighter_1
            winLossRecords[fight.fighter_2].second++; // Défaite pour fighter_2
        } else {
            winLossRecords[fight.fighter_2].first++;  // Victoire pour fighter_2
            winLossRecords[fight.fighter_1].second++; // Défaite pour fighter_1
        }
    }

    // Sauvegarder le classement Elo dans un fichier
    saveEloRankingsToCSV(eloScores, "elo_rankings.csv");

    // Sauvegarder les victoires et défaites dans un fichier
    saveWinLossRecordsToCSV(winLossRecords, "win_loss_records.csv");

    // // Menu pour afficher et trier les résultats
    // int choice;
    // do {
    //     cout << "\nMenu :\n";
    //     cout << "1. Afficher les combats triés par round\n";
    //     cout << "2. Afficher les combats triés par catégorie de poids\n";
    //     cout << "3. Afficher les combattants triés par score Elo\n";
    //     cout << "4. Quitter\n";
    //     cout << "Entrez votre choix : ";
    //     cin >> choice;

    //     switch (choice) {
    //         case 1: {
    //             sort(fights.begin(), fights.end(), compareByRound);
    //             cout << "\nCombats triés par round :\n";
    //             for (const auto& fight : fights) {
    //                 cout << "Round: " << fight.round
    //                      << ", Fighters: " << fight.fighter_1 << " vs " << fight.fighter_2
    //                      << ", Weight Class: " << fight.weight_class << endl;
    //             }
    //             break;
    //         }
    //         case 2: {
    //             sort(fights.begin(), fights.end(), compareByWeightClass);
    //             cout << "\nCombats triés par catégorie de poids :\n";
    //             for (const auto& fight : fights) {
    //                 cout << "Weight Class: " << fight.weight_class
    //                      << ", Fighters: " << fight.fighter_1 << " vs " << fight.fighter_2
    //                      << ", Round: " << fight.round << endl;
    //             }
    //             break;
    //         }
    //         case 3: {
    //             vector<pair<string, double>> sortedElo(eloScores.begin(), eloScores.end());
    //             sort(sortedElo.begin(), sortedElo.end(), compareByElo);

    //             cout << "\nClassement Elo (du meilleur au moins bon) :\n";
    //             for (const auto& pair : sortedElo) {
    //                 cout << "Fighter: " << pair.first << ", Elo: " << pair.second << endl;
    //             }
    //             break;
    //         }
    //         case 4:
    //             cout << "Au revoir !\n";
    //             break;
    //         default:
    //             cout << "Choix invalide. Réessayez.\n";
    //     }
    // } while (choice != 4);

    return 0;
}
