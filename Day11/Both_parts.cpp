#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

// Forward declarations
vector<string> readInput();
void solvePart1(const vector<string>& input);
void solvePart2(const vector<string>& input);
vector<long long> transformStone(long long n);
map<long long, long long> blink(const map<long long, long long>& stoneCount);
long long totalStones(const map<long long, long long>& stoneCount);

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

// Function to transform a stone
vector<long long> transformStone(long long n) {
    vector<long long> result;
    string s = to_string(n);
    
    if (n == 0) {
        result.push_back(1);
    }
    else if (s.length() % 2 == 0) {
        int half = s.length() / 2;
        result.push_back(stoll(s.substr(0, half)));
        result.push_back(stoll(s.substr(half)));
    }
    else {
        result.push_back(n * 2024LL);
    }
    
    return result;
}

// Function to blink stones
map<long long, long long> blink(const map<long long, long long>& stoneCount) {
    map<long long, long long> newStones;
    
    for (const auto& [stone, count] : stoneCount) {
        vector<long long> transformed = transformStone(stone);
        for (long long t : transformed) {
            newStones[t] += count;
        }
    }
    
    return newStones;
}

// Function to count total stones
long long totalStones(const map<long long, long long>& stoneCount) {
    long long total = 0;
    for (const auto& [stone, count] : stoneCount) {
        total += count;
    }
    return total;
}

// Solution for Part 1
void solvePart1(const vector<string>& input) {
    // Parse initial stones
    vector<long long> initialStones;
    stringstream ss(input[0]);
    long long stone;
    while (ss >> stone) {
        initialStones.push_back(stone);
    }

    // Convert to stone count map
    map<long long, long long> stoneCount;
    for (long long stone : initialStones) {
        stoneCount[stone]++;
    }

    // Process 25 blinks
    auto s = stoneCount;
    for (int i = 0; i < 25; i++) {
        s = blink(s);
    }
    
    cout << "Part 1 Solution: " << totalStones(s) << endl;
}

// Solution for Part 2
void solvePart2(const vector<string>& input) {
    // Parse initial stones
    vector<long long> initialStones;
    stringstream ss(input[0]);
    long long stone;
    while (ss >> stone) {
        initialStones.push_back(stone);
    }

    // Convert to stone count map
    map<long long, long long> stoneCount;
    for (long long stone : initialStones) {
        stoneCount[stone]++;
    }

    // Process 75 blinks
    auto s = stoneCount;
    for (int i = 0; i < 75; i++) {
        s = blink(s);
    }
    
    cout << "Part 2 Solution: " << totalStones(s) << endl;
}

int main() {
    // Read input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solve both parts
    cout << "Solving Day 11 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 