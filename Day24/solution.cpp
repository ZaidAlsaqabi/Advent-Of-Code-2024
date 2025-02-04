#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <algorithm>

using namespace std;

// Add this global variable before the readInput function
unordered_map<string, function<int(int, int)>> operation;

// Add this helper function before solvePart1
long long dfs(string s, unordered_map<string, vector<string>>& m, unordered_map<string, int>& val) {
    if(val.count(s))
        return val[s];
    
    int n1 = dfs(m[s][0], m, val);
    int n2 = dfs(m[s][1], m, val);
    return val[s] = operation[m[s][2]](n1, n2);
}

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

// Solution for Part 1
void solvePart1(const vector<string>& input) {
    cout << "Part 1 Solution: " << endl;
    
    unordered_map<string, int> val;
    bool foundEmptyLine = false;
    int i = 0;
    
    // Process initial values
    for(; i < input.size(); i++) {
        const string& line = input[i];
        if(line.empty()) {
            foundEmptyLine = true;
            break;
        }
        val[line.substr(0,3)] = line.back()-'0';
    }
    
    // Skip empty line if found
    if(foundEmptyLine) i++;
    
    // Process operations
    unordered_map<string, vector<string>> m;
    unordered_set<string> gates;
    
    for(; i < input.size(); i++) {
        const string& line = input[i];
        string in1 = line.substr(0,3);
        
        string remainingLine = line.substr(4);
        string op = remainingLine.substr(0, remainingLine.find(' '));
        
        remainingLine = remainingLine.substr(remainingLine.find(' ')+1);
        string in2 = remainingLine.substr(0,3);

        string out = remainingLine.substr(7);
        m[out] = {in1,in2,op};
        if(out[0] == 'z')
            gates.insert(out);
    }

    // Initialize operations
    operation["AND"] = [](int a, int b) { return a && b; };
    operation["OR"] = [](int a, int b) { return a || b; };
    operation["XOR"] = [](int a, int b) { return a != b; }; 

    // Calculate final result
    long long ans = 0;
    for(const string& g : gates) 
        ans |= dfs(g, m, val) << stoll(g.substr(1));

    cout << ans << endl;
}

// Solution for Part 2
void solvePart2(const vector<string>& input) {
    cout << "Part 2 Solution: " << endl;
    
    unordered_map<string, int> val;
    int i = 0;
    
    // Process initial values
    for(; i < input.size(); i++) {
        const string& line = input[i];
        if(line.empty()) break;
        val[line.substr(0,3)] = line.back()-'0';
    }
    
    // Skip empty line
    i++;
    
    // Process operations
    unordered_map<string, vector<string>> m;
    for(; i < input.size(); i++) {
        const string& line = input[i];
        string in1 = line.substr(0,3);
        
        string remainingLine = line.substr(4);
        string op = remainingLine.substr(0, remainingLine.find(' '));
        
        remainingLine = remainingLine.substr(remainingLine.find(' ')+1);
        string in2 = remainingLine.substr(0,3);

        string out = remainingLine.substr(7);
        m[out] = {in1,in2,op};
    }

    // Categorize gates
    vector<string> xyAdds, xyCarries, zOuts, ands, carries, wrongs;
    for(const auto& p : m) {
        string a = p.second[0], b = p.second[1], op = p.second[2];
        if(a[0] == 'x' || a[0] == 'y' || b[0] == 'x' || b[0] == 'y') 
            (op == "AND" ? xyCarries : xyAdds).push_back(p.first);
        else
            (op == "AND" ? ands : op == "XOR" ? zOuts : carries).push_back(p.first);
    }

    // Check XY Adds
    for(const string& s : xyAdds) {
        if(m[s][0] == "x00" || m[s][0] == "y00")
            continue; 

        bool isValid = true, foundInXor = false;
        if(s[0] == 'z')
            isValid = false;
        for(const string& t : zOuts) {
            if(m[t][0] == s || m[t][1] == s) {
                foundInXor = true;
                break;
            }
        }
        if(!foundInXor)
            isValid = false;

        if(!isValid) 
            wrongs.push_back(s);
    }

    // Check XY Carries
    for(const string& s : xyCarries) {
        if(m[s][0] == "x00" || m[s][0] == "y00")
            continue; 

        bool isValid = true, foundInXor = false;
        for(const string& t : carries) {
            if(m[t][0] == s || m[t][1] == s) {
                foundInXor = true;
                break;
            }
        }
        if(!foundInXor)
            isValid = false;

        if(!isValid) 
            wrongs.push_back(s);
    }

    // Check Z outputs
    for(const string& s : zOuts)
        if(s[0] != 'z')
            wrongs.push_back(s); 

    // Check ANDs
    for(const string& s : ands)
        if(s[0] == 'z')
            wrongs.push_back(s); 

    // Check carries
    for(const string& s : carries)
        if(s[0] == 'z' && s != "z45")
            wrongs.push_back(s);

    // Format output
    sort(begin(wrongs), end(wrongs));
    string ans;
    for(const string& s : wrongs)
        ans += s + ',';
    if(!ans.empty())
        ans.pop_back();
        
    cout << ans << endl;
}

int main() {
    // Read input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solve both parts
    cout << "Solving Day 5 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 