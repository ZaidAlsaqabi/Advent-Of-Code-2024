#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

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

// Function to evaluate expression with given operators
long long concatenateNumbers(long long a, long long b) {
    // Convert b to string to get its length
    string b_str = to_string(b);
    // Concatenate by multiplying a by 10^(length of b) and adding b
    return a * (long long)pow(10, b_str.length()) + b;
}

// Function to evaluate expression with given operators
long long evaluateExpression(const vector<int>& numbers, const vector<string>& operators) {
    long long result = numbers[0];
    for (size_t i = 0; i < operators.size(); i++) {
        if (operators[i] == "+") {
            result += numbers[i + 1];
        } else if (operators[i] == "*") {
            result *= numbers[i + 1];
        } else if (operators[i] == "||") {
            result = concatenateNumbers(result, numbers[i + 1]);
        }
    }
    return result;
}

// Parse a single equation line
bool parseEquation(const string& line, long long& testValue, vector<int>& numbers) {
    istringstream iss(line);
    string part;
    
    // Read test value
    getline(iss, part, ':');
    testValue = stoll(part);
    
    // Read numbers
    int num;
    while (iss >> num) {
        numbers.push_back(num);
    }
    
    return true;
}

// Results for Part 1
void solvePart1(const vector<string>& input) {
    long long totalSum = 0;
    
    for (const string& line : input) {
        long long testValue;
        vector<int> numbers;
        
        if (!parseEquation(line, testValue, numbers)) {
            continue;
        }
        
        // Generate all possible operator combinations
        int numOperators = numbers.size() - 1;
        int numCombinations = 1 << numOperators; // 2^numOperators
        
        bool canBeSolved = false;
        for (int i = 0; i < numCombinations; i++) {
            vector<string> operators;
            for (int j = 0; j < numOperators; j++) {
                // Use bit j of i to determine operator
                operators.push_back((i & (1 << j)) ? "*" : "+");
            }
            
            if (evaluateExpression(numbers, operators) == testValue) {
                canBeSolved = true;
                break;
            }
        }
        
        if (canBeSolved) {
            totalSum += testValue;
        }
    }
    
    cout << "Part 1 Results: " << totalSum << endl;
}

// Results for Part 2
void solvePart2(const vector<string>& input) {
    long long totalSum = 0;
    
    for (const string& line : input) {
        long long testValue;
        vector<int> numbers;
        
        if (!parseEquation(line, testValue, numbers)) {
            continue;
        }
        
        // Generate all possible operator combinations
        int numOperators = numbers.size() - 1;
        vector<string> possibleOps = {"+", "*", "||"};
        int numCombinations = pow(3, numOperators); // 3^numOperators for three possible operators
        
        bool canBeSolved = false;
        for (int i = 0; i < numCombinations; i++) {
            vector<string> operators;
            int temp = i;
            
            // Convert number to base-3 to generate operator combinations
            for (int j = 0; j < numOperators; j++) {
                operators.push_back(possibleOps[temp % 3]);
                temp /= 3;
            }
            
            if (evaluateExpression(numbers, operators) == testValue) {
                canBeSolved = true;
                break;
            }
        }
        
        if (canBeSolved) {
            totalSum += testValue;
        }
    }
    
    cout << "Part 2 Results: " << totalSum << endl;
}

int main() {
    // Read input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solve both parts
    cout << "Solving Day 7 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 