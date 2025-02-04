#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// Function to read input from file
vector<string> readInput() {
    vector<string> input;
    string line;
    ifstream file("input.txt");
    
    if (!file.is_open()) {
        cerr << "Error: Could not open input.txt" << endl;
        return input;
    }

    while (getline(file, line)) {
        input.push_back(line);
    }

    file.close();
    return input;
}

// Helper function to check if a sequence is safe
bool isSafe(const vector<int>& levels) {
    if (levels.size() < 2) return true;
    
    // Checks first difference to determine if sequence should be increasing or decreasing
    bool shouldIncrease = levels[1] > levels[0];
    
    for (size_t i = 1; i < levels.size(); i++) {
        int diff = levels[i] - levels[i-1];
        
        // Checks the difference 
        if (abs(diff) < 1 || abs(diff) > 3) return false;
        
        // Check if direction matches the initial direction
        if (shouldIncrease && diff <= 0) return false;
        if (!shouldIncrease && diff >= 0) return false;
    }
    
    return true;
}

// Results for Part 1
void solvePart1(const vector<string>& input) {
    cout << "Part 1 Results: " << endl;
    
    int safeReports = 0;
    
    for (const string& line : input) {
        vector<int> levels;
        istringstream iss(line);
        int level;
        
        while (iss >> level) {
            levels.push_back(level);
        }
        
        if (isSafe(levels)) {
            safeReports++;
        }
    }
    
    cout << safeReports << endl;
}

// Results for Part 2
void solvePart2(const vector<string>& input) {
    cout << "Part 2 Results: " << endl;

    int safeReports = 0;

    for (const string& line : input) {
        vector<int> levels;
        istringstream iss(line);
        int level;

        while (iss >> level) {
            levels.push_back(level);
        }

        // Checks if the report is safe
        if (isSafe(levels)) {
            safeReports++;
            continue; // Already safe, no need to check further
        }

        // Checks if removing one level can make it safe
        bool canBeSafe = false;
        for (size_t i = 0; i < levels.size(); i++) {
            vector<int> modifiedLevels = levels;
            modifiedLevels.erase(modifiedLevels.begin() + i); // Remove one level

            if (isSafe(modifiedLevels)) {
                canBeSafe = true;
                break; // No need to check further if we found a safe configuration
            }
        }

        if (canBeSafe) {
            safeReports++;
        }
    }

    cout << safeReports << endl;
}

int main() {
    // Read input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    cout << "Solving Day 2 Puzzle 🏃‍♀️..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 