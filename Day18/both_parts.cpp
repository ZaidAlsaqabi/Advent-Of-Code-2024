#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <sstream>

using namespace std;

// Structure to represent a point in the grid
struct Point {
    int x, y;
    int steps;
    Point(int x = 0, int y = 0, int steps = 0) : x(x), y(y), steps(steps) {}
};

// Function to read input from file
vector<Point> readInput() {
    vector<Point> points;
    string line;
    ifstream file("input.txt");
    
    if (!file.is_open()) {
        cerr << "Error: Could not open input.txt" << endl;
        return points;
    }

    while (getline(file, line)) {
        stringstream ss(line);
        int x, y;
        char comma;
        ss >> x >> comma >> y;
        points.push_back(Point(x, y));
    }

    file.close();
    return points;
}

// Check if a point is within grid bounds
bool isValid(int x, int y, int gridSize) {
    return x >= 0 && x < gridSize && y >= 0 && y < gridSize;
}

// Find shortest path using BFS
int findShortestPath(const vector<vector<bool>>& grid) {
    const int gridSize = grid.size();
    vector<vector<bool>> visited(gridSize, vector<bool>(gridSize, false));
    
    // Direction arrays for moving up, right, down, left
    const int dx[] = {-1, 0, 1, 0};
    const int dy[] = {0, 1, 0, -1};
    
    queue<Point> q;
    q.push(Point(0, 0, 0));
    visited[0][0] = true;
    
    while (!q.empty()) {
        Point current = q.front();
        q.pop();
        
        // If we reached the destination
        if (current.x == gridSize-1 && current.y == gridSize-1) {
            return current.steps;
        }
        
        // Try all four directions
        for (int i = 0; i < 4; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];
            
            if (isValid(newX, newY, gridSize) && !grid[newX][newY] && !visited[newX][newY]) {
                visited[newX][newY] = true;
                q.push(Point(newX, newY, current.steps + 1));
            }
        }
    }
    
    return -1; // No path found
}

// Solution for Part 1
void solvePart1(const vector<Point>& input) {
    const int gridSize = 71; // 0 to 70 inclusive
    vector<vector<bool>> grid(gridSize, vector<bool>(gridSize, false));
    
    // Mark corrupted locations (first 1024 bytes)
    int numBytes = min(1024, static_cast<int>(input.size()));
    for (int i = 0; i < numBytes; i++) {
        grid[input[i].x][input[i].y] = true;
    }
    
    // Find shortest path
    int shortestPath = findShortestPath(grid);
    
    cout << "Part 1 Solution: " << shortestPath << endl;
}

// Solution for Part 2
void solvePart2(const vector<Point>& input) {
    const int gridSize = 71; // 0 to 70 inclusive
    vector<vector<bool>> grid(gridSize, vector<bool>(gridSize, false));
    
    // Try each byte one by one until path is blocked
    for (size_t i = 0; i < input.size(); i++) {
        // Save current byte coordinates
        int blockingX = input[i].x;
        int blockingY = input[i].y;
        
        // Add this byte to grid
        grid[blockingX][blockingY] = true;
        
        // Check if path still exists
        int pathLength = findShortestPath(grid);
        
        // If no path exists, this is the blocking byte
        if (pathLength == -1) {
            cout << "Part 2 Solution: " << blockingX << "," << blockingY << endl;
            return;
        }
    }
    
    cout << "Part 2 Solution: No blocking byte found" << endl;
}

int main() {
    // Read input
    vector<Point> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solve both parts
    cout << "Solving Day 18 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 