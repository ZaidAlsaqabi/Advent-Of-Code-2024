#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

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

// Helper function to evaluate a multiplication instruction
int evaluateMul(const string& x, const string& y) {
    return stoi(x) * stoi(y);
}

// Results for Part 1
void solvePart1(const vector<string>& input) {
    int total = 0;
    regex mulPattern(R"(mul\((\d{1,3}),(\d{1,3})\))");
    
    for (const string& line : input) {
        smatch matches;
        string::const_iterator searchStart(line.cbegin());
        
        while (regex_search(searchStart, line.cend(), matches, mulPattern)) {
            string x = matches[1];
            string y = matches[2];
            total += evaluateMul(x, y);
            searchStart = matches.suffix().first;
        }
    }
    
    cout << "Part 1 Results: " << total << endl;
}

// Results for Part 2
void solvePart2(const vector<string>& input) {
    int total = 0;
    bool mulEnabled = true; 
    regex instructionPattern(R"((do\(\)|don't\(\)|mul\((\d{1,3}),(\d{1,3})\)))");
    
    for (const string& line : input) {
        smatch matches;
        string::const_iterator searchStart(line.cbegin());
        
        while (regex_search(searchStart, line.cend(), matches, instructionPattern)) {
            if (matches[0] == "do()") {
                mulEnabled = true; // Enables multiplication
            } else if (matches[0] == "don't()") {
                mulEnabled = false; // Disables multiplication
            } else if (mulEnabled) {
                string x = matches[2];
                string y = matches[3];
                total += evaluateMul(x, y);
            }
            searchStart = matches.suffix().first;
        }
    }
    
    cout << "Part 2 Results: " << total << endl;
}

int main() {
    // Reads input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input file found!" << endl;
        return 1;
    }

    // Solves both parts
    cout << "Solving Day 3 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 