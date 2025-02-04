#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <utility>

using namespace std;

// Structure to represent a position
struct Position {
    int row, col;
    
    Position(int r = 0, int c = 0) : row(r), col(c) {}
    
    bool operator<(const Position& other) const {
        return row < other.row || (row == other.row && col < other.col);
    }
    
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

// Structure for queue elements in pathfinding
struct QueueElement {
    Position pos;
    vector<Position> path;
    
    QueueElement(Position p, vector<Position> pth) : pos(p), path(pth) {}
};

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

// Find path from start to end
vector<Position> findPath(const vector<string>& map, Position start, Position end) {
    set<Position> visited;
    queue<QueueElement> q;
    q.push(QueueElement(start, {start}));
    
    const vector<pair<int, int>> directions = {{0,1}, {0,-1}, {1,0}, {-1,0}};
    
    while (!q.empty()) {
        Position current = q.front().pos;
        vector<Position> currentPath = q.front().path;
        q.pop();
        
        if (current == end) {
            return currentPath;
        }
        
        if (visited.find(current) != visited.end()) {
            continue;
        }
        
        visited.insert(current);
        
        for (const auto& dir : directions) {
            int newRow = current.row + dir.first;
            int newCol = current.col + dir.second;
            
            if (newRow < 0 || newCol < 0 || newRow >= map.size() || newCol >= map[0].size()) {
                continue;
            }
            
            Position next(newRow, newCol);
            if (visited.find(next) != visited.end() || map[newRow][newCol] == '#') {
                continue;
            }
            
            vector<Position> newPath = currentPath;
            newPath.push_back(next);
            q.push(QueueElement(next, newPath));
        }
    }
    
    return vector<Position>();
}

// Find possible jumps and their time savings
map<pair<Position, Position>, int> findJumps(const vector<Position>& path, int dist) {
    map<Position, int> positionCost;
    for (size_t i = 0; i < path.size(); i++) {
        positionCost[path[i]] = i;
    }
    
    map<pair<Position, Position>, int> jumps;
    
    for (const auto& [pos, step] : positionCost) {
        for (int dr = -dist; dr <= dist; dr++) {
            int colSpan = dist - abs(dr);
            for (int dc = -colSpan; dc <= colSpan; dc++) {
                Position next(pos.row + dr, pos.col + dc);
                int pathSpan = abs(dr) + abs(dc);
                
                auto it = positionCost.find(next);
                if (it == positionCost.end()) continue;
                int nextStep = it->second;
                
                if (nextStep - pathSpan <= step) continue;
                
                int saving = (nextStep - step) - pathSpan;
                if (saving > 0) {
                    jumps[{pos, next}] = saving;
                }
            }
        }
    }
    
    return jumps;
}

// Solution for Part 1
void solvePart1(const vector<string>& input) {
    Position start, end;
    
    // Find start and end positions
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            if (input[i][j] == 'S') start = Position(i, j);
            else if (input[i][j] == 'E') end = Position(i, j);
        }
    }
    
    vector<Position> path = findPath(input, start, end);
    auto jumps = findJumps(path, 2);
    
    int count = 0;
    for (const auto& [jump, saving] : jumps) {
        if (saving >= 100) count++;
    }
    
    cout << "Part 1 Solution: " << count << endl;
}

// Solution for Part 2
void solvePart2(const vector<string>& input) {
    Position start, end;
    
    // Find start and end positions
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            if (input[i][j] == 'S') start = Position(i, j);
            else if (input[i][j] == 'E') end = Position(i, j);
        }
    }
    
    vector<Position> path = findPath(input, start, end);
    auto jumps = findJumps(path, 20);
    
    int count = 0;
    for (const auto& [jump, saving] : jumps) {
        if (saving >= 100) count++;
    }
    
    cout << "Part 2 Solution: " << count << endl;
}

int main() {
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    cout << "Solving Day 20 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 