#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <functional>

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

// Helper function to split string by delimiter
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    size_t start = 0, end;
    while ((end = s.find(delimiter, start)) != string::npos) {
        token = s.substr(start, end - start);
        tokens.push_back(token);
        start = end + 1;
    }
    tokens.push_back(s.substr(start));
    return tokens;
}

// Solution for Part 1
void solvePart1(const vector<string>& input) {
    // Create adjacency list representation of the network
    unordered_map<string, unordered_set<string>> network;
    
    // Build the network from input
    for (const string& line : input) {
        vector<string> computers = split(line, '-');
        network[computers[0]].insert(computers[1]);
        network[computers[1]].insert(computers[0]); // Undirected connection
    }
    
    // Set to store unique triplets
    set<set<string>> triplets;
    
    // Find all sets of three interconnected computers
    for (const auto& [comp1, neighbors1] : network) {
        for (const string& comp2 : neighbors1) {
            for (const string& comp3 : network[comp2]) {
                // Check if comp3 is connected to comp1 to form a triangle
                if (comp3 != comp1 && network[comp1].count(comp3)) {
                    set<string> triplet = {comp1, comp2, comp3};
                    triplets.insert(triplet);
                }
            }
        }
    }
    
    // Count triplets containing at least one computer starting with 't'
    int count = 0;
    for (const auto& triplet : triplets) {
        bool hasT = false;
        for (const string& comp : triplet) {
            if (comp[0] == 't') {
                hasT = true;
                break;
            }
        }
        if (hasT) count++;
    }
    
    cout << "Part 1 Solution: " << count << endl;
}

// Solution for Part 2
void solvePart2(const vector<string>& input) {
    // Create adjacency list representation of the network
    unordered_map<string, unordered_set<string>> network;
    
    // Build the network from input
    for (const string& line : input) {
        vector<string> computers = split(line, '-');
        network[computers[0]].insert(computers[1]);
        network[computers[1]].insert(computers[0]); // Undirected connection
    }
    
    // Get all computers
    vector<string> computers;
    for (const auto& [comp, _] : network) {
        computers.push_back(comp);
    }
    sort(computers.begin(), computers.end());
    
    // Find maximum clique using a simpler approach
    set<string> maxClique;
    
    // Try each computer as a potential clique member
    for (const string& start : computers) {
        set<string> potentialClique = {start};
        set<string> candidates;
        
        // Add all neighbors as candidates
        for (const string& neighbor : network[start]) {
            candidates.insert(neighbor);
        }
        
        // Keep adding computers that are connected to all current clique members
        while (!candidates.empty()) {
            bool added = false;
            for (auto it = candidates.begin(); it != candidates.end(); ) {
                bool connectsToAll = true;
                for (const string& member : potentialClique) {
                    if (network[*it].count(member) == 0) {
                        connectsToAll = false;
                        break;
                    }
                }
                
                if (connectsToAll) {
                    potentialClique.insert(*it);
                    it = candidates.erase(it);
                    added = true;
                } else {
                    ++it;
                }
            }
            
            if (!added) break;
        }
        
        if (potentialClique.size() > maxClique.size()) {
            maxClique = potentialClique;
        }
    }
    
    // Generate password from maxClique
    string password;
    for (const string& comp : maxClique) {
        if (!password.empty()) password += ",";
        password += comp;
    }
    
    cout << "Part 2 Solution: " << password << endl;
}

int main() {
    // Read input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solve both parts
    cout << "Solving Day 23 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 