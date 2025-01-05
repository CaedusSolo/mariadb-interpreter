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

vector<string> tokenize(const string&);
void readFileInput();
void createTable(const vector<string> &);

vector<string> tokenize(const string &line) {
    vector<string> tokens;
    stringstream stream(line);
    string token;

    while (getline(stream, token, ' ')) {
        if (!token.empty()) {
            size_t pos = token.find('(');
            if (pos != string::npos) {
                if (pos > 0) {
                    tokens.push_back(token.substr(0, pos));
                }
                tokens.push_back("(");
                if (pos + 1 < token.length()) {
                    tokens.push_back(token.substr(pos + 1));
                }
            } else {
                pos = token.find(')');
                if (pos != string::npos) {
                    if (pos > 0) {
                        tokens.push_back(token.substr(0, pos));
                    }
                    tokens.push_back(")");
                    if (pos + 1 < token.length()) {
                        tokens.push_back(token.substr(pos + 1));
                    }
                } else {
                    tokens.push_back(token);
                }
            }
        }
    }
    return tokens;
}


int main() {
    readFileInput();
    return 0;
}

void readFileInput() {
    inputFile.open("fileInput1.mdb"); //  open input file

    string line;  //  initialize variable to store content of each line

    while (getline(inputFile, line)) {
        if (line.empty()) continue;  //  skip tokenization for empty lines
        vector<string> tokens = tokenize(line);

        for (int i = 0; i < tokens.size(); i++) {
            cout << tokens[i] << endl;
        }

        if (!tokens.empty() && tokens[0] == "CREATE") {
            if (tokens[1] == "TABLE") {
                cout << "This is a table creation operation." << endl;
                createTable(tokens);
            }
            else if (tokens[1].length() >= 4 && tokens[1].substr(tokens[1].length() - 5, 5) == ".txt;") {
                cout << "Creating output file..." << endl;
            }
        }

        else if (!tokens.empty() && tokens[0] == "INSERT" && tokens[1] == "INTO") {
            createTable(tokens);
        }

    }

    inputFile.close();
}

void createTable(const vector<string> &tokens) {
    string tableName = tokens[2];
}