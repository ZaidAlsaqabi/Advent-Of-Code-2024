#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <set>

using namespace std;

// Direction vectors for up, right, down, left
const int dx[] = {-1, 0, 1, 0};
const int dy[] = {0, 1, 0, -1};

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

// Structure to represent a position in the grid
struct Position {
    int x, y, height;
    Position(int x, int y, int h) : x(x), y(y), height(h) {}
};

// Function to check if a position is within grid bounds
bool isValid(int x, int y, const vector<string>& grid) {
    return x >= 0 && x < grid.size() && y >= 0 && y < grid[0].size();
}

// Function to count reachable 9s from a trailhead
int countReachableNines(int startX, int startY, const vector<string>& grid) {
    set<pair<int, int>> visitedNines;
    queue<Position> q;
    set<pair<int, int>> visited;
    
    q.push(Position(startX, startY, 0));
    visited.insert({startX, startY});

    while (!q.empty()) {
        Position current = q.front();
        q.pop();

        // If we reach a 9, add it to visitedNines
        if (current.height == 9) {
            visitedNines.insert({current.x, current.y});
            continue;
        }

        // Try all four directions
        for (int i = 0; i < 4; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            if (isValid(newX, newY, grid)) {
                int newHeight = grid[newX][newY] - '0';
                
                // Check if it's a valid next step (exactly 1 higher)
                if (newHeight == current.height + 1) {
                    pair<int, int> newPos = {newX, newY};
                    if (visited.find(newPos) == visited.end()) {
                        visited.insert(newPos);
                        q.push(Position(newX, newY, newHeight));
                    }
                }
            }
        }
    }

    return visitedNines.size();
}

// Function to find all possible paths from a trailhead to any 9
void findPaths(int x, int y, const vector<string>& grid, vector<vector<bool>>& visited, 
               vector<pair<int, int>>& currentPath, set<vector<pair<int, int>>>& allPaths, int currentHeight) {
    // Mark current position as visited
    visited[x][y] = true;
    currentPath.push_back({x, y});
    
    // If we reach a 9, we found a valid path
    if (grid[x][y] - '0' == 9) {
        allPaths.insert(currentPath);
    } else {
        // Try all four directions
        for (int i = 0; i < 4; i++) {
            int newX = x + dx[i];
            int newY = y + dy[i];
            
            if (isValid(newX, newY, grid) && !visited[newX][newY]) {
                int newHeight = grid[newX][newY] - '0';
                if (newHeight == currentHeight + 1) {
                    findPaths(newX, newY, grid, visited, currentPath, allPaths, newHeight);
                }
            }
        }
    }
    
    // Backtrack
    visited[x][y] = false;
    currentPath.pop_back();
}

// Function to get the rating of a trailhead
int getTrailheadRating(int startX, int startY, const vector<string>& grid) {
    vector<vector<bool>> visited(grid.size(), vector<bool>(grid[0].size(), false));
    vector<pair<int, int>> currentPath;
    set<vector<pair<int, int>>> allPaths;
    
    findPaths(startX, startY, grid, visited, currentPath, allPaths, 0);
    
    return allPaths.size();
}

// Solution for Part 1
void solvePart1(const vector<string>& input) {
    int totalScore = 0;

    // Find all trailheads (positions with height 0)
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            if (input[i][j] == '0') {
                int score = countReachableNines(i, j, input);
                totalScore += score;
            }
        }
    }

    cout << "Part 1 Solution: " << totalScore << endl;
}

// Solution for Part 2
void solvePart2(const vector<string>& input) {
    int totalRating = 0;

    // Find all trailheads (positions with height 0)
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            if (input[i][j] == '0') {
                int rating = getTrailheadRating(i, j, input);
                totalRating += rating;
            }
        }
    }

    cout << "Part 2 Solution: " << totalRating << endl;
}

int main() {
    // Read input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solve both parts
    cout << "Solving Day 10 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 