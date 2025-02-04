#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

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

// Results for Part 1
void solvePart1(const vector<string>& input) {
    vector<int> leftList, rightList;
    
    // Parse input into two lists
    for (const string& line : input) {
        size_t spacePos = line.find(' ');
        if (spacePos != string::npos) {
            int leftNum = stoi(line.substr(0, spacePos));
            int rightNum = stoi(line.substr(spacePos + 1));
            leftList.push_back(leftNum);
            rightList.push_back(rightNum);
        }
    }
    
    // Sort both lists
    sort(leftList.begin(), leftList.end());
    sort(rightList.begin(), rightList.end());
    
    // Calculates total distance
    int totalDistance = 0;
    for (size_t i = 0; i < leftList.size(); i++) {
        totalDistance += abs(leftList[i] - rightList[i]);
    }
    
    cout << "Part 1 Results: " << totalDistance << endl;
}

// Results for Part 2
void solvePart2(const vector<string>& input) {
    vector<int> leftList, rightList;
    
    // Parse input into two lists (similar to Part 1)
    for (const string& line : input) {
        size_t spacePos = line.find(' ');
        if (spacePos != string::npos) {
            int leftNum = stoi(line.substr(0, spacePos));
            int rightNum = stoi(line.substr(spacePos + 1));
            leftList.push_back(leftNum);
            rightList.push_back(rightNum);
        }
    }

    // Calculate similarity score
    int similarityScore = 0;
    for (int leftNum : leftList) {
        int frequency = count(rightList.begin(), rightList.end(), leftNum);
        similarityScore += leftNum * frequency;
    }
    
    cout << "Part 2 Results: " << similarityScore << endl;
}

int main() {
    // Read input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solve both parts
    cout << "Solving Day 1 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 