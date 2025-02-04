#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include <map>

using namespace std;

// Structure to represent a point on the grid
struct Point {
    int x, y;
    
    bool operator<(const Point& other) const {
        return tie(x, y) < tie(other.x, other.y);
    }
};

// Structure to represent an antenna
struct Antenna {
    int x, y;
    char frequency;
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

// Function to find all antennas in the grid
vector<Antenna> findAntennas(const vector<string>& grid) {
    vector<Antenna> antennas;
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].length(); x++) {
            char c = grid[y][x];
            if (c != '.' && c != '#') {
                antennas.push_back({x, y, c});
            }
        }
    }
    return antennas;
}

// Function to calculate antinodes between two antennas
vector<Point> calculateAntinodes(const Antenna& a1, const Antenna& a2, int maxX, int maxY) {
    vector<Point> antinodes;
    
    // Only process antennas with the same frequency
    if (a1.frequency != a2.frequency) {
        return antinodes;
    }
    
    // Calculate distances and check for 2:1 ratio points
    int dx = a2.x - a1.x;
    int dy = a2.y - a1.y;
    
    // Try both antennas as the closer one
    for (int i = 0; i < 2; i++) {
        // Calculate potential antinode position
        int nodeX = i == 0 ? a1.x + 2 * dx : a2.x - 2 * dx;
        int nodeY = i == 0 ? a1.y + 2 * dy : a2.y - 2 * dy;
        
        // Check if antinode is within bounds
        if (nodeX >= 0 && nodeX < maxX && nodeY >= 0 && nodeY < maxY) {
            antinodes.push_back({nodeX, nodeY});
        }
    }
    
    return antinodes;
}

// Function to check if three points are collinear
bool areCollinear(const Point& p1, const Point& p2, const Point& p3) {
    // Using the slope formula: (y2-y1)/(x2-x1) = (y3-y1)/(x3-x1)
    // Rearranged to avoid division: (y2-y1)(x3-x1) = (y3-y1)(x2-x1)
    return (p2.y - p1.y) * (p3.x - p1.x) == (p3.y - p1.y) * (p2.x - p1.x);
}

// Function to find all antinodes for Part 2
set<Point> findPart2Antinodes(const vector<Antenna>& antennas, int maxX, int maxY) {
    set<Point> antinodes;
    
    // Group antennas by frequency
    map<char, vector<Point>> freqGroups;
    for (const Antenna& ant : antennas) {
        freqGroups[ant.frequency].push_back({ant.x, ant.y});
    }
    
    // For each frequency group
    for (const auto& [freq, points] : freqGroups) {
        // Skip if there's only one antenna of this frequency
        if (points.size() < 2) continue;
        
        // Add antenna positions as antinodes (except for single frequency antennas)
        for (const Point& p : points) {
            antinodes.insert(p);
        }
        
        // Check all grid positions for collinearity with any two antennas
        for (int y = 0; y < maxY; y++) {
            for (int x = 0; x < maxX; x++) {
                Point current{x, y};
                
                // Check each pair of antennas
                for (size_t i = 0; i < points.size(); i++) {
                    for (size_t j = i + 1; j < points.size(); j++) {
                        if (areCollinear(points[i], points[j], current)) {
                            antinodes.insert(current);
                        }
                    }
                }
            }
        }
    }
    
    return antinodes;
}

// Solution for Part 1
void solvePart1(const vector<string>& input) {
    vector<Antenna> antennas = findAntennas(input);
    set<Point> uniqueAntinodes;
    
    // Calculate antinodes for each pair of antennas
    for (size_t i = 0; i < antennas.size(); i++) {
        for (size_t j = i + 1; j < antennas.size(); j++) {
            vector<Point> newAntinodes = calculateAntinodes(
                antennas[i], antennas[j], 
                input[0].length(), input.size()
            );
            
            // Add new antinodes to set (automatically handles duplicates)
            for (const Point& p : newAntinodes) {
                uniqueAntinodes.insert(p);
            }
        }
    }
    
    cout << "Part 1 Solution: " << uniqueAntinodes.size() << endl;
}

// Update Part 2 solution
void solvePart2(const vector<string>& input) {
    vector<Antenna> antennas = findAntennas(input);
    set<Point> antinodes = findPart2Antinodes(antennas, input[0].length(), input.size());
    cout << "Part 2 Solution: " << antinodes.size() << endl;
}

int main() {
    // Read input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solve both parts
    cout << "Solving Day 8 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 