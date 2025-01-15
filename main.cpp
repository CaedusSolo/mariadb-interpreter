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
#include <cctype>
using namespace std;

ifstream inputFile;
ofstream outputFile;

struct Column {
    string columnName;
    string columnType;
};

struct Table {
    string tableName;
    vector<Column> tableColumns;
    vector<vector<string>> tableRows;
};


vector<string> tokenize(const string&);
void readFileInput();
void createOutputFile(string&);
void createTable(vector<string> &);

Table table;

vector<string> tokenize(const string& input) {
    vector<string> tokens;
    stringstream ss(input);
    string token;
    char c;

    while (ss.get(c)) {
        if (isspace(c)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else if (c == '(' || c == ')' || c == ',' || c == ';' || c == '*') { // Handle delimiters
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(string(1, c)); // Add the delimiter as a token
        } else if (c == '\'') { // Handle string literals
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            string str_lit;
            while (ss.get(c)) {
                if (c == '\'') {
                    tokens.push_back("'" + str_lit + "'");
                    str_lit.clear();
                    break;
                } else {
                    str_lit += c;
                }
            }
        } else {
            token += c;
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    for (size_t i = 0; i < tokens.size(); i++) {
        cout << "Token: " << tokens[i] << endl;
    }
//  OUTPUT for the above for loop:
// Token: CREATE
// Token: fileOutput1.txt
// Token: ;
// Token: DATABASES
// Token: ;
// Token: CREATE
// Token: TABLE
// Token: customer
// Token: (
// Token: customer_id
// Token: INT
// Token: ,
// Token: customer_name
// Token: TEXT
// Token: ,
// Token: customer_city
// Token: TEXT
// Token: ,
// Token: customer_state
// Token: TEXT
// Token: ,
// Token: customer_country
// Token: TEXT
// Token: ,
// Token: customer_phone
// Token: TEXT
// Token: ,
// Token: customer_email
// Token: TEXT
// Token: )
// Token: ;
// Token: TABLES
// Token: ;
// Token: INSERT
// Token: INTO
// Token: customer
// Token: (
// Token: customer_id
// Token: ,
// Token: customer_name
// Token: ,
// Token: customer_city
// Token: ,
// Token: customer_state
// Token: ,
// Token: customer_country
// Token: ,
// Token: customer_phone
// Token: ,
// Token: customer_email
// Token: )
// Token: VALUES
// Token: (
// Token: 1
// Token: ,
// Token: 'name1'
// Token: ,
// Token: 'city1'
// Token: ,
// Token: 'state1'
// Token: ,
// Token: 'country1'
// Token: ,
// Token: 'phone1'
// Token: ,
// Token: 'email1'
// Token: )
// Token: ;
    return tokens;
}


int main() {
    readFileInput();
    return 0;
}

void readFileInput() {
    inputFile.open("fileInput1.mdb");
    if (!inputFile.is_open()) {
        cerr << "Error opening input file." << endl;
        return;
    }

    stringstream buffer;
    buffer << inputFile.rdbuf(); // Read entire file into buffer
    string fileContent = buffer.str();
    inputFile.close();

    vector<string> allTokens = tokenize(fileContent);

    // Now you have all tokens, process them
    for (size_t i = 0; i < allTokens.size(); ++i) {
        if (allTokens[i] == "CREATE") {
            if (allTokens[i + 1] == "TABLE") {
                vector<string> createTableTokens;
                size_t j = i;
                while(j < allTokens.size() && allTokens[j] != ";"){
                    createTableTokens.push_back(allTokens[j]);
                    j++;
                }
                createTable(createTableTokens);
                i = j;
            } else if (allTokens[i + 1].length() >= 4 && allTokens[i + 1].substr(allTokens[i + 1].length() - 5, 5) == ".txt;") {
                createOutputFile(allTokens[i + 1]);
            }
        } else if (allTokens[i] == "INSERT" && allTokens[i + 1] == "INTO") {
            cout << "Table insertion operation..." << endl;
        }
    }
}

void createOutputFile(string &fileName) {
    if (fileName.back() == ';') {
        fileName.pop_back();
    }
    
    outputFile.open(fileName);
    if (outputFile.is_open()) {
        cout << "Output file: " << fileName << " created successfully." << endl;
    }
    else {
        cerr << "Failed to create output file." << endl;
    }
}


void createTable(vector<string> &tokens) {
    string tableName = tokens[2];
    table.tableColumns.clear();

    table.tableName = tableName;  //  set tableName in Table struct
    
    for (size_t i = 4; i < tokens.size(); i+=3) {
        string columnName = tokens[i];
        string columnType = tokens[i+1];
        if (columnType.back() == ',') {
            columnType.pop_back();
        }

        Column column = {columnName, columnType};
        table.tableColumns.push_back(column);
    }

    cout << "Table '" << table.tableName << "' created with the following columns:" << endl;
    for (const auto &col : table.tableColumns) {
        cout << "- " << col.columnName << " (" << col.columnType << ")" << endl;
    }

}