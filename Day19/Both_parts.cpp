#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>

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

// Function to split string by delimiter
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        // Remove whitespace
        token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

// Function to check if a pattern can be constructed from available towels
bool canConstructPattern(const string& pattern, const vector<string>& availableTowels, 
                        unordered_set<string>& memo, int startPos = 0) {
    if (startPos == pattern.length()) return true;
    if (memo.count(pattern.substr(startPos))) return false;
    
    for (const string& towel : availableTowels) {
        if (startPos + towel.length() <= pattern.length() && 
            pattern.substr(startPos, towel.length()) == towel) {
            if (canConstructPattern(pattern, availableTowels, memo, startPos + towel.length())) {
                return true;
            }
        }
    }
    
    memo.insert(pattern.substr(startPos));
    return false;
}

// Function to count all possible ways to construct a pattern
long long countPatternConstructions(const string& pattern, const vector<string>& availableTowels,
                                  unordered_map<string, long long>& memo, int startPos = 0) {
    if (startPos == pattern.length()) return 1;
    
    string key = pattern.substr(startPos);
    if (memo.count(key)) return memo[key];
    
    long long totalWays = 0;
    for (const string& towel : availableTowels) {
        if (startPos + towel.length() <= pattern.length() && 
            pattern.substr(startPos, towel.length()) == towel) {
            totalWays += countPatternConstructions(pattern, availableTowels, memo, 
                                                 startPos + towel.length());
        }
    }
    
    memo[key] = totalWays;
    return totalWays;
}

// Solution for Part 1
void solvePart1(const vector<string>& input) {
    // Parse available towel patterns
    vector<string> availableTowels = split(input[0], ',');
    
    // Count possible designs
    int possibleDesigns = 0;
    for (size_t i = 2; i < input.size(); i++) {
        string design = input[i];
        unordered_set<string> memo;
        if (canConstructPattern(design, availableTowels, memo)) {
            possibleDesigns++;
        }
    }
    
    cout << "Part 1 Solution: " << possibleDesigns << endl;
}

// Updated Solution for Part 2
void solvePart2(const vector<string>& input) {
    // Parse available towel patterns
    vector<string> availableTowels = split(input[0], ',');
    
    // Count total ways for all designs
    long long totalWays = 0;
    for (size_t i = 2; i < input.size(); i++) {
        string design = input[i];
        unordered_map<string, long long> memo;
        long long ways = countPatternConstructions(design, availableTowels, memo);
        totalWays += ways;
        
        // Optional: Print individual results
        // cout << "Design " << design << ": " << ways << " ways" << endl;
    }
    
    cout << "Part 2 Solution: " << totalWays << endl;
}

int main() {
    // Read input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solve both parts
    cout << "Solving Day 19 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 