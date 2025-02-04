#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <climits>
#include <algorithm>

using namespace std;

// 2D vector structure
struct Vec2 {
    int x, y;
    Vec2(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
    
    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }
    
    Vec2 operator-() const {
        return Vec2(-x, -y);
    }
    
    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }
};

// State structure (equivalent to C# record)
struct State {
    Vec2 pos;
    string presses;
    State(Vec2 p, string pr) : pos(p), presses(pr) {}
};

// Constants
const Vec2 DOWN(1, 0);
const Vec2 RIGHT(0, 1);
const Vec2 OUT1(0, 0);
const Vec2 OUT2(3, 0);

// Helper function to replace all occurrences of a character in a string
string replaceAll(string str, char from, const string& to) {
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != string::npos) {
        str.replace(pos, 1, to);
        pos += to.length();
    }
    return str;
}

class Solution {
private:
    map<string, long long> memo;
    
    long long scoreGoal(const string& target, int depth, int maxDepth) {
        if (depth == -1) return target.length();
        
        string padText;
        Vec2 cur;
        
        if (depth == maxDepth) {
            padText = "789456123X0A";
            cur = Vec2(3, 2);
        } else {
            padText = "X^A<v>";
            cur = Vec2(0, 2);
        }
        
        long long result = 0;
        for (char ch : target) {
            size_t k = padText.find(ch);
            Vec2 nxt(k / 3, k % 3); // 3=padwidth
            result += minimal(cur, nxt, depth, maxDepth);
            cur = nxt;
        }
        return result;
    }
    
    long long minimal(Vec2 cur, Vec2 dst, int depth, int maxDepth) {
        string key = to_string(cur.x) + "," + to_string(cur.y) + ":" +
                     to_string(dst.x) + "," + to_string(dst.y) + ":" +
                     to_string(depth);
                     
        if (memo.find(key) != memo.end())
            return memo[key];
            
        long long answer = LLONG_MAX;
        queue<State> q;
        q.push(State(cur, ""));
        
        while (!q.empty()) {
            State state = q.front();
            q.pop();
            
            if (state.pos == dst) {
                answer = min(answer, scoreGoal(state.presses + "A", depth - 1, maxDepth));
                continue;
            }
            
            // Out of bounds check
            if ((depth == maxDepth && state.pos == OUT2) || 
                (depth != maxDepth && state.pos == OUT1))
                continue;
                
            // Move closer
            if (state.pos.x < dst.x) 
                q.push(State(state.pos + DOWN, state.presses + "v"));
            else if (state.pos.x > dst.x) 
                q.push(State(state.pos + (-DOWN), state.presses + "^"));
                
            if (state.pos.y < dst.y)
                q.push(State(state.pos + RIGHT, state.presses + ">"));
            else if (state.pos.y > dst.y)
                q.push(State(state.pos + (-RIGHT), state.presses + "<"));
        }
        
        memo[key] = answer;
        return answer;
    }

public:
    long long solve(const vector<string>& input, bool part2) {
        int maxDepth = part2 ? 25 : 2;
        long long total = 0;
        
        for (const string& code : input) {
            string processedCode = replaceAll(code, 'A', "");
            total += scoreGoal(code, maxDepth, maxDepth) * stoll(processedCode);
        }
        
        return total;
    }
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

// Solution for Part 1
void solvePart1(const vector<string>& input) {
    Solution solution;
    cout << "Part 1 Solution: " << solution.solve(input, false) << endl;
}

// Solution for Part 2
void solvePart2(const vector<string>& input) {
    Solution solution;
    cout << "Part 2 Solution: " << solution.solve(input, true) << endl;
}

int main() {
    // Read input
    vector<string> input = readInput();
    
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    // Solve both parts
    cout << "Solving Day 21 Puzzle..." << endl;
    cout << "----------------------" << endl;
    
    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);
    
    return 0;
} 