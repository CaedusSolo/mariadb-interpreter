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
//  Member_1: Read input file, tokenization, create table, insert into table, write to output file
//  Member_2: Delete from table, update table, count table rows, flowchart
//  ************************************************************************************************
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <typeinfo>
#include <filesystem>
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

vector<string> inputContent;
vector<string> outputContent;

//  function prototypes
vector<string> tokenize(const string&);
void readFileInput();
void createOutputFile(string&);
void createTable(vector<string> &);
void insertIntoTable(vector<string> &);
void updateTable(vector<string>&);
void deleteFromTable(vector<string>&);
void countRows();
void selectFromTable();
bool validateRowData(const vector<string>&);
bool isInteger(const string&);
string removeQuotesFromStringLit(string &);
void writeToOutputFile();


Table table;
//  This is what the Table struct looks like:
//  {
//     "customer", // Table name
//     {
//         {"customer_id", "INT"},
//         {"customer_name", "TEXT"},
//         {"customer_city", "TEXT"},
//         {"customer_state", "TEXT"},
//     }, // Columns

//     {
//         {"1", "'Alice'", "'NYC'", "'NY'"},
//         {"2", "'Bob'", "'LA'", "'CA'"},
//     } // Rows
// };

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
        } else if (c == '(' || c == ')' || c == ',' || c == ';' || c == '*') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(string(1, c));
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
    return tokens;
}

int main() {
    readFileInput();
    writeToOutputFile();
    return 0;
}

void readFileInput() {
    inputFile.open("fileInput2.mdb");
    if (!inputFile.is_open()) {
        cerr << "Error opening input file." << endl;
        return;
    }

    stringstream buffer;
    buffer << inputFile.rdbuf();
    string fileContent = buffer.str();
    inputFile.close();

    vector<string> allTokens = tokenize(fileContent);

    for (size_t i = 0; i < allTokens.size(); ++i) {
        if (allTokens[i] == "CREATE") {
            if (allTokens[i + 1] == "TABLE") {
                vector<string> createTableTokens;
                size_t j = i;
                while (j < allTokens.size() && allTokens[j] != ";") {
                    createTableTokens.push_back(allTokens[j]);
                    j++;
                }
                createTable(createTableTokens);
                i = j;
            } else if (allTokens[i + 1].find(".txt") != string::npos) {
                createOutputFile(allTokens[i + 1]);
            }

        } else if (allTokens[i] == "INSERT" && allTokens[i + 1] == "INTO") {
            vector<string> insertTokens;
            size_t j = i;
            while (j < allTokens.size() && allTokens[j] != ";") {
                insertTokens.push_back(allTokens[j]);
                j++;
            }
            insertIntoTable(insertTokens);
            i = j;

        } else if (allTokens[i] == "DELETE") {
            vector<string> deleteTokens;
            size_t j = i;
            while (j < allTokens.size() && allTokens[j] != ";") {
                deleteTokens.push_back(allTokens[j]);
                j++;
            }
            deleteFromTable(deleteTokens);
            i = j;
            
        } 
         else if (allTokens[i] == "UPDATE") {
            vector<string> updateTokens;
            size_t j = i;

            while (j < allTokens.size() && allTokens[j] != ";" && allTokens[i+2] == "FROM") {
                updateTokens.push_back(allTokens[j]);
                j++;
            }
            updateTable(updateTokens);
            i = j;
        }

        else if (allTokens[i] == "SELECT") {
            if (allTokens[i + 1] == "*" && allTokens[i+2] == "FROM") {
                if (allTokens[i+3] == table.tableName) {
                    selectFromTable();
                }
                else {
                    outputContent.push_back("Table does not exist.");
                }
            }

            else if (allTokens[i+1] == "COUNT") {
                if (allTokens[i+2] == "(" && allTokens[i+3] == "*" && allTokens[i+4] == ")") {
                    countRows();
                }
            }
        }

        else if (allTokens[i] == "DATABASES") {
            auto filePath = filesystem::absolute("fileInput2.mdb");    
            outputContent.push_back(filePath.string());
        }

        else if (allTokens[i] == "TABLES") {
            outputContent.push_back(table.tableName);
        }
    }
}

void createOutputFile(string &fileName) {
    ostringstream oss;
    
    if (fileName.back() == ';') {
        fileName.pop_back();
    }
    
    outputFile.open(fileName);
    if (outputFile.is_open()) {
        oss << "Output file " << fileName << " created successfully!";
        outputContent.push_back(oss.str());
    }
    else {
        outputContent.push_back("Failed to created output file.");
        return;
    }
}

void createTable(vector<string> &tokens) { 
    string tableName = tokens[2];
    ostringstream oss;

    table.tableName = tableName;  //  set tableName in Table struct
    
    for (size_t i = 4; i < tokens.size(); i+=3) {
        string columnName = tokens[i];
        string columnType = tokens[i+1];
        if (columnType.back() == ',') {
            columnType.pop_back();
        }

        if (columnType != "INT" && columnType != "TEXT") {
            oss << "ERROR: Unsupported column type '" << columnType << "'. " << endl << "Supported types are INT and TEXT.";
            outputContent.push_back(oss.str());
            oss.clear();
            return;
        }

        Column column = {columnName, columnType};
        table.tableColumns.push_back(column);

    }
    outputContent.push_back("Table creation successful!");
}

void insertIntoTable(vector<string> &tokens) {
    vector<string> row;
    auto iterator = find(tokens.begin(), tokens.end(), "VALUES");

    if (iterator == tokens.end() || distance(tokens.begin(), iterator) + 2 >= tokens.size()) {
        outputContent.push_back("Error: VALUES keyword missing or invalid INSERT syntax." );
        return;
    }

    // Directly calculate index after the "VALUES" keyword
    int index = distance(tokens.begin(), iterator) + 2;

    // Start from the index after "VALUES" and collect valid values
    for (size_t i = index; i < tokens.size(); i++) {
        if (tokens[i] != "," && tokens[i] != ")" && !tokens[i].empty()) {
            row.push_back(tokens[i]);
        }
    }

    // Validate the row data based on column types
    if (!validateRowData(row)) {
        outputContent.push_back("Invalid data types in row. Insert operation aborted.");
        return;
    }

    table.tableRows.push_back(row);
    outputContent.push_back("");
}

void selectFromTable() {
    if (table.tableName.empty()) {
        outputContent.push_back("The table does not exist.");
        return;
    }

    ostringstream oss;

    // // Print column headers
    // for (size_t i = 0; i < table.tableColumns.size(); i++) {
    //     oss << table.tableColumns[i].columnName;
    //     if (i + 1 < table.tableColumns.size()) {
    //         oss << ",";
    //     }
    // }
    // outputContent.push_back(oss.str()); // Store column names as CSV
    // oss.clear();

    // Print rows
    for (size_t i = 0; i < table.tableRows.size(); i++) {
        if (i == 0) {
            for (size_t i = 0; i < table.tableColumns.size(); i++) {
                oss << table.tableColumns[i].columnName;
                if (i + 1 < table.tableColumns.size()) {
                    oss << ",";
                }
            }
        }
        oss << "\n";
        for (size_t j = 0; j < table.tableRows[i].size(); j++) {
            if (table.tableColumns[j].columnType == "TEXT") {
                oss << removeQuotesFromStringLit(table.tableRows[i][j]);
            } else {
                oss << table.tableRows[i][j];
            }
            if (j + 1 < table.tableRows[i].size()) {
                oss << ",";
            }
        }
    }
    outputContent.push_back(oss.str()); // Store row as CSV
    oss.clear();
}


string removeQuotesFromStringLit(string& str) {
    if (str.front() == '\'' && str.back() == '\'') {
        return str.substr(1, str.size() - 2);
    }
    return str;
}

void updateTable(vector<string>& tokens) {
    auto itSet = find(tokens.begin(), tokens.end(), "SET");
    auto itWhere = find(tokens.begin(), tokens.end(), "WHERE");

    if (itSet == tokens.end() || itWhere == tokens.end() || distance(itSet, itWhere) < 3) {
        outputContent.push_back("ERROR: Invalid UPDATE syntax.");
        return;
    }

    string tableName = tokens[1]; // Get table name
    if (tableName != table.tableName) {
        outputContent.push_back("ERROR: Table '" + tableName + "' does not exist.");
        return;
    }

    string columnUpdate = *(itSet + 1);
    string newValue = *(itSet + 3);
    string conditionColumn = *(itWhere + 1);
    string conditionValue = *(itWhere + 3);

    newValue = removeQuotesFromStringLit(newValue);
    conditionValue = removeQuotesFromStringLit(conditionValue);

    int updateColumnIndex = -1, conditionColumnIndex = -1;

    for (size_t i = 0; i < table.tableColumns.size(); ++i) {
        if (table.tableColumns[i].columnName == columnUpdate) updateColumnIndex = i;
        if (table.tableColumns[i].columnName == conditionColumn) conditionColumnIndex = i;
    }

    if (updateColumnIndex == -1 || conditionColumnIndex == -1) {
        outputContent.push_back("ERROR: Invalid column name in UPDATE statement.");
        return;
    }

    bool updated = false;
    for (auto& row : table.tableRows) {
        if (row[conditionColumnIndex] == conditionValue) {
            row[updateColumnIndex] = newValue;
            updated = true;
        }
    }

    outputContent.push_back(updated ? "Table update successful!" : "No matching rows found for update.");
}


void deleteFromTable(vector<string>& tokens) {
    auto itWhere = find(tokens.begin(), tokens.end(), "WHERE");
    if (itWhere == tokens.end() || distance(itWhere, tokens.end()) < 4) { // Corrected condition here
        outputContent.push_back("ERROR: Invalid DELETE syntax.");
        return;
    }

    string tableName = tokens[2]; //Get table name
    if (tableName != table.tableName) {
        outputContent.push_back("ERROR: Table '" + tableName + "' does not exist.");
        return;
    }

    string columnName = *(itWhere + 1);
    string conditionValue = *(itWhere + 3);

    conditionValue = removeQuotesFromStringLit(conditionValue);

    int columnIndex = -1;
    for (size_t i = 0; i < table.tableColumns.size(); ++i) {
        if (table.tableColumns[i].columnName == columnName) {
            columnIndex = i;
            break;
        }
    }

    if (columnIndex == -1) {
        outputContent.push_back("ERROR: Column not found in DELETE statement.");
        return;
    }

    size_t oldSize = table.tableRows.size();
    table.tableRows.erase(remove_if(table.tableRows.begin(), table.tableRows.end(),
                                    [&](const vector<string>& row) { return row[columnIndex] == conditionValue; }),
                          table.tableRows.end());

    outputContent.push_back((table.tableRows.size() < oldSize) ? "Table deletion successful!" : "No matching rows found for deletion.");
}


void countRows() {
  ostringstream oss;
  oss << table.tableRows.size();
  outputContent.push_back(oss.str());
  oss.clear();
}


bool validateRowData(const vector<string> &row) {
    for (int i = 0; i < table.tableColumns.size(); i++) {
        string columnType = table.tableColumns[i].columnType;
        string rowValue = row[i];

        if (columnType == "INT") {
            bool isInt = isInteger(rowValue);
            if (!isInt) {
                cerr << "ERROR: Mismatching data types at column " << i + 1 << ". Expected INT type but got '" << rowValue << "'" << endl;
                return false;
            }
        }
        else if (columnType == "TEXT") {
            if (rowValue.front() != '\'' && rowValue.back() != '\'') {
                cerr << "ERROR: Mismatching data types at column " << i + 1 << ". Expected TEXT but got '" << rowValue << "'" << endl;
                return false;
            }
        }
    }
    return true;
}

bool isInteger(const string& value) {
    if (value.empty()) return false;

    size_t start = (value[0] == '-') ? 1 : 0;    // ternary operator

    for (int i = start; i < value.size(); i++) {
        if (!isdigit(value[i])) {
            return false;
        }
    }
    return true;
}

void getInputCommand() {
    inputFile.open("fileInput2.mdb");
    string line;
    string buffer;

    while (getline(inputFile, line)) {
        if (line.empty()) continue;
        buffer += line + "\n";

        if (line.find(";") != std::string::npos) {
            inputContent.push_back(buffer);
            buffer.clear();
        }
    }
    for (int i = 0; i < inputContent.size(); i++) {
        cout << "Debug: InputContent[" << i << "] = " << inputContent[i] << endl;
    }
    inputFile.close();
}


void writeToOutputFile() {
    getInputCommand();

    for (int i = 0; i < outputContent.size(); i++) {
        cout << "Debug: OutputContent[" << i << "] = " << outputContent[i] << endl;
    }

    int inputContentSize = inputContent.size();     
    int outputContentSize = outputContent.size();

    for (int i = 0; i < inputContentSize; i++) {

        outputFile << "> " << inputContent[i];
        outputFile << outputContent[i] << "\n";
        
    }
}