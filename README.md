# UFC Elo Ranking System
This project implements an Elo ranking system specifically tailored for UFC fighters. It calculates fighter ratings based on fight outcomes, with options to include factors such as the method of victory. The application scrapes data from the UFC web site, reads fight data from a CSV file, updates fighter Elo ratings, tracks win-loss records, and saves the results in sorted CSV files.

## Features
- Web Scraping: Automatically scrapes fight data from the UFC Stats website and saves it to a CSV file.
- Elo Ranking System: Calculates fighter Elo scores dynamically based on fight outcomes.
- Win-Loss Tracking: Records victories and defeats for each fighter.
- Customizable Elo Calculations:
  1. Option to include the method of victory (e.g., KO, Submission, Decision) with adjusted impact on Elo scores.
  2. Option to calculate rankings globally or separately by weight categories.
- Insertion Sort Integration: Outputs sorted results directly into CSV files during the saving process.
- Configurable K-Factor: Allows custom K-Factor adjustments based on fight results.

## Requirements
- A C++ compiler (e.g., GCC, Clang, or MSVC)
- Standard C++17 or later
- Python3 and the libraries : requests, BeautifulSoup and panda

## Input File Format
The input CSV file (e.g., ufc_fights.csv) should have the following columns:
- Link: Link to the fight details
- Fighter 1: Name of the first fighter
- Fighter 2: Name of the second fighter
- Method: Method of victory (e.g., KO, Submission, Decision)
- Round: Round in which the fight ended
- Time: Time at which the fight ended
- Weight Class: Weight category of the fight

## Web Scraping
The project includes a Python script that scrapes fight data from the UFC Stats website. It retrieves:
- Fight links
- Fighter names
- Method of victory
- Round and time of the fight
- Weight category

### Scraping Workflow
The script identifies all fight events from the UFC event list.
For each event, it extracts fight details and saves them into a CSV file.
This file is then used as input for the Elo ranking calculations in the C++ program.

## Elo Calculation Details

The Elo ranking system calculates the probability of a fighter winning based on their Elo scores and adjusts their scores accordingly. Below are the details:

### Probability of Winning

The probability that Fighter A beats Fighter B is given by:

```math
P_A = \frac{1}{1 + 10^{(R_B - R_A) / 400}}
```

Where:
- **\( P_A \)**: Probability of Fighter A winning.
- **\( R_A \), \( R_B \)**: Current Elo ratings of Fighter A and Fighter B.

### Score Updates

After a fight, the Elo scores are updated based on the actual outcome:

```math
R'_A = R_A + K \times (\text{Actual Score} - P_A)
```

Where:
- **\( R'_A \)**: New Elo score for Fighter A.
- **\( K \)**: Adjustment factor (higher \( K \) means faster changes).
- **Actual Score**:
  - \( 1 \): Fighter A wins.
  - \( 0 \): Fighter A loses.

### Winning Method Impact

To account for the method of victory, the **\( K \)-factor** is dynamically adjusted based on the following rules:

- **KO/TKO**: \( K = 40 \)
- **Submission**: \( K = 35 \)
- **Decision**: \( K = 25 \)

This adjustment reflects the dominance of certain victory methods, where decisive outcomes like knockouts or submissions result in greater Elo changes compared to decisions.

## Usage
First you have to scrap data from the ufc web site. To do that you can simply execute the python file: 
python3 scraping_ufc_fights.py

### Compilation
To compile the project, use a C++ compiler:
g++ -o ufc_elo_ranking main.cpp

### Execution
Run the executable:
./ufc_elo_ranking

### User Interaction
The application provides options to:
- Include or exclude weight categories in Elo calculations.
- Include or exclude the method of victory in Elo calculations.

Results will be saved in sorted CSV files:
- elo_rankings_sorted.csv: Sorted Elo rankings.
- win_loss_sorted.csv: Sorted win-loss records.

## Project Structure
- main.cpp: Core implementation of the UFC Elo ranking system.
- ufc_fights.csv: Input data containing fight details.
- elo_rankings_sorted.csv: Output file with sorted fighter Elo ratings.
- win_loss_sorted.csv: Output file with sorted win-loss records.

## How It Works
1. Data Loading:
 Reads fight details from the input CSV file.
2. Initialization:
 Assigns initial Elo scores (1500) to all fighters.
 Initializes win-loss records.
3. Elo Calculations:
 Updates fighter Elo scores dynamically based on fight results.
 Adjusts scores based on method of victory (optional).
4. Sorting:
 Uses insertion sort to output Elo and win-loss records in descending order.
5. Output:
 Saves results to CSV files.
