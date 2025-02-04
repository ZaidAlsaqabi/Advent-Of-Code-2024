#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
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
        // Ignore empty lines
        if (!line.empty()) {
            input.push_back(line);
        }
    }
    
    file.close();
    return input;
}

// Function to compute the next secret number
long long computeNextSecret(long long currentSecret) {
    // Step 1: Multiply by 64, mix, prune
    long long step1 = (currentSecret * 64);
    currentSecret = currentSecret ^ step1;
    currentSecret = currentSecret % 16777216;

    // Step 2: Divide by 32, round down, mix, prune
    long long step2 = currentSecret / 32;
    currentSecret = currentSecret ^ step2;
    currentSecret = currentSecret % 16777216;

    // Step 3: Multiply by 2048, mix, prune
    long long step3 = currentSecret * 2048;
    currentSecret = currentSecret ^ step3;
    currentSecret = currentSecret % 16777216;

    return currentSecret;
}

// Helper function to encode a 4-change sequence into a unique index
int encodeSequence(int a, int b, int c, int d) {
    // Differences range from -9 to +9
    // Shift by +9 to make them non-negative (0 to 18)
    // Encode as a base-19 number
    return ((a + 9) * 19 * 19 * 19) + ((b + 9) * 19 * 19) + ((c + 9) * 19) + (d + 9);
}

// Solution for Part 1
void solvePart1(const vector<string>& input) {
    long long total = 0;
    const int iterations = 2000;
    
    for (const string& line : input) {
        // Convert the initial secret number from string to long long
        long long secret = stoll(line);

        // Simulate 2000 secret number generations
        for (int i = 0; i < iterations; ++i) {
            secret = computeNextSecret(secret);
        }

        // Add the 2000th secret number to the total
        total += secret;
    }

    cout << "Part 1 Solution: " << total << endl;
}

// Solution for Part 2
void solvePart2(const vector<string>& input) {
    // Initialize a vector to hold the sum of prices for each unique 4-change sequence
    // There are 19 possible differences (-9 to +9) for each change
    // Total unique sequences = 19^4 = 130321
    const int totalSequences = 19 * 19 * 19 * 19; // 130321
    vector<long long> sequenceSum(totalSequences, 0);

    // Iterate over each buyer
    for (const string& line : input) {
        // Convert the initial secret number from string to long long
        long long secret = stoll(line);

        // Store all secrets including the initial one
        vector<long long> secrets;
        secrets.reserve(2001); // Initial secret + 2000 generations
        secrets.push_back(secret);

        // Simulate 2000 secret number generations
        for (int i = 0; i < 2000; ++i) {
            secret = computeNextSecret(secret);
            secrets.push_back(secret);
        }

        // Extract prices: last digit of each secret
        vector<int> prices;
        prices.reserve(2001);
        for (const auto& sec : secrets) {
            prices.push_back(sec % 10);
        }

        // Compute changes: price[n] - price[n-1] for n from 1 to 2000
        vector<int> changes;
        changes.reserve(2000);
        for (int i = 1; i < prices.size(); ++i) {
            changes.push_back(prices[i] - prices[i - 1]);
        }

        // Track which sequences have been already accounted for this buyer
        // To ensure only the first occurrence per sequence is considered
        // Using a boolean array is not feasible due to memory constraints
        // Instead, we'll keep track of sequences encountered in this buyer
        // using an unordered_set with encoded sequence keys
        // However, since sequences are numerically encoded, we can use a vector of flags
        // Reset the flags for each buyer

        // Initialize a vector to track if a sequence has been seen for this buyer
        // To save memory and time, we can iterate through changes and update the sequenceSum immediately
        // without tracking individual sequences per buyer

        // Iterate through all possible 4-change sequences for this buyer
        // and for each sequence, add the corresponding price once
        if (changes.size() < 4) {
            // Not enough changes to form a sequence
            continue;
        }

        // To track the first occurrence of each sequence for this buyer
        // We can use a vector<bool> to mark sequences, but it's too large
        // Alternatively, since we process sequences in order, we can mark sequences as we encounter them

        // Create a local unordered_set to store sequences already accounted for this buyer
        // To optimize, use a bitset if possible; however, unordered_set is acceptable for clarity

        // Using a vector<char> as a bitmap for sequences encountered
        // Each char represents whether a sequence has been counted for this buyer
        // Initialize all to false
        static vector<char> seenSequences(totalSequences, 0);
        fill(seenSequences.begin(), seenSequences.end(), 0);

        for (int i = 0; i <= changes.size() - 4; ++i) {
            int a = changes[i];
            int b = changes[i + 1];
            int c = changes[i + 2];
            int d = changes[i + 3];
            int seqIndex = encodeSequence(a, b, c, d);

            if (!seenSequences[seqIndex]) {
                // Mark as seen
                seenSequences[seqIndex] = 1;

                // The sale occurs at the 4th price in the sequence
                // Which is prices[i + 4]
                // Ensure that i + 4 is within bounds
                if ((i + 4) < prices.size()) {
                    int salePrice = prices[i + 4];
                    sequenceSum[seqIndex] += salePrice;
                }
            }
        }
    }

    // Find the maximum sum of prices across all sequences
    long long maxSum = 0;
    for (const auto& sum : sequenceSum) {
        if (sum > maxSum) {
            maxSum = sum;
        }
    }

    cout << "Part 2 Solution: " << maxSum << endl;
}

int main() {
    // Read input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solve both parts
    cout << "Solving Day 22 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 