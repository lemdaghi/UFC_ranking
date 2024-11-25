// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <sstream>
// using namespace std;

// struct Fighter {
//     string name;
//     int wins;
//     int losses;
//     string method;
// };

// vector<Fighter> loadFighters(const string& filename) {
//     vector<Fighter> fighters;
//     ifstream file(filename);
//     string line;
//     if (file.is_open()) {
//         getline(file, line); // Ignorer l'en-tête
//         while (getline(file, line)) {
//             stringstream ss(line);
//             string name, wins, losses, method;
//             getline(ss, name, ',');
//             getline(ss, wins, ',');
//             getline(ss, losses, ',');
//             getline(ss, method, ',');
            
//             fighters.push_back({name, stoi(wins), stoi(losses), method});
//         }
//         file.close();
//     }
//     return fighters;
// }

// int main() {
//     vector<Fighter> fighters = loadFighters("ufc_fighters.csv");
//     for (const auto& f : fighters) {
//         cout << "Name: " << f.name << ", Wins: " << f.wins
//              << ", Losses: " << f.losses << ", Method: " << f.method << endl;
//     }
//     return 0;
// }


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
using namespace std;

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

int main() {
    // Charger les combats depuis le fichier CSV
    vector<Fight> fights = loadFightsFromCSV("ufc_fights.csv");

    // Afficher les données
    for (const auto& fight : fights) {
        cout << "Fight Details:" << endl;
        cout << "Link: " << fight.link << endl;
        cout << "Fighter 1: " << fight.fighter_1 << endl;
        cout << "Fighter 2: " << fight.fighter_2 << endl;
        cout << "Method: " << fight.method << endl;
        cout << "Round: " << fight.round << endl;
        cout << "Time: " << fight.time << endl;
        cout << "Weight Class: " << fight.weight_class << endl;
        cout << "---------------------" << endl;
    }
    
    // // Carte pour stocker les scores Elo des combattants
    // std::map<std::string, double> eloScores;

    // // Initialiser chaque combattant avec un Elo de base (par exemple, 1500)
    // for (const auto& fight : fights) {
    //     if (eloScores.find(fight.fighter_1) == eloScores.end())
    //         eloScores[fight.fighter_1] = 1500;
    //     if (eloScores.find(fight.fighter_2) == eloScores.end())
    //         eloScores[fight.fighter_2] = 1500;
    // }

    // // Mettre à jour les scores Elo après chaque combat
    // for (const auto& fight : fights) {
    //     // Déterminer le gagnant en fonction de la méthode
    //     bool fighter1Wins = (fight.method.find("win") != std::string::npos);

    //     // Mettre à jour les scores
    //     double fighter1Elo = eloScores[fight.fighter_1];
    //     double fighter2Elo = eloScores[fight.fighter_2];

    //     eloScores[fight.fighter_1] = calculateElo(fighter1Elo, fighter2Elo, fighter1Wins);
    //     eloScores[fight.fighter_2] = calculateElo(fighter2Elo, fighter1Elo, !fighter1Wins);
    // }

    // // Afficher les scores Elo finaux
    // std::cout << "Final Elo Rankings:" << std::endl;
    // for (const auto& [fighter, elo] : eloScores) {
    //     std::cout << fighter << ": " << elo << std::endl;
    // }

    return 0;
}