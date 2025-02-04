#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <regex>
#include <cmath>

using namespace std;

// Structure to hold claw machine data
struct ClawMachine {
    int Xa, Ya; // Button A movements
    int Xb, Yb; // Button B movements
    long long Px, Py; // Prize positions
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

// Helper function to parse claw machines from input
vector<ClawMachine> parseClawMachines(const vector<string>& input, long long prizeOffset = 0) {
    vector<ClawMachine> machines;
    ClawMachine currentMachine;
    regex buttonARegex(R"(Button A: X\+(\d+), Y\+(\d+))");
    regex buttonBRegex(R"(Button B: X\+(\d+), Y\+(\d+))");
    regex prizeRegex(R"(Prize: X=(\d+), Y=(\d+))");
    smatch match;

    for (const auto& line : input) {
        if (regex_search(line, match, buttonARegex)) {
            currentMachine.Xa = stoi(match[1]);
            currentMachine.Ya = stoi(match[2]);
        }
        else if (regex_search(line, match, buttonBRegex)) {
            currentMachine.Xb = stoi(match[1]);
            currentMachine.Yb = stoi(match[2]);
        }
        else if (regex_search(line, match, prizeRegex)) {
            currentMachine.Px = stoll(match[1]) + prizeOffset;
            currentMachine.Py = stoll(match[2]) + prizeOffset;
            machines.push_back(currentMachine);
        }
    }

    return machines;
}

// Function to find minimum tokens for a single claw machine using integer arithmetic
// Returns the minimum tokens or -1 if no solution exists
long long findMinTokensPart2(const ClawMachine& machine) {
    long long Xa = machine.Xa;
    long long Xb = machine.Xb;
    long long Ya = machine.Ya;
    long long Yb = machine.Yb;
    long long Px = machine.Px;
    long long Py = machine.Py;
    
    // Calculate the denominator for Cramer's Rule
    long long denominator = Xa * Yb - Xb * Ya;
    if (denominator == 0) {
        // No unique solution exists
        return -1;
    }
    
    // Calculate numerator for b
    long long numerator_b = Py * Xa - Ya * Px;
    
    // Check if denominator divides numerator_b
    if (numerator_b % denominator != 0) {
        return -1;
    }
    
    long long b = numerator_b / denominator;
    
    // Ensure b is non-negative
    if (b < 0) {
        return -1;
    }
    
    // Calculate a using the first equation
    if ((Px - Xb * b) % Xa != 0) {
        return -1;
    }
    
    long long a = (Px - Xb * b) / Xa;
    
    // Ensure a is non-negative
    if (a < 0) {
        return -1;
    }
    
    // Verify the solution with the second equation
    if (Ya * a + Yb * b != Py) {
        return -1;
    }
    
    // Calculate total tokens
    return 3 * a + b;
}

// Solution for Part 1
void solvePart1(const vector<string>& input) {
    auto machines = parseClawMachines(input);
    long long totalTokens = 0;

    for (const auto& machine : machines) {
        long long tokens = findMinTokensPart2(machine);
        if (tokens != -1) {
            totalTokens += tokens;
        }
    }

    cout << "Part 1 Solution: " << totalTokens << endl;
}

// Solution for Part 2
void solvePart2(const vector<string>& input) {
    const long long OFFSET = 10000000000000;
    auto machines = parseClawMachines(input, OFFSET);
    long long totalTokens = 0;

    for (const auto& machine : machines) {
        long long tokens = findMinTokensPart2(machine);
        if (tokens != -1) {
            totalTokens += tokens;
        }
    }

    cout << "Part 2 Solution: " << totalTokens << endl;
}

int main() {
    // Read input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solve both parts
    cout << "Solving Day 13 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 