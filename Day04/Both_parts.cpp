#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Function to read input from file
vector<string> readInput() {
    vector<string> input;
    string line;
    ifstream file("input.txt");
    
    if (!file.is_open()) {
        cerr << "No input file found!" << endl;
        return input;
    }

    while (getline(file, line)) {
        input.push_back(line);
    }

    file.close();
    return input;
}

// Function to check if XMAS exists starting from a position in a given direction
bool checkXMAS(const vector<string>& grid, int row, int col, int dRow, int dCol) {
    const string target = "XMAS";
    int rows = grid.size();
    int cols = grid[0].size();
    
    for (int i = 0; i < 4; i++) {  // XMAS has 4 characters
        int newRow = row + i * dRow;
        int newCol = col + i * dCol;
        
        if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols ||
            grid[newRow][newCol] != target[i]) {
            return false;
        }
    }
    return true;
}

// Function to check if MAS exists starting from a position in a given direction
bool checkMAS(const vector<string>& grid, int row, int col, int dRow, int dCol) {
    const string target = "MAS";
    const string targetRev = "SAM";  // reversed version
    int rows = grid.size();
    int cols = grid[0].size();
    
    // Checks for both forward and backward MAS
    bool forwardMatch = true;
    bool backwardMatch = true;
    
    for (int i = 0; i < 3; i++) {  // MAS has 3 characters
        int newRow = row + i * dRow;
        int newCol = col + i * dCol;
        
        if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols) {
            return false;
        }
        
        if (grid[newRow][newCol] != target[i]) {
            forwardMatch = false;
        }
        if (grid[newRow][newCol] != targetRev[i]) {
            backwardMatch = false;
        }
    }
    return forwardMatch || backwardMatch;
}

// Function to check for X-MAS pattern at a given position
bool checkXPattern(const vector<string>& grid, int row, int col) {
    // Check diagonal down-right and diagonal up-right
    bool downRight = checkMAS(grid, row, col, 1, 1);
    bool upRight = checkMAS(grid, row + 2, col, -1, 1);
    
    return downRight && upRight;
}

// Add struct to hold results
struct Results {
    int part1;
    int part2;
    
    Results(int p1 = 0, int p2 = 0) : part1(p1), part2(p2) {}
};

// Modify solvePart1 to return int instead of void
int solvePart1(const vector<string>& input) {
    int count = 0;
    int rows = input.size();
    int cols = input[0].size();
    
    // Direction vectors for all 8 directions
    vector<pair<int, int>> directions = {
        {0, 1},   // right
        {1, 0},   // down
        {1, 1},   // diagonal down-right
        {-1, 1},  // diagonal up-right
        {0, -1},  // left
        {-1, 0},  // up
        {-1, -1}, // diagonal up-left
        {1, -1}   // diagonal down-left
    };
    
    // Checks each position as a potential starting point
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Tries all 8 directions from this position
            for (const auto& dir : directions) {
                if (checkXMAS(input, i, j, dir.first, dir.second)) {
                    count++;
                }
            }
        }
    }
    
    cout << "Part 1 Results: " << count << endl;
    return count;
}

// Modify solvePart2 to return int instead of void
int solvePart2(const vector<string>& input) {
    int count = 0;
    int rows = input.size();
    int cols = input[0].size();
    
    // Checks each position as a potential starting point of the X pattern
    for (int i = 0; i < rows - 2; i++) {
        for (int j = 0; j < cols - 2; j++) {
            if (checkXPattern(input, i, j)) {
                count++;
            }
        }
    }
    
    cout << "Part 2 Results: " << count << endl;
    return count;
}

int main() {
    // Reads input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solves both parts
    cout << "Solving Day 4 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    Results results;
    results.part1 = solvePart1(input);
    cout << "----------------------" << endl;
    results.part2 = solvePart2(input);
    
    return 0;
} 