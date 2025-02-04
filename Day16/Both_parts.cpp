#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <tuple>
#include <limits>
#include <regex>
#include <map>

using namespace std;

// Direction vectors for Up(0), Right(1), Down(2), Left(3)
const vector<pair<int, int>> DIRECTIONS = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

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

// Find start and end positions
pair<pair<int, int>, pair<int, int>> findStartEnd(const vector<string>& maze) {
    pair<int, int> start, end;
    for (int i = 0; i < maze.size(); i++) {
        for (int j = 0; j < maze[i].size(); j++) {
            if (maze[i][j] == 'S') start = {i, j};
            if (maze[i][j] == 'E') end = {i, j};
        }
    }
    return {start, end};
}

// Function to convert string to vector of integers
vector<int> ints(const string& s) {
    vector<int> result;
    regex re("-?\\d+");
    auto words_begin = sregex_iterator(s.begin(), s.end(), re);
    auto words_end = sregex_iterator();
    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        result.push_back(stoi(i->str()));
    }
    return result;
}

// Solution for Part 1
void solvePart1(const vector<string>& maze) {
    auto [start, end] = findStartEnd(maze);
    int rows = maze.size(), cols = maze[0].size();
    
    // Priority queue for Dijkstra's algorithm
    priority_queue<tuple<long long, int, int, int>, vector<tuple<long long, int, int, int>>, greater<tuple<long long, int, int, int>>> pq;
    
    // Set to keep track of visited states (position + direction)
    set<tuple<int, int, int>> visited;
    
    // Start facing East (direction 1)
    pq.push({0, start.first, start.second, 1});
    
    long long best = numeric_limits<long long>::max();
    
    while (!pq.empty()) {
        auto [d, r, c, dir] = pq.top();
        pq.pop();
        
        if (visited.count({r, c, dir})) continue;
        visited.insert({r, c, dir});
        
        if (r == end.first && c == end.second && best == numeric_limits<long long>::max()) {
            best = d;
        }
        
        // Move in the current direction
        int dr = DIRECTIONS[dir].first;
        int dc = DIRECTIONS[dir].second;
        int rr = r + dr, cc = c + dc;
        
        // Check if move is valid
        if (0 <= cc && cc < cols && 0 <= rr && rr < rows && maze[rr][cc] != '#') {
            pq.push({d + 1, rr, cc, dir});
        }
        
        // Rotate right
        pq.push({d + 1000, r, c, (dir + 1) % 4});
        // Rotate left
        pq.push({d + 1000, r, c, (dir + 3) % 4});
    }
    
    cout << "Part 1 Solution: " << best << endl;
    
    // Part 2 logic
    // ... (similar structure for Part 2, using the same logic as above)
}

// Solution for Part 2
void solvePart2(const vector<string>& maze) {
    auto [start, end] = findStartEnd(maze);
    int rows = maze.size(), cols = maze[0].size();
    
    // Maps to store distances for both forward and backward searches
    map<tuple<int, int, int>, long long> DIST1, DIST2;
    
    // Forward search from start
    {
        priority_queue<tuple<long long, int, int, int>, 
                      vector<tuple<long long, int, int, int>>, 
                      greater<tuple<long long, int, int, int>>> pq;
        set<tuple<int, int, int>> visited;
        
        // Start facing East (direction 1)
        pq.push({0, start.first, start.second, 1});
        long long best = numeric_limits<long long>::max();
        
        while (!pq.empty()) {
            auto [d, r, c, dir] = pq.top();
            pq.pop();
            
            if (DIST1.find({r, c, dir}) == DIST1.end()) {
                DIST1[{r, c, dir}] = d;
            }
            
            if (r == end.first && c == end.second && best == numeric_limits<long long>::max()) {
                best = d;
            }
            
            if (visited.count({r, c, dir})) continue;
            visited.insert({r, c, dir});
            
            // Move forward
            int dr = DIRECTIONS[dir].first;
            int dc = DIRECTIONS[dir].second;
            int rr = r + dr, cc = c + dc;
            
            if (0 <= cc && cc < cols && 0 <= rr && rr < rows && maze[rr][cc] != '#') {
                pq.push({d + 1, rr, cc, dir});
            }
            
            // Rotations
            pq.push({d + 1000, r, c, (dir + 1) % 4});
            pq.push({d + 1000, r, c, (dir + 3) % 4});
        }
        
        // Backward search from end
        priority_queue<tuple<long long, int, int, int>, 
                      vector<tuple<long long, int, int, int>>, 
                      greater<tuple<long long, int, int, int>>> pq2;
        visited.clear();
        
        // Try all directions from end
        for (int dir = 0; dir < 4; dir++) {
            pq2.push({0, end.first, end.second, dir});
        }
        
        while (!pq2.empty()) {
            auto [d, r, c, dir] = pq2.top();
            pq2.pop();
            
            if (DIST2.find({r, c, dir}) == DIST2.end()) {
                DIST2[{r, c, dir}] = d;
            }
            
            if (visited.count({r, c, dir})) continue;
            visited.insert({r, c, dir});
            
            // Move backward (opposite direction)
            int dr = DIRECTIONS[(dir + 2) % 4].first;
            int dc = DIRECTIONS[(dir + 2) % 4].second;
            int rr = r + dr, cc = c + dc;
            
            if (0 <= cc && cc < cols && 0 <= rr && rr < rows && maze[rr][cc] != '#') {
                pq2.push({d + 1, rr, cc, dir});
            }
            
            // Rotations
            pq2.push({d + 1000, r, c, (dir + 1) % 4});
            pq2.push({d + 1000, r, c, (dir + 3) % 4});
        }
        
        // Find positions on optimal paths
        set<pair<int, int>> OK;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                for (int dir = 0; dir < 4; dir++) {
                    auto pos = make_tuple(r, c, dir);
                    // Check if position is on an optimal path
                    if (DIST1.count(pos) && DIST2.count(pos) && 
                        DIST1[pos] + DIST2[pos] == best) {
                        OK.insert({r, c});
                    }
                }
            }
        }
        
        cout << "Part 2 Solution: " << OK.size() << endl;
    }
}

int main() {
    // Read input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solve both parts
    cout << "Solving Day 16 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 