#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>

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

// Convert string of digits to vector of integers
vector<int> parseDigits(const string& str) {
    vector<int> digits;
    for (char c : str) {
        digits.push_back(c - '0');
    }
    return digits;
}

// Create initial disk layout from alternating file and space lengths
vector<int> createInitialDisk(const vector<int>& lengths) {
    vector<int> disk;
    int fileId = 0;
    
    for (size_t i = 0; i < lengths.size(); i++) {
        int length = lengths[i];
        if (i % 2 == 0) { // File blocks
            for (int j = 0; j < length; j++) {
                disk.push_back(fileId);
            }
            fileId++;
        } else { // Free space
            for (int j = 0; j < length; j++) {
                disk.push_back(-1); // -1 represents free space
            }
        }
    }
    return disk;
}

// Calculate checksum after compaction
long long calculateChecksum(const vector<int>& disk) {
    long long checksum = 0;
    for (size_t i = 0; i < disk.size(); i++) {
        if (disk[i] != -1) { // Skip free space
            checksum += i * disk[i];
        }
    }
    return checksum;
}

// Find the size of a file starting at the given position
int getFileSize(const vector<int>& disk, int startPos) {
    int fileId = disk[startPos];
    int size = 0;
    for (int i = startPos; i < disk.size() && disk[i] == fileId; i++) {
        size++;
    }
    return size;
}

// Find the leftmost suitable free space for a file of given size
int findLeftmostFreeSpace(const vector<int>& disk, int fileSize) {
    int consecutiveFree = 0;
    for (int i = 0; i < disk.size(); i++) {
        if (disk[i] == -1) {
            consecutiveFree++;
            if (consecutiveFree >= fileSize) {
                return i - fileSize + 1; // Return start position of free space
            }
        } else {
            consecutiveFree = 0;
        }
    }
    return -1; // No suitable free space found
}

// Move a file from source position to target position
void moveFile(vector<int>& disk, int sourcePos, int targetPos, int fileSize) {
    int fileId = disk[sourcePos];
    // Copy file to new position
    for (int i = 0; i < fileSize; i++) {
        disk[targetPos + i] = fileId;
    }
    // Clear old position
    for (int i = 0; i < fileSize; i++) {
        disk[sourcePos + i] = -1;
    }
}

// Solution for Part 1
void solvePart1(const vector<string>& input) {
    string diskMap = input[0];
    vector<int> lengths = parseDigits(diskMap);
    vector<int> disk = createInitialDisk(lengths);
    
    // Compact files from right to left
    for (int i = disk.size() - 1; i >= 0; i--) {
        if (disk[i] != -1) { // Found a file block
            // Look for leftmost free space
            for (int j = 0; j < i; j++) {
                if (disk[j] == -1) { // Found free space
                    // Move file block to free space
                    disk[j] = disk[i];
                    disk[i] = -1;
                    break;
                }
            }
        }
    }
    
    long long checksum = calculateChecksum(disk);
    cout << "Part 1 Solution: " << checksum << endl;
}

// Solution for Part 2
void solvePart2(const vector<string>& input) {
    string diskMap = input[0];
    vector<int> lengths = parseDigits(diskMap);
    vector<int> disk = createInitialDisk(lengths);
    
    // Find highest file ID
    int maxFileId = -1;
    for (int block : disk) {
        maxFileId = max(maxFileId, block);
    }
    
    // Process files in decreasing order of file ID
    for (int fileId = maxFileId; fileId >= 0; fileId--) {
        // Find the first occurrence of this file
        int filePos = -1;
        for (int i = 0; i < disk.size(); i++) {
            if (disk[i] == fileId) {
                filePos = i;
                break;
            }
        }
        
        if (filePos != -1) {
            int fileSize = getFileSize(disk, filePos);
            int targetPos = findLeftmostFreeSpace(disk, fileSize);
            
            // Only move the file if we found suitable free space to the left
            if (targetPos != -1 && targetPos < filePos) {
                moveFile(disk, filePos, targetPos, fileSize);
            }
        }
    }
    
    long long checksum = calculateChecksum(disk);
    cout << "Part 2 Solution: " << checksum << endl;
}

int main() {
    // Read input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solve both parts
    cout << "Solving Day 9 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 