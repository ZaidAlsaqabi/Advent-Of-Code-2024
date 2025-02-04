#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

struct Robot {
    long long px, py;
    long long vx, vy;
};

long long positiveMod(long long a, long long b) {
    return ((a % b) + b) % b;
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

const long long WIDTH = 101;
const long long HEIGHT = 103;

bool isChristmasTree(const vector<Robot>& robots) {
    vector<pair<long long, long long>> treePattern = {
        {0, 0}, {1, 0}, {-1, 0}, {0, 1}, {0, 2}
    };

    if (robots.empty()) return false;
    long long ref_x = robots[0].px;
    long long ref_y = robots[0].py;

    for (const auto& offset : treePattern) {
        bool match = false;
        for (const auto& robot : robots) {
            if (robot.px == positiveMod(ref_x + offset.first, WIDTH) &&
                robot.py == positiveMod(ref_y + offset.second, HEIGHT)) {
                match = true;
                break;
            }
        }
        if (!match) return false;
    }

    return true;
}

void solvePart1(const vector<string>& input) {
    vector<Robot> robots;
    for (const auto& line : input) {
        Robot robot;
        size_t p_start = line.find('p');
        size_t v_start = line.find('v');
        string p_part = line.substr(p_start + 2, line.find(' ', p_start) - (p_start + 2));
        string v_part = line.substr(v_start + 2);

        size_t comma = p_part.find(',');
        robot.px = stoll(p_part.substr(0, comma));
        robot.py = stoll(p_part.substr(comma + 1));

        comma = v_part.find(',');
        robot.vx = stoll(v_part.substr(0, comma));
        robot.vy = stoll(v_part.substr(comma + 1));

        robots.push_back(robot);
    }

    const long long TIME = 100;

    long long q1 = 0, q2 = 0, q3 = 0, q4 = 0;

    for (const auto& robot : robots) {
        long long new_x = positiveMod(robot.px + robot.vx * TIME, WIDTH);
        long long new_y = positiveMod(robot.py + robot.vy * TIME, HEIGHT);

        if (new_x > WIDTH / 2 && new_y < HEIGHT / 2) {
            q1++;
        }
        else if (new_x < WIDTH / 2 && new_y < HEIGHT / 2) {
            q2++;
        }
        else if (new_x < WIDTH / 2 && new_y > HEIGHT / 2) {
            q3++;
        }
        else if (new_x > WIDTH / 2 && new_y > HEIGHT / 2) {
            q4++;
        }
    }

    long long safety_factor = q1 * q2 * q3 * q4;

    cout << "Part 1 Solution: " << safety_factor << endl;
}

void solvePart2(const vector<string>& input) {
    vector<Robot> robots;
    for (const auto& line : input) {
        Robot robot;
        size_t p_start = line.find('p');
        size_t v_start = line.find('v');
        string p_part = line.substr(p_start + 2, line.find(' ', p_start) - (p_start + 2));
        string v_part = line.substr(v_start + 2);

        size_t comma = p_part.find(',');
        robot.px = stoll(p_part.substr(0, comma));
        robot.py = stoll(p_part.substr(comma + 1));

        comma = v_part.find(',');
        robot.vx = stoll(v_part.substr(0, comma));
        robot.vy = stoll(v_part.substr(comma + 1));

        robots.push_back(robot);
    }

    long long time = 0;
    const long long MAX_TIME = 1000000;

    while (time < MAX_TIME) {
        time++;

        for (auto& robot : robots) {
            robot.px = positiveMod(robot.px + robot.vx, WIDTH);
            robot.py = positiveMod(robot.py + robot.vy, HEIGHT);
        }

            if (isChristmasTree(robots)) {
                cout << "Part 2 Solution: " << time << endl;
                return;
            }
        }

    cout << "Part 2 Solution: Christmas tree pattern not found within " << MAX_TIME << " seconds." << endl;
}

int main() {
    vector<string> input = readInput();

    if (input.empty()) {
        cerr << "No input data found!" << endl;
        return 1;
    }

    cout << "Solving Day 14 Puzzle..." << endl;
    cout << "----------------------" << endl;

    solvePart1(input);
    cout << "----------------------" << endl;
    solvePart2(input);

    return 0;
} 