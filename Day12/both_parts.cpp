#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <queue>
#include <utility>

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

// Helper function to check if a position is valid
bool isValid(int row, int col, const vector<string>& grid) {
    return row >= 0 && row < grid.size() && col >= 0 && col < grid[0].size();
}

// Function to calculate area and perimeter of a region
pair<int, int> calculateRegionStats(const vector<string>& grid, vector<vector<bool>>& visited, 
                                  int startRow, int startCol, char plantType) {
    int area = 0;
    int perimeter = 0;
    queue<pair<int, int>> q;
    q.push({startRow, startCol});
    visited[startRow][startCol] = true;

    // Direction arrays for 4-directional movement
    vector<int> dr = {-1, 1, 0, 0};
    vector<int> dc = {0, 0, -1, 1};

    while (!q.empty()) {
        int row = q.front().first;
        int col = q.front().second;
        q.pop();
        area++;

        // Check all four directions
        for (int i = 0; i < 4; i++) {
            int newRow = row + dr[i];
            int newCol = col + dc[i];

            if (!isValid(newRow, newCol, grid) || grid[newRow][newCol] != plantType) {
                perimeter++;
                continue;
            }

            if (!visited[newRow][newCol]) {
                visited[newRow][newCol] = true;
                q.push({newRow, newCol});
            }
        }
    }

    return {area, perimeter};
}

// Helper function to count distinct sides of a region
pair<int, int> calculateRegionStats2(const vector<string>& grid, vector<vector<bool>>& visited,
                                   int startRow, int startCol, char plantType) {
    int area = 0;
    set<pair<pair<int, int>, pair<int, int>>> sides;  // Store unique sides
    queue<pair<int, int>> q;
    q.push({startRow, startCol});
    visited[startRow][startCol] = true;

    // Direction arrays for 4-directional movement
    vector<int> dr = {-1, 1, 0, 0};
    vector<int> dc = {0, 0, -1, 1};

    while (!q.empty()) {
        int row = q.front().first;
        int col = q.front().second;
        q.pop();
        area++;

        // Check all four directions
        for (int i = 0; i < 4; i++) {
            int newRow = row + dr[i];
            int newCol = col + dc[i];

            if (!isValid(newRow, newCol, grid) || grid[newRow][newCol] != plantType) {
                // Add this side to our set of sides
                // We'll store both points that define the side, ordered to avoid duplicates
                pair<int, int> p1 = {row, col};
                pair<int, int> p2;
                
                // Calculate the other endpoint of this side based on direction
                if (i < 2) { // Vertical side
                    p2 = {row, col + 1};
                } else { // Horizontal side
                    p2 = {row + 1, col};
                }
                
                // Order points to ensure consistent representation
                if (p1 > p2) swap(p1, p2);
                sides.insert({p1, p2});
                continue;
            }

            if (!visited[newRow][newCol]) {
                visited[newRow][newCol] = true;
                q.push({newRow, newCol});
            }
        }
    }

    return {area, sides.size()};
}

// Solution for Part 1
void solvePart1(const vector<string>& input) {
    if (input.empty()) return;

    vector<vector<bool>> visited(input.size(), vector<bool>(input[0].size(), false));
    int totalPrice = 0;

    // Iterate through each position in the grid
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            if (!visited[i][j]) {
                auto [area, perimeter] = calculateRegionStats(input, visited, i, j, input[i][j]);
                int price = area * perimeter;
                totalPrice += price;
            }
        }
    }

    cout << "Part 1 Solution: " << totalPrice << endl;
}

// Solution for Part 2
void solvePart2(const vector<string>& input) {
    if (input.empty()) return;

    vector<vector<bool>> visited(input.size(), vector<bool>(input[0].size(), false));
    int totalPrice = 0;

    // Iterate through each position in the grid
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            if (!visited[i][j]) {
                auto [area, sides] = calculateRegionStats2(input, visited, i, j, input[i][j]);
                int price = area * sides;
                totalPrice += price;
            }
        }
    }

    cout << "Part 2 Solution: " << totalPrice << endl;
}

int main() {
    // Read input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solve both parts
    cout << "Solving Day 12 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 