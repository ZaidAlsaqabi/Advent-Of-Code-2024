#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Dir {
    int dr, dc;
};

static const Dir UP = {-1, 0};
static const Dir DOWN = {1, 0};
static const Dir LEFT = {0, -1};
static const Dir RIGHT = {0, 1};

Dir directionFromChar(char c) {
    switch (c) {
        case '^': return UP;
        case 'v': return DOWN;
        case '<': return LEFT;
        case '>': return RIGHT;
    }
    return {0,0};
}

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

void splitInput(const vector<string>& input, vector<string>& warehouse, vector<string>& moveLines) {
    int splitIndex = 0;
    for (int i = 0; i < (int)input.size(); i++) {
        bool isMapLine = (input[i].find('#') != string::npos);
        if (!isMapLine) {
            splitIndex = i;
            break;
        }
    }

    if (splitIndex == 0) {
        cerr << "Could not find moves in input!" << endl;
        return;
    }

    warehouse.assign(input.begin(), input.begin() + splitIndex);
    moveLines.assign(input.begin() + splitIndex, input.end());
}

string extractMoves(const vector<string>& moveLines) {
    string moves;
    for (auto &ml : moveLines) {
        for (auto c : ml) {
            if (c == '^' || c == 'v' || c == '<' || c == '>') {
                moves.push_back(c);
            }
        }
    }
    return moves;
}

void solvePart1(const vector<string>& warehouse, const string& moves) {
    int mapHeight = (int)warehouse.size();
    int mapWidth = (int)warehouse[0].size();

    vector<vector<char>> grid(mapHeight, vector<char>(mapWidth));
    int robotR = -1, robotC = -1;
    for (int r = 0; r < mapHeight; r++) {
        for (int c = 0; c < mapWidth; c++) {
            grid[r][c] = warehouse[r][c];
            if (grid[r][c] == '@') {
                robotR = r;
                robotC = c;
            }
        }
    }

    auto canMove = [&](int r, int c) {
        if (r < 0 || r >= mapHeight || c < 0 || c >= mapWidth) return false;
        return grid[r][c] != '#';
    };

    for (auto move : moves) {
        Dir d = directionFromChar(move);
        int newR = robotR + d.dr;
        int newC = robotC + d.dc;
        if (!canMove(newR, newC)) continue;

        if (grid[newR][newC] == '.') {
            grid[robotR][robotC] = '.';
            grid[newR][newC] = '@';
            robotR = newR; robotC = newC;
        } else if (grid[newR][newC] == 'O') {
            vector<pair<int,int>> boxes;
            int currR = newR, currC = newC;
            while (true) {
                if (grid[currR][currC] == 'O') {
                    boxes.push_back({currR, currC});
                    currR += d.dr; currC += d.dc;
                    if (!canMove(currR, currC)) {
                        boxes.clear();
                        break;
                    }
                } else {
                    break;
                }
            }
            if (boxes.empty()) continue;

            for (int i = (int)boxes.size() - 1; i >= 0; i--) {
                int br = boxes[i].first;
                int bc = boxes[i].second;
                int nr = br + d.dr;
                int nc = bc + d.dc;
                grid[nr][nc] = 'O';
                grid[br][bc] = '.';
            }

            grid[robotR][robotC] = '.';
            grid[newR][newC] = '@';
            robotR = newR; robotC = newC;
        }
    }

    long long sumGPS = 0;
    for (int r = 0; r < mapHeight; r++) {
        for (int c = 0; c < mapWidth; c++) {
            if (grid[r][c] == 'O') {
                sumGPS += (r * 100 + c);
            }
        }
    }

    cout << "Part 1 Solution: " << sumGPS << endl;
}

struct Pos {
    int x;
    int y;
};

static vector<vector<char>> jsMap;
static vector<char> jsMoves;
static Pos jsCurrent;

Pos findRobotJS(const vector<vector<char>>& m) {
    for (int y = 0; y < (int)m.size(); y++) {
        for (int x = 0; x < (int)m[y].size(); x++) {
            if (m[y][x] == '@') {
                return {x,y};
            }
        }
    }
    return {-1,-1};
}

void parseJS(const string& input) {
    size_t sep = input.find("\n\n");
    if (sep == string::npos) {
        cerr << "Invalid input format for JS parse!" << endl;
        return;
    }
    string mapPart = input.substr(0, sep);
    string movesPart = input.substr(sep+2);

    vector<string> mapLines;
    {
        size_t start = 0;
        while (true) {
            size_t end = mapPart.find('\n', start);
            string line = (end == string::npos) ? mapPart.substr(start) : mapPart.substr(start, end - start);
            if (!line.empty()) mapLines.push_back(line);
            if (end == string::npos) break;
            start = end + 1;
        }
    }

    jsMap.clear();
    for (auto &line : mapLines) {
        vector<char> row(line.begin(), line.end());
        jsMap.push_back(row);
    }

    jsMoves.clear();
    for (auto c : movesPart) {
        if (c == '^' || c == 'v' || c == '<' || c == '>') {
            jsMoves.push_back(c);
        }
    }

    jsCurrent = findRobotJS(jsMap);
}

long long checksumJS(const vector<vector<char>>& m) {
    long long sum = 0;
    for (int y = 0; y < (int)m.size(); y++) {
        for (int x = 0; x < (int)m[y].size(); x++) {
            if (m[y][x] == 'O' || m[y][x] == '[') {
                sum += (y * 100 + x);
            }
        }
    }
    return sum;
}

Pos moveJS(vector<vector<char>>& map, Pos current, char dir);

Pos doMoveJS(vector<vector<char>>& map, Pos current, char dir) {
    Pos dest = current;
    if (dir == 'v') dest.y++;
    if (dir == '^') dest.y--;
    if (dir == '>') dest.x++;
    if (dir == '<') dest.x--;

    if (dest.y < 0 || dest.y >= (int)map.size() || dest.x < 0 || dest.x >= (int)map[0].size()) {
        return current;
    }

    char cell = map[dest.y][dest.x];
    if (cell == 'O') {
        moveJS(map, dest, dir);
    }
    if (cell == '[' || cell == ']') {
        if (dest.x != current.x) {
            moveJS(map, dest, dir);
        } else {
            vector<vector<char>> copy = map;
            Pos pair = dest;
            if (cell == '[') pair.x += 1; else pair.x -= 1;
            Pos res1 = moveJS(copy, dest, dir);
            Pos res2 = moveJS(copy, pair, dir);
            if (!(res1.x == dest.x && res1.y == dest.y) && !(res2.x == pair.x && res2.y == pair.y)) {
                moveJS(map, dest, dir);
                moveJS(map, pair, dir);
            }
        }
    }
    cell = map[dest.y][dest.x];
    if (cell == '.') {
        map[dest.y][dest.x] = map[current.y][current.x];
        map[current.y][current.x] = '.';
        return dest;
    }

    return current;
}

Pos moveJS(vector<vector<char>>& map, Pos current, char dir) {
    return doMoveJS(map, current, dir);
}

long long part1JS(const string& input) {
    parseJS(input);
    Pos c = jsCurrent;
    for (auto m : jsMoves) {
        c = moveJS(jsMap, c, m);
    }
    return checksumJS(jsMap);
}

long long part2JS(const string& input) {
    string transformed = input;
    {
        string result;
        for (auto ch : transformed) {
            if (ch == '#') result += "##"; else result.push_back(ch);
        }
        transformed = result;
    }
    {
        string result;
        for (int i = 0; i < (int)transformed.size(); i++) {
            if (transformed[i] == 'O') {
                result += "[]";
            } else {
                result.push_back(transformed[i]);
            }
        }
        transformed = result;
    }
    {
        string result;
        for (auto ch : transformed) {
            if (ch == '.') result += ".."; else result.push_back(ch);
        }
        transformed = result;
    }
    {
        string result;
        for (int i = 0; i < (int)transformed.size(); i++) {
            if (transformed[i] == '@') {
                result += "@.";
            } else {
                result.push_back(transformed[i]);
            }
        }
        transformed = result;
    }

    return part1JS(transformed);
}

int main() {
    vector<string> input = readInput();
    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    vector<string> warehouse, moveLines;
    splitInput(input, warehouse, moveLines);
    if (warehouse.empty()) {
        cerr << "No warehouse data found in input." << endl;
        return 1;
    }

    string moves = extractMoves(moveLines);

    cout << "Solving Day 15 Puzzle..." << endl;
    cout << "----------------------" << endl;
    solvePart1(warehouse, moves);
    cout << "----------------------" << endl;

    auto joinLines = [&](const vector<string>& lines) {
        string out;
        for (int i = 0; i < (int)lines.size(); i++) {
            if (i > 0) out += "\n";
            out += lines[i];
        }
        return out;
    };

    string warehouseStr = joinLines(warehouse);
    string movesStr = moves;
    string combinedInput = warehouseStr + "\n\n" + movesStr;

    long long part2Answer = part2JS(combinedInput);
    cout << "Part 2 Solution: " << part2Answer << endl;

    return 0;
}
