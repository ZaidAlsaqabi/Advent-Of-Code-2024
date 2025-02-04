#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <utility> 

using namespace std;

// Direction vectors for up(0), right(1), down(2), left(3)
const int dx[] = {0, 1, 0, -1};
const int dy[] = {-1, 0, 1, 0};

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

// Find starting position and direction of guard
pair<pair<int, int>, int> findStart(const vector<string>& grid) {
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].length(); x++) {
            if (grid[y][x] == '^') return {{x, y}, 0};  // facing up
            if (grid[y][x] == '>') return {{x, y}, 1};  // facing right
            if (grid[y][x] == 'v') return {{x, y}, 2};  // facing down
            if (grid[y][x] == '<') return {{x, y}, 3};  // facing left
        }
    }
    return {{-1, -1}, -1}; 
}

// Checks if position is within grid bounds
bool isInBounds(int x, int y, const vector<string>& grid) {
    return y >= 0 && y < grid.size() && x >= 0 && x < grid[0].length();
}

// Checks if guard gets stuck in a loop with an obstruction at (obstX, obstY)
bool checksForLoop(const vector<string>& grid, int obstX, int obstY, pair<pair<int, int>, int> start) {
    vector<string> modifiedGrid = grid;
    modifiedGrid[obstY][obstX] = '#';  // Places obstruction
    
    set<pair<pair<int, int>, int>> visited;  // Tracks position AND directions
    int x = start.first.first, y = start.first.second;
    int direction = start.second;
    
    while (true) {
        // Current state
        pair<pair<int, int>, int> state = {{x, y}, direction};
        
        // If we have seen this state before, it's a loop
        if (visited.find(state) != visited.end()) {
            return true;
        }
        visited.insert(state);
        
        // Checks next position
        int nextX = x + dx[direction];
        int nextY = y + dy[direction];
        
        if (!isInBounds(nextX, nextY, modifiedGrid) || modifiedGrid[nextY][nextX] == '#') {
            // Turns right if theres an obstacle
            direction = (direction + 1) % 4;
        } else {
            // Moves forward
            x = nextX;
            y = nextY;
            
            // Check if guard has left the map
            if (!isInBounds(x + dx[direction], y + dy[direction], modifiedGrid) &&
                (x == 0 || x == modifiedGrid[0].length() - 1 || 
                 y == 0 || y == modifiedGrid.size() - 1)) {
                return false;  // Guard escapes, not a loop
            }
        }
    }
}

// Results for Part 1
void solvePart1(const vector<string>& input) {
    auto [start, direction] = findStart(input);
    if (start.first == -1) {
        cout << "No starting position found!" << endl;
        return;
    }

    set<pair<int, int>> visited;
    int x = start.first, y = start.second;
    visited.insert({x, y});

    while (true) {
        // Checks if there's an obstacle in front
        int nextX = x + dx[direction];
        int nextY = y + dy[direction];
        
        if (!isInBounds(nextX, nextY, input) || input[nextY][nextX] == '#') {
            // Turns right if obstacle
            direction = (direction + 1) % 4;
        } else {
            // Moves forward
            x = nextX;
            y = nextY;
            visited.insert({x, y});
            
            // Checks if guard has left the map
            if (!isInBounds(x + dx[direction], y + dy[direction], input) &&
                (x == 0 || x == input[0].length() - 1 || 
                 y == 0 || y == input.size() - 1)) {
                break;
            }
        }
    }

    cout << "Part 1 Results: " << visited.size() << endl;
}

// Results for Part 2
void solvePart2(const vector<string>& input) {
    auto start = findStart(input);
    if (start.first.first == -1) {
        cout << "No starting position found!" << endl;
        return;
    }
    
    int loopCount = 0;
    
    // Try placing obstruction at each empty position
    for (int y = 0; y < input.size(); y++) {
        for (int x = 0; x < input[y].length(); x++) {
            // Skips if not empty space or if it's the guard's starting position
            if (input[y][x] != '.' || (x == start.first.first && y == start.first.second)) {
                continue;
            }
            
            // Checks if placing obstruction here creates a loop
            if (checksForLoop(input, x, y, start)) {
                loopCount++;
            }
        }
    }
    
    cout << "Part 2 Results: " << loopCount << endl;
}

int main() {
    // Reads input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solves both parts
    cout << "Solving Day 6 Puzzle 👮‍♂️ (Part 2 takes a while as it loops through all empty spaces)..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 