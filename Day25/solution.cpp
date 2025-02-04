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

// Function to split input into schematics separated by blank lines
vector<vector<string>> splitSchematics(const vector<string>& input) {
    vector<vector<string>> schematics;
    vector<string> currentSchematic;
    for (const auto& line : input) {
        if (line.empty()) {
            if (!currentSchematic.empty()) {
                schematics.push_back(currentSchematic);
                currentSchematic.clear();
            }
        } else {
            currentSchematic.push_back(line);
        }
    }
    // Add the last schematic if the file doesn't end with a blank line
    if (!currentSchematic.empty()) {
        schematics.push_back(currentSchematic);
    }
    return schematics;
}

// Function to determine if a schematic is a lock
bool isLock(const vector<string>& schematic) {
    if (schematic.empty()) return false;
    // Check if first row is all '#' and last row is all '.'
    const string& firstRow = schematic[0];
    const string& lastRow = schematic.back();
    for (char c : firstRow) {
        if (c != '#') return false;
    }
    for (char c : lastRow) {
        if (c != '.') return false;
    }
    return true;
}

// Function to determine if a schematic is a key
bool isKey(const vector<string>& schematic) {
    if (schematic.empty()) return false;
    // Check if first row is all '.' and last row is all '#'
    const string& firstRow = schematic[0];
    const string& lastRow = schematic.back();
    for (char c : firstRow) {
        if (c != '.') return false;
    }
    for (char c : lastRow) {
        if (c != '#') return false;
    }
    return true;
}

// Function to convert a lock schematic to height vector
vector<int> convertLockToHeights(const vector<string>& schematic) {
    vector<int> heights;
    if (schematic.empty()) return heights;
    int numRows = schematic.size();
    int numCols = schematic[0].size();
    for (int col = 0; col < numCols; ++col) {
        int height = 0;
        // Start from row 1 (second row) to row numRows-2 (second last row)
        for (int row = 1; row < numRows - 1; ++row) {
            if (schematic[row][col] == '#') {
                height++;
            } else {
                break;
            }
        }
        heights.push_back(height);
    }
    return heights;
}

// Function to convert a key schematic to height vector
vector<int> convertKeyToHeights(const vector<string>& schematic) {
    vector<int> heights;
    if (schematic.empty()) return heights;
    int numRows = schematic.size();
    int numCols = schematic[0].size();
    for (int col = 0; col < numCols; ++col) {
        int height = 0;
        // Start from the second last row upwards to row 0
        for (int row = numRows - 2; row >= 0; --row) { // Exclude the last row which is all '#'
            if (schematic[row][col] == '#') {
                height++;
            } else {
                break;
            }
        }
        heights.push_back(height);
    }
    return heights;
}

// Solution for Part 1
void solvePart1(const vector<string>& input) {
    // Split input into individual schematics
    vector<vector<string>> schematics = splitSchematics(input);

    // Classify schematics into locks and keys
    vector<vector<int>> locks;
    vector<vector<int>> keys;
    int numRows = 0; // To determine max_height

    for (const auto& schematic : schematics) {
        if (schematic.empty()) continue;
        if (isLock(schematic)) {
            vector<int> lockHeights = convertLockToHeights(schematic);
            locks.push_back(lockHeights);
            if (numRows == 0) numRows = schematic.size();
        } else if (isKey(schematic)) {
            vector<int> keyHeights = convertKeyToHeights(schematic);
            keys.push_back(keyHeights);
            if (numRows == 0) numRows = schematic.size();
        } else {
            // Unknown schematic type, ignore or handle accordingly
            cerr << "Warning: Unknown schematic type detected. Ignoring." << endl;
        }
    }

    if (numRows == 0) {
        cerr << "Error: No valid schematics found." << endl;
        return;
    }

    // Corrected max_height calculation
    int max_height = numRows - 2; // Adjusted based on the example

    // Now, iterate through all lock and key pairs
    int validPairs = 0;
    for (const auto& lock : locks) {
        for (const auto& key : keys) {
            bool fits = true;
            for (size_t i = 0; i < lock.size(); ++i) {
                if (lock[i] + key[i] > max_height) {
                    fits = false;
                    break;
                }
            }
            if (fits) {
                validPairs++;
            }
        }
    }

    cout << "Part 1 Solution: " << validPairs << endl;
}

// Solution for Part 2
void solvePart2(const vector<string>& input) {
    cout << "Part 2 Solution: " << endl;
    // TODO: Implement Part 2 solution
}

int main() {
    // Read input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solve both parts
    cout << "Solving Day 25 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
}
