#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

using namespace std;

// Structure to store the rules and updates
struct PuzzleInput {
    vector<pair<int, int>> rules;  // Stores X|Y rules
    vector<vector<int>> updates;   // Stores the updates to validate
};

// Function to split string by delimiter
vector<string> split(const string& str, char delim) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to read and parse input
PuzzleInput readInput() {
    PuzzleInput input;
    string line;
    ifstream file("input.txt");
    bool parsingRules = true;
    
    if (!file.is_open()) {
        cerr << "No input file found" << endl;
        return input;
    }

    while (getline(file, line)) {
        if (line.empty()) {
            parsingRules = false;
            continue;
        }

        if (parsingRules) {
            // Parse rules (X|Y format)
            auto parts = split(line, '|');
            if (parts.size() == 2) {
                input.rules.push_back({
                    stoi(parts[0]),
                    stoi(parts[1])
                });
            }
        } else {
            // Parse updates (comma-separated numbers)
            vector<int> update;
            auto numbers = split(line, ',');
            for (const auto& num : numbers) {
                update.push_back(stoi(num));
            }
            input.updates.push_back(update);
        }
    }

    file.close();
    return input;
}

// Function to check if an update sequence is valid according to rules
bool isValidUpdate(const vector<int>& update, const vector<pair<int, int>>& rules) {
    // Creates a map of page positions for quick lookup
    unordered_map<int, int> pagePositions;
    for (int i = 0; i < update.size(); i++) {
        pagePositions[update[i]] = i;
    }

    // Checks each applicable rule
    for (const auto& rule : rules) {
        int page1 = rule.first;
        int page2 = rule.second;
        
        // Skip rule if either page is not in the update
        if (pagePositions.find(page1) == pagePositions.end() ||
            pagePositions.find(page2) == pagePositions.end()) {
            continue;
        }

        // Check if pages are in correct order
        if (pagePositions[page1] >= pagePositions[page2]) {
            return false;
        }
    }

    return true;
}

// New function to topologically sort pages based on rules
vector<int> topologicalSort(const vector<int>& update, const vector<pair<int, int>>& rules) {
    // Builds adjacency list
    unordered_map<int, vector<int>> graph;
    unordered_map<int, int> inDegree;
    
    // Initialize for all pages in update
    for (int page : update) {
        graph[page] = vector<int>();
        inDegree[page] = 0;
    }
    
    // Add edges based on applicable rules
    for (const auto& rule : rules) {
        int page1 = rule.first;
        int page2 = rule.second;
        
        // Only consider rules where both pages are in the update
        if (graph.find(page1) != graph.end() && graph.find(page2) != graph.end()) {
            graph[page1].push_back(page2);
            inDegree[page2]++;
        }
    }
    
    vector<int> result;
    vector<int> noIncoming;
    
    // Find all vertices with no incoming edges
    for (const auto& pair : inDegree) {
        if (pair.second == 0) {
            noIncoming.push_back(pair.first);
        }
    }
    
    while (!noIncoming.empty()) {
        int current = noIncoming.back();
        noIncoming.pop_back();
        result.push_back(current);
        
        for (int neighbor : graph[current]) {
            inDegree[neighbor]--;
            if (inDegree[neighbor] == 0) {
                noIncoming.push_back(neighbor);
            }
        }
    }
    
    return result;
}

// Results for Part 1
void solvePart1(const PuzzleInput& input) {
    int sum = 0;
    
    for (const auto& update : input.updates) {
        if (isValidUpdate(update, input.rules)) {
            // Get middle page number
            int middleIndex = update.size() / 2;
            sum += update[middleIndex];
        }
    }
    
    cout << "Part 1 Results: " << sum << endl;
}

// Results for Part 2
void solvePart2(const PuzzleInput& input) {
    int sum = 0;
    
    for (const auto& update : input.updates) {
        // Only process updates that are not valid
        if (!isValidUpdate(update, input.rules)) {
            // Get correctly ordered version
            vector<int> orderedUpdate = topologicalSort(update, input.rules);
            
            // Get middle page number
            int middleIndex = orderedUpdate.size() / 2;
            sum += orderedUpdate[middleIndex];
        }
    }
    
    cout << "Part 2 Results: " << sum << endl;
}

int main() {
    // Read input
    PuzzleInput input = readInput();
    
    if (input.rules.empty() || input.updates.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solve both parts
    cout << "Solving Day 5 Puzzle🖨..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 