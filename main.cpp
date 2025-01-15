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
        while (!token.empty()) {
            size_t openPos = token.find('(');
            size_t closePos = token.find(')');

            if (openPos != string::npos) {
                if (openPos > 0) {
                    tokens.push_back(token.substr(0, openPos)); // Before '('
                }
                tokens.push_back("("); // Add '(' as its own token
                token = token.substr(openPos + 1); // Remaining part after '('

            } else if (closePos != string::npos) {
                if (closePos > 0) {
                    tokens.push_back(token.substr(0, closePos)); // Before ')'
                }
                tokens.push_back(")"); // Add ')' as its own token
                token = token.substr(closePos + 1); // Remaining part after ')'
                
            } else {
                tokens.push_back(token); // Add the full token if no '(' or ')'
                token.clear(); // Break the loop
            }
        }
    }

    // Debugging: Print tokens
    for (size_t i = 0; i < tokens.size(); ++i) {
        cout << "Token #" << i + 1 << ": " << tokens[i] << endl;
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

        if (!tokens.empty() && tokens[0] == "CREATE") {
            if (tokens[1] == "TABLE") {
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