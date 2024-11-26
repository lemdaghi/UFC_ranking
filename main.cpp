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

// Structure of a Fight
struct Fight {
    string link;
    string fighter_1;
    string fighter_2;
    string method;
    string round;
    string time;
    string weight_class;
};

// Fonction to load data from CSV file
vector<Fight> loadFightsFromCSV(const string& filename) {
    vector<Fight> fights;
    ifstream file(filename);
    string line;

    if (file.is_open()) {
        // Ignore the 1st line (header)
        getline(file, line);

        // Read line by line
        while (getline(file, line)) {
            stringstream ss(line);
            Fight fight;
            string value;

            // Get values separated by commas
            getline(ss, fight.link, ',');
            getline(ss, fight.fighter_1, ',');
            getline(ss, fight.fighter_2, ',');
            getline(ss, fight.method, ',');
            getline(ss, fight.round, ',');
            getline(ss, fight.time, ',');
            getline(ss, fight.weight_class, ',');

            // Add fights to the list
            fights.push_back(fight);
        }
        file.close();
    } else {
        cerr << "Impossible d'ouvrir le fichier : " << filename << endl;
    }

    return fights;
}

// Fonction to calculate the new Elo
double calculateElo(double currentElo, double opponentElo, bool isWinner, int kFactor = 32) {
    double expectedScore = 1.0 / (1.0 + pow(10, (opponentElo - currentElo) / 400.0));
    double actualScore = isWinner ? 1.0 : 0.0;
    return currentElo + kFactor * (actualScore - expectedScore);
}

// Define the K factor according to the winning method
int determineKFactor(const string& method) {
    if (method.find("KO") != string::npos || method.find("TKO") != string::npos)
        return 40; // KO/TKO
    else if (method.find("SUB") != string::npos)
        return 35; // Submission
    else
        return 25; // Decision or other
}

// Fonction to sort by insertion
void insertSorted(vector<pair<string, double>>& sorted, const pair<string, double>& newEntry) {
    auto it = sorted.begin();
    while (it != sorted.end() && it->second > newEntry.second) {
        ++it;
    }

    // Verify if the fighter already exists 
    for (const auto& entry : sorted) {
        if (entry.first == newEntry.first) {
            return; // fighter already exists
        }
    }

    // Insert input in correct position
    sorted.insert(it, newEntry);
}

// Fonction for saving the Elo's ranking in a CSV file
void saveEloRankingsToCSV(const map<string, double>& eloScores, const string& filename) {
    ofstream file(filename);

    if (file.is_open()) {
        // Write the header
        file << "Fighter,Elo\n";

        // Write data in the file
        for (const auto& pair : eloScores) {
            file << pair.first << "," << fixed << setprecision(2) << pair.second << "\n";
        }

        file.close();
        cout << "Elo Ranking saved in : " << filename << endl;
    } else {
        cerr << "Impossible to open the file : " << filename << endl;
    }
}

// Fontion to sort data then saving it in a CSV file
void saveEloRankingsWithInsertionSortToCSV(const map<string, double>& eloScores, const string& filename) {
    ofstream file(filename);

    if (file.is_open()) {
        // Write the header
        file << "Fighter,Elo\n";

        vector<pair<string, double>> sorted;

        for (const auto& pair : eloScores) {
            // Insert every fighter in the sorted list
            insertSorted(sorted, pair);
        }

        // Copy the sorted list in the CSV file
        for (const auto& sortedPair : sorted) {
            file << sortedPair.first << "," << fixed << setprecision(2) << sortedPair.second << "\n";
        }

        file.close();
        cout << "Elo Ranking sorted is saved in : " << filename << endl;
    } else {
        cerr << "Impossible to open the file : " << filename << endl;
    }
}

// Fonction to save the win/loss record for every fighter in a CSV file
void saveWinLossRecordsToCSV(const map<string, pair<int, int>>& winLossRecords, const string& filename) {
    ofstream file(filename);

    if (file.is_open()) {
        // Write the header
        file << "Fighter,Victories,Defeats\n";

        // Write data in the CSV file
        for (const auto& pair : winLossRecords) {
            file << pair.first << "," << pair.second.first << "," << pair.second.second << "\n";
        }

        file.close();
        cout << "Victories and defeats saved in : " << filename << endl;
    } else {
        cerr << "Impossible to open the file : " << filename << endl;
    }
}

int main() {
    // Load data from the recovered CSV file
    vector<Fight> fights = loadFightsFromCSV("ufc_fights.csv");

    // Map to save the Elo score of fighters
    map<string, double> eloScores;

    // Map to save the win/loss records
    map<string, pair<int, int>> winLossRecords;

    // Choisir le mode de calcul
    int choice;
    cout << "Choose Elo calculation method :\n";
    cout << "1. Regardless of the winning method\n";
    cout << "2. Taking into account the winning method\n";
    cout << "Enter your choice : ";
    cin >> choice;

    // Initialize every fighter with a basic Elo and 0 wins/losses
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

    // Update Elo scores and wins/losses after each fight
    for (const auto& fight : fights) {
        bool fighter1Wins = (fight.method.find("win") == string::npos); // the 1st fighter is always the winner according to the UFC web site

        double fighter1Elo = eloScores[fight.fighter_1];
        double fighter2Elo = eloScores[fight.fighter_2];

        int kFactor = 32; // by default for choice 1
        if (choice == 2) {
            kFactor = determineKFactor(fight.method); // Adjust K factor according to the method
        }
        // Update Elo scores
        eloScores[fight.fighter_1] = calculateElo(fighter1Elo, fighter2Elo, fighter1Wins, kFactor);
        eloScores[fight.fighter_2] = calculateElo(fighter2Elo, fighter1Elo, !fighter1Wins, kFactor);

        // Update wins/losses
        if (fighter1Wins) {
            winLossRecords[fight.fighter_1].first++;  // win for fighter_1
            winLossRecords[fight.fighter_2].second++; // loss for fighter_2
        } else {
            winLossRecords[fight.fighter_2].first++;  // win for fighter_2
            winLossRecords[fight.fighter_1].second++; // loss for fighter_1
        }
    }

    // Sauve results in a CSV file
    if (choice == 1) {
        saveEloRankingsWithInsertionSortToCSV(eloScores, "elo_rankings_without_method.csv");
    } else if (choice == 2) {
        saveEloRankingsWithInsertionSortToCSV(eloScores, "elo_rankings_with_method.csv");
    }

    // Save win/loss record in a CSV file
    saveWinLossRecordsToCSV(winLossRecords, "win_loss_records.csv");
    return 0;
}
