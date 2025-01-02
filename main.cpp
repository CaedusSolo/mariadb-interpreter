//  ************************************************************************************************
//  Program: main.cpp
//  Course: CCP6114 Programming Fundamentals
//  Lecture Class: TC1L
//  Tutorial Class: TT1L
//  Trimester: 2430
//  Member_1: 242UC244NK | CHOW YING TONG | chow.ying.tong@student.mmu.edu.my | 016-5767692
//  Member_2: 242UC244NK | LAW CHIN XUAN | law.chin.xuan@student.mmu.edu.my | 011-10988658
//  ************************************************************************************************
//  Task Distribution
//  Member_1:
//  Member_2: 
//  ************************************************************************************************

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

ifstream inputFile;
ofstream outputFile;

void readFileInput();
void createTable();

vector<string> tokenize(const string &line) {
    vector<string> tokens;
    stringstream stream(line);
    string token;

    while (stream >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

int main() {
    readFileInput();
    return 0;
}

void readFileInput() {
    inputFile.open("fileInput1.mdb");

    string line;
    while (getline(inputFile, line)) {
        cout << "Currently reading: " << line << endl;
        
        vector<string> tokens = tokenize(line);

        if (!tokens.empty() && tokens[0] == "CREATE") {
            if (tokens[1] == "TABLE") {
                cout << "This is a table creation operation." << endl;
            }
            else if (tokens[1].length() >= 4 && tokens[1].substr(tokens[1].length() - 5, 5) == ".txt;") {
                cout << "Creating output file..." << endl;
            }
        }
    }

    inputFile.close();
}

void createTable() {

}