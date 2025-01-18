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
void countRows(vector<string>& tokens);
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
            return;
        }

        Column column = {columnName, columnType};
        table.tableColumns.push_back(column);

        outputContent.push_back("null");
    }
}

void insertIntoTable(vector<string> &tokens) {
    // for (size_t i = 0; i < tokens.size(); i++) {
    //     cout << "insertIntoTable Token: " << tokens[i] << endl;
    // }      

    //  output for above for loop:

// insertIntoTable Token: INSERT
// insertIntoTable Token: INTO
// insertIntoTable Token: customer
// insertIntoTable Token: (
// insertIntoTable Token: customer_id
// insertIntoTable Token: ,
// insertIntoTable Token: customer_name
// insertIntoTable Token: ,
// insertIntoTable Token: customer_city
// insertIntoTable Token: ,
// insertIntoTable Token: customer_state
// insertIntoTable Token: ,
// insertIntoTable Token: customer_country
// insertIntoTable Token: ,
// insertIntoTable Token: customer_phone
// insertIntoTable Token: ,
// insertIntoTable Token: customer_email
// insertIntoTable Token: )
// insertIntoTable Token: VALUES
// insertIntoTable Token: (
// insertIntoTable Token: 1
// insertIntoTable Token: ,
// insertIntoTable Token: 'name1'
// insertIntoTable Token: ,
// insertIntoTable Token: 'city1'
// insertIntoTable Token: ,
// insertIntoTable Token: 'state1'
// insertIntoTable Token: ,
// insertIntoTable Token: 'country1'
// insertIntoTable Token: ,
// insertIntoTable Token: 'phone1'
// insertIntoTable Token: ,
// insertIntoTable Token: 'email1'
// insertIntoTable Token: )
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

    // Now, insert the row into table's rows
    table.tableRows.push_back(row);
    outputContent.push_back("null");

 // for (int i = 0; i < row.size(); i++) {
 //     cout << "Row token: " << row[i] << endl;
//     cout << "Column In Table: " << table.tableColumns[i].columnName << endl;
// }
// Output of above for loop:
// Row token: 1
// Row token: 'name1'
// Row token: 'city1'
// Row token: 'state1'
// Row token: 'country1'
// Row token: 'phone1'
// Row token: 'email1'

    // for (const Column& col : table.tableColumns) {
    //     cout << "Column in Table: " << col.columnName << endl;
    // }

    
// Column in Table: customer_id
// Column in Table: customer_name
// Column in Table: customer_city
// Column in Table: customer_state
// Column in Table: customer_country
// Column in Table: customer_phone
// Column in Table: customer_email
}

void selectFromTable() {
    if (table.tableName.empty()) {
        outputContent.push_back("The table does not exist.");
        return;
    }

    // if (table.tableRows.empty()) {
    //     cerr << "Table does not have any values." << endl;
    //     return;
    // }

    for (size_t i = 0; i < table.tableColumns.size(); i++) {
        cout << table.tableColumns[i].columnName;
        if (i + 1 < table.tableColumns.size()) { // Only add a comma if not the last element
            cout << ",";
        }
    }
    cout << endl;

    for (size_t i = 0; i < table.tableRows.size(); i++) {
        for (size_t j = 0; j < table.tableRows[i].size(); j++) {
            if (table.tableColumns[j].columnType == "TEXT") {
                if (j + 1 < table.tableRows[i].size()) {
                    outputContent.push_back(removeQuotesFromStringLit(table.tableRows[i][j]) + ",");
                }
                else {
                    outputContent.push_back(removeQuotesFromStringLit(table.tableRows[i][j]));
                }
            }
            else {
                if (j + 1 < table.tableRows[i].size()) {
                    outputContent.push_back(table.tableRows[i][j] + ",");
                }
                else {
                    outputContent.push_back(table.tableRows[i][j]);
                }
            }
        }
        cout << endl;
    }
}

string removeQuotesFromStringLit(string& str) {
    if (str.front() == '\'' && str.back() == '\'') {
        return str.substr(1, str.size() - 2);
    }
    return str;
}

void updateTable(vector<string>& tokens) {
    if (find(tokens.begin(), tokens.end(), "WHERE") == tokens.end()) {  // if "WHERE" is not found, terminate operation
        outputContent.push_back("ERROR: Invalid UPDATE syntax.");
        return;
    }

    string columnUpdate, newValue, conditionColumn, conditionValue;

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "SET") {
            columnUpdate = tokens[i + 1].substr(0, tokens[i + 1].find('='));
            newValue = tokens[i + 1].substr(tokens[i + 1].find('=') + 1);

            if (newValue.front() == '\'' && newValue.back() == '\'') {
                newValue = newValue.substr(1, newValue.size() - 2); // Remove quotes for internal processing
            }
        } else if (tokens[i] == "WHERE") {
            conditionColumn = tokens[i + 1].substr(0, tokens[i + 1].find('='));
            conditionValue = tokens[i + 1].substr(tokens[i + 1].find('=') + 1);

            if (conditionValue.front() == '\'' && conditionValue.back() == '\'') {
                conditionValue = conditionValue.substr(1, conditionValue.size() - 2); // Remove quotes for internal processing
            }
        }
    }

    size_t updateColumnIndex = -1, conditionColumnIndex = -1;

    for (size_t i = 0; i < table.tableColumns.size(); ++i) {
        if (table.tableColumns[i].columnName == columnUpdate) {
            updateColumnIndex = i;
        }
        if (table.tableColumns[i].columnName == conditionColumn) {
            conditionColumnIndex = i;
        }
    }

    bool updated = false;

    if (updateColumnIndex != -1 && conditionColumnIndex != -1) {
        for (auto& row : table.tableRows) {
            if (row[conditionColumnIndex] == conditionValue) {
                row[updateColumnIndex] = newValue;
                updated = true;
            }
        }
    }
    outputContent.push_back("null");
}

void deleteFromTable(vector<string>& tokens) {

    if (find(tokens.begin(), tokens.end(), "WHERE") == tokens.end()) {  // if "WHERE" is not found, terminate operation
        outputContent.push_back("ERROR: Invalid DELETE syntax.");
        return;
    }

    string columnName, values;
    ostringstream oss;
    
    for (size_t i = 0; i < tokens.size(); ++i) {

        if (tokens[i] == "WHERE") {
            columnName = tokens[i + 1].substr(0, tokens[i + 1].find('='));
            values = tokens[i + 1].substr(tokens[i + 1].find('=') + 1);

            if (values.front() == '\'' && values.back() == '\'') {
                values = values.substr(1, values.size() - 2); 
            }
            break;
        }
    }

    int columnIndex = -1;
    for (int i = 0; i < table.tableColumns.size(); ++i) {

        if (table.tableColumns[i].columnName == columnName) {
            columnIndex = i;
            break;
        }
    }

    if (columnIndex == -1) {
        oss << "Did not find this column " << columnName;
        outputContent.push_back(oss.str());
        return;
    }

 
    table.tableRows.erase(
        remove_if(table.tableRows.begin(), table.tableRows.end(),
                  [&](const vector<string>& row) { return row[columnIndex] == values; }),
        table.tableRows.end()
    );

    outputContent.push_back("null");
}

void countRows(vector<string>& tokens) {
    ostringstream oss;
    if (tokens.size() >= 4 && tokens[0] == "SELECT" && tokens[1] == "COUNT(*)" && tokens[2] == "FROM") {
        string tableName = tokens[3];

        if (tableName == table.tableName) {
            oss << table.tableRows.size();
            outputContent.push_back(oss.str());
        } else {
            oss << "ERROR: Table '" << tableName << "' does not exist.";
            outputContent.push_back(oss.str());
        }

    } else {
        outputContent.push_back("Unable to count");
    }
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
        buffer += line + "\n";

        if (line.find(";") != std::string::npos) {
            inputContent.push_back(buffer);
            buffer.clear();
        }
    }
    inputFile.close();
}

void writeToOutputFile() {
    getInputCommand();

    int inputContentSize = inputContent.size();     
    int outputContentSize = outputContent.size();

    for (int i = 0; i < inputContentSize; i++) {
        outputFile << "> " << inputContent[i];
        if (inputContent[i].find("DATABASES;") != std::string::npos) {
            auto filePath = filesystem::absolute("fileInput2.mdb");
            outputFile << filePath << "\n";       
        }
        else if (inputContent[i].find("TABLES;") != std::string::npos) {
            outputFile << table.tableName << "\n";
        }
        else if (outputContent[i] == "null") continue;
        else {
            outputFile << outputContent[i] << "\n";
        }
    }
}
