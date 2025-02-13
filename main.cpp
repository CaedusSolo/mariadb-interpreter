//  ************************************************************************************************
//  Program: main.cpp
//  Course: CCP6114 Programming Fundamentals
//  Lecture Class: TC1L
//  Tutorial Class: TT1L
//  Trimester: 2430
//  Member_1: 242UC244NK | CHOW YING TONG | chow.ying.tong@student.mmu.edu.my 
//  Member_2: 242UC244NK | LAW CHIN XUAN | law.chin.xuan@student.mmu.edu.my 
//  ************************************************************************************************
//  Task Distribution
//  Member_1: Read input file, tokenization, create table, insert into table, delete from table, write to output file, parts of update table
//  Member_2: Count table rows, parts of update table, pseudocode
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
#include <tuple>
using namespace std;

ifstream inputFile;
ofstream outputFile;

// struct representing table column
struct Column {
    string columnName;
    string columnType;
};

//  struct representing table
struct Table {
    string tableName;
    vector<Column> tableColumns;
    vector<vector<string>> tableRows;
};

const string INPUT_FILE = "fileInput3.mdb";  //  input file name
vector<string> inputContent;  //  global vector to store each line in input
vector<string> outputContent; //  global vector to store the corresponding output for each input


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
tuple<string, string> extractValuesFromEqualSign(string&);
void getInputCommands();

Table table;        //  global table variable

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


vector<string> tokenize(const string& input) {  //  splits the input file into tokens to be stored into a vector
//  for example, SELECT * FROM customer; ---> {"SELECT", "*", "FROM", "customer", ";"}

    vector<string> tokens;  //  declare vector variable to store the result 
    stringstream ss(input);  //  feed input variable (the entire input file content) into stringstream to be processed
    string token;   //  declare string variable to store each token
    char c;   //  variable to store each character

    while (ss.get(c)) {  // read each character in the input string
        if (isspace(c)) {   //  if a space is encountered, save token variable to tokens vector
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else if (c == '(' || c == ')' || c == ',' || c == ';' || c == '*') {  // if any of the symbols is encountered, save previous string into tokens vector
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(string(1, c));    //  append the symbol as a token into tokens vector
        } else {
            token += c;  //  append the character to the token string
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);  //  only append token to tokens if token is populated
    }
    //  for example, SELECT * FROM customer; ---> {"SELECT", "*", "FROM", "customer", ";"}
    return tokens;
}

int main() {
    readFileInput();   //  call readFileInput function to start processing input file
    writeToOutputFile();  //  call writeToOutputFile function to write output to outputFile
    return 0;
}

void readFileInput() {
    inputFile.open(INPUT_FILE);
    if (!inputFile.is_open()) {
        cerr << "Error opening input file." << endl;
        return;
    }

    stringstream buffer;  //  create stringstream object to store content of input file
    buffer << inputFile.rdbuf();  
    string fileContent = buffer.str();  //  save buffer to a string variable fileContent
    inputFile.close();   //  close input file

    vector<string> allTokens = tokenize(fileContent);  //  call tokenize function, passing in fileContent as argument. 
                                                       //  save the result to a vector variable allTokens

    //  Loop through every token and process                                                   
    for (size_t i = 0; i < allTokens.size(); ++i) {
     // Check if the current token is "CREATE"
        if (allTokens[i] == "CREATE") {
            // If yes, check if "TABLE" comes right after "CREATE"
            if (allTokens[i + 1] == "TABLE") {
                vector<string> createTableTokens; // Temporary variable to store tokens related to CREATE TABLE syntax
                size_t j = i; // Initialize j to the current index i

            // Collect tokens for the CREATE TABLE statement until a semicolon is encountered
                while (j < allTokens.size() && allTokens[j] != ";") {
                    createTableTokens.push_back(allTokens[j]);  // Add the current token to the vector
                    j++;    // Increment j to move to the next token
                }
                createTable(createTableTokens); // Call the createTable function to process the collected tokens
                i = j;  // Update i to j to skip the processed tokens in the outer loop
            } else if (allTokens[i + 1].find(".txt") != string::npos) {
                createOutputFile(allTokens[i + 1]);     // call createOutputFile function
        }

    } else if (allTokens[i] == "INSERT" && allTokens[i + 1] == "INTO") {
        vector<string> insertTokens; // Temporary variable to store tokens related to INSERT INTO syntax
        size_t j = i; // Initialize j to the current index i

        // Collect tokens for the INSERT INTO statement until a semicolon is encountered
        while (j < allTokens.size() && allTokens[j] != ";") {
            insertTokens.push_back(allTokens[j]); // Add the current token to the vector
            j++; // Increment j to move to the next token
        }
        insertIntoTable(insertTokens); // Call the insertIntoTable function to process the collected tokens
        i = j; // Update i to j to skip the processed tokens in the outer loop

    } else if (allTokens[i] == "DELETE") {
        vector<string> deleteTokens; // Temporary variable to store tokens related to DELETE statement
        size_t j = i; // Initialize j to the current index i

        // Collect tokens for the DELETE statement until a semicolon is encountered
        while (j < allTokens.size() && allTokens[j] != ";") {
            deleteTokens.push_back(allTokens[j]); // Add the current token to the vector
            j++; // Increment j to move to the next token
        }
        deleteFromTable(deleteTokens); // Call the deleteFromTable function to process the collected tokens
        i = j; // Update i to j to skip the processed tokens in the outer loop

    } else if (allTokens[i] == "UPDATE") {
        vector<string> updateTokens; // Temporary variable to store tokens related to UPDATE statement
        size_t j = i; // Initialize j to the current index i

        // Collect tokens for the UPDATE statement until a semicolon is encountered
        while (j < allTokens.size() && allTokens[j] != ";") {
            updateTokens.push_back(allTokens[j]);       // Add the current token to the vector
            j++;        // Increment j to move to the next token
        }
        updateTable(updateTokens); // Call the updateTable function to process the collected tokens
        i = j;               // Update i to j to skip the processed tokens in the outer loop

    } else if (allTokens[i] == "SELECT") { 
        if (allTokens[i + 1] == "*" && allTokens[i+2] == "FROM") {   //  if syntax is correct 
            if (allTokens[i+3] == table.tableName) {  //  if table name and syntax are correct, call selectFromTable function
                selectFromTable();
            }
            else {
                outputContent.push_back("ERROR: Table does not exist.");  //  if table name does not match, append error message to outputContent
            }
        }

        else if (allTokens[i+1] == "COUNT") {
            //  if syntax for COUNT is correct (SELECT (*) FROM table;) then call countRows() function to count number of rows 
            if (allTokens[i+2] == "(" && allTokens[i+3] == "*" && allTokens[i+4] == ")") {  
                countRows();
            }
        }
    } else if (allTokens[i] == "DATABASES") {
        auto filePath = filesystem::absolute(INPUT_FILE);    // get absolute path of input file
            outputContent.push_back(filePath.string());   //  append absolute path to global outputContent vector
        }

        else if (allTokens[i] == "TABLES") {
            outputContent.push_back(table.tableName);  //  if current token is "TABLES", save table name to global outputContent vector
        }
    }
}

void createOutputFile(string &fileName) {
    ostringstream oss;  //  oss stream to build output content string
    
    if (fileName.back() == ';') {
        fileName.pop_back();  //  remove trailing ; from file name
    }
    
    outputFile.open(fileName);  //  open file
    if (outputFile.is_open()) {
        oss << "Output file " << fileName << " created successfully!";
        outputContent.push_back(oss.str());  //  if file creation succeeded, append success message to outputContent vector
    }
    else {
        outputContent.push_back("ERROR: Failed to created output file.");  //  if file creation failed, append error message to outputContent vector
        return;
    }
}

//  example content of tokens vector: {"CREATE", "TABLE", "customer", "(", "customer_id", "INT", ",", "customer_name", "TEXT", ",", "customer_city", "TEXT"}
void createTable(vector<string> &tokens) { 
    string tableName = tokens[2];  //  get table name
    ostringstream oss;  // oss object to build output string

    table.tableName = tableName;  //  set tableName in Table struct
    
    //  loop through tokens to extract column name and column type
    for (size_t i = 4; i < tokens.size(); i+=3) {
        string columnName = tokens[i];
        string columnType = tokens[i+1];
        if (columnType.back() == ',') {
            columnType.pop_back();  // remove , from column type (example: TEXT, --> TEXT) 
        }

        //  if columnType is anything other than INT or TEXT, append error message to outputContent
        if (columnType != "INT" && columnType != "TEXT") {
            oss << "ERROR: Unsupported column type '" << columnType << "'. " << endl << "Supported types are INT and TEXT.";
            outputContent.push_back(oss.str());
            oss.clear();
            return;
        }

        Column column = {columnName, columnType};  //  initialize column object
        table.tableColumns.push_back(column);  //  append the column object to tableColumns vector
    }
    outputContent.push_back("");  //  append empty string because table creation does not produce output
}


//  example content of tokens vector: 
//  {"INSERT", "INTO", "customer", "(", "customer_id", "INT", ",", "customer_name", 
//   "TEXT", ",", "customer_city", "TEXT", ")", "VALUES", "(", "1", "'Jack'","'Subang'",")"}
void insertIntoTable(vector<string> &tokens) {
    vector<string> row;  //  variable to store new row to be inserted into table 
    auto iterator = find(tokens.begin(), tokens.end(), "VALUES");  //  find if "VALUES" is found in tokens vector

    //  iterator will return tokens.end() if not found.
    if (iterator == tokens.end() || distance(tokens.begin(), iterator) + 2 >= tokens.size()) {
        outputContent.push_back("ERROR: VALUES keyword missing or invalid INSERT syntax." );
        return;
    }

    // Directly calculate index after the "VALUES" keyword. 
    int index = distance(tokens.begin(), iterator) + 2;

    // Start from the index after "VALUES" and collect valid values
    for (size_t i = index; i < tokens.size(); i++) {
        if (tokens[i] != "," && tokens[i] != ")" && !tokens[i].empty()) {
            //  if tokens[i] is not , and is not ) and is not empty, append tokens[i] to row vector
            row.push_back(tokens[i]);
        }
    }

    // Validate the row data based on column types
    if (!validateRowData(row)) {
        outputContent.push_back("ERROR: Invalid data types in row. Insert operation aborted.");
        return;
    }

    if (table.tableRows.size() == 10) {
        outputContent.push_back("ERROR: Table is at maximum size of 10 rows.");
        return;
    }

    if (table.tableRows.size() == 10) {  //  if table already has 10 rows, append error message to outputContent
        outputContent.push_back("ERROR: Table is already at maximum size of 10 rows.");
        return;
    }
    
    table.tableRows.push_back(row);       //  append newly created row to tableRows vector
    outputContent.push_back("");  //  append empty string to outputContent because table insertion does not produce output.
}

void selectFromTable() {
    if (table.tableName.empty()) {
        outputContent.push_back("ERROR: The table does not exist.");
        return;
    }

    ostringstream oss;  // oss for building output string

    // Print rows
    for (size_t i = 0; i < table.tableRows.size(); i++) {
        if (i == 0) {  //  print table headers for first row only
            for (size_t i = 0; i < table.tableColumns.size(); i++) {
                oss << table.tableColumns[i].columnName;
                if (i + 1 < table.tableColumns.size()) {
                    oss << ",";     //  add , at the end of each token except for the last one of each line
                }
            }
        }
        oss << "\n";  //  start new line after table headers
        for (size_t j = 0; j < table.tableRows[i].size(); j++) {
            if (table.tableColumns[j].columnType == "TEXT") {
                oss << removeQuotesFromStringLit(table.tableRows[i][j]);  // removes quotes from string literals so that the output for TEXT is without '' 
            } else {
                oss << table.tableRows[i][j]; 
            }
            if (j + 1 < table.tableRows[i].size()) {
                oss << ",";  //  add , at the end of each token except for the last one of each line
            }
        }
    }
    outputContent.push_back(oss.str()); // Store row as CSV
    oss.clear();
}

//  helper function that removes '' from string literals. 
string removeQuotesFromStringLit(string& str) {
    if (str.front() == '\'' && str.back() == '\'') {
        return str.substr(1, str.size() - 2);
    }
    return str;
}

void updateTable(vector<string>& tokens) {

    auto itSet = find(tokens.begin(), tokens.end(), "SET");  //  find location of "SET" in tokens
    auto itWhere = find(tokens.begin(), tokens.end(), "WHERE");  //  find location of WHERE in tokens

    //  if SET and WHERE are not in tokens, append error message and exit
    if (itSet == tokens.end() || itWhere == tokens.end()) {
        outputContent.push_back("ERROR: Invalid UPDATE syntax.");
        return;
    }

    string tableName = tokens[1]; // Get table name
    if (tableName != table.tableName) {
        outputContent.push_back("ERROR: Table '" + tableName + "' does not exist.");
        return;
    }

    tuple<string, string> setParts = extractValuesFromEqualSign(tokens[3]); //  call extractValuesFromEqualSign function, passing in SET condition
    string columnUpdate; 
    string newValue;    
    tie(columnUpdate, newValue) = setParts;  //  assign columnUpdate to first value in tuple, newValue to second value in tuple

    tuple<string, string> whereParts = extractValuesFromEqualSign(tokens[5]);  // call extractValuesFromEqualSign function, passing in WHERE condition
    string conditionColumn;   
    string conditionValue;  
    tie(conditionColumn, conditionValue) = whereParts;  //  assign conditionColumn to first value in tuple, conditionValue to second value in tuple

    newValue = removeQuotesFromStringLit(newValue);  //  remove '' from newValue
    conditionValue = removeQuotesFromStringLit(conditionValue);  // remove '' from newValue

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
    for (auto& row : table.tableRows) { // Iterate through the actual rows in the table
        if (row[conditionColumnIndex] == conditionValue) {
            row[updateColumnIndex] = newValue; // Correctly update the element in the row
            updated = true;
        }
    }
     //  if table was updated, append empty string because update operation does not produce output.
     //  if table was not updated, append "No matching rows found for update."
    outputContent.push_back(updated ? "" : "No matching rows found for update."); }


void deleteFromTable(vector<string>& tokens) {
    auto itWhere = find(tokens.begin(), tokens.end(), "WHERE");
    if (itWhere == tokens.end()) {  //  if "WHERE" is not found in tokens, append error message to outputContent and exit 
        outputContent.push_back("ERROR: Invalid DELETE syntax.");
        return;
    }

    string tableName = tokens[2]; 
    //  if table specified in DELETE clause is not the table name, append error message and exit
    if (tableName != table.tableName) {
        outputContent.push_back("ERROR: Table '" + tableName + "' does not exist.");
        return;
    }

    string columnName;
    string conditionValue;
    tuple<string, string> whereParts = extractValuesFromEqualSign(tokens[4]);  // extract value on either side of = in WHERE clause 
    tie(columnName, conditionValue) = whereParts;  // assign columnName to first value in tuple, conditionValue to second value in tuple

    conditionValue = removeQuotesFromStringLit(conditionValue);  // remove '' from conditionValue

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
    table.tableRows.erase(std::remove_if(table.tableRows.begin(), table.tableRows.end(),
                                     [&](const std::vector<std::string>& row) { return row[columnIndex] == conditionValue; }),
                          table.tableRows.end());

    outputContent.push_back((table.tableRows.size() < oldSize) ? "" : "No matching rows found for deletion.");
}

//  helper function to extract token on either side of '=' char
//  this function assumes that there is no space on either side of '=' char
tuple<string, string> extractValuesFromEqualSign(string& str) {
    size_t equalPos = str.find('=');  //  get index of =
    string part1 = str.substr(0, equalPos);  //  get the token on the left side of =
    string part2 = str.substr(equalPos + 1);  //  get the token on the right side of =

    return tuple<string, string>(part1, part2);   //  returns tuple of 2 strings
}

void countRows() {
    ostringstream oss;
    oss << table.tableRows.size();  //  get the size of tableRows
    outputContent.push_back(oss.str());  //  append number of rows to outputContent
    oss.clear();
}

//  helper function to check if data types match
//  mechanism: TEXT values must be enclosed within ''.
//             INT values must not be enclosed within ''.
bool validateRowData(const vector<string> &row) {
    for (int i = 0; i < table.tableColumns.size(); i++) {
        string columnType = table.tableColumns[i].columnType;
        string rowValue = row[i]; 

        if (columnType == "INT") {
            bool isInt = isInteger(rowValue);  // call helper function isInteger to validate if rowValue is INT
            if (!isInt) {
                return false;
            }
        }
        else if (columnType == "TEXT") {
            //  if rowValue is not enclosed within '', return false  
            if (rowValue.front() != '\'' && rowValue.back() != '\'') {
                return false;
            }
        }
    }
    return true;
}

//  helper function to check if a value entered is an integer
bool isInteger(const string& value) {
    if (value.empty()) return false;

    size_t start = (value[0] == '-') ? 1 : 0;    // check if it starts with - to allow for negative integers

    for (int i = start; i < value.size(); i++) {
        if (!isdigit(value[i])) {
            return false;  //  loop through value, if value is not a digit, return false to indicate that it is not integer
        }
    }
    return true;  //  if every character in value is digit, return true to indicate it is integer
}

//  Function to save each input into global inputContent vector
void getInputCommands() {
    inputFile.open(INPUT_FILE);   //  open input file
    string line;  // variable to hold each line
    string buffer;  //  variable to store line and "\n"

    while (getline(inputFile, line)) {  //  read by line
        if (line.empty()) continue;     //  skip empty lines
        buffer += line + "\n";          //  save each line along with a new line character to buffer

        //  if ";" is found in line, append buffer to global inputContent vector
        if (line.find(";") != std::string::npos) { 
            inputContent.push_back(buffer);
            buffer.clear();  //  clear buffer after appending
        }
    }
    inputFile.close();   //  close input file after processing
}

//  Function to write output to output file
void writeToOutputFile() {

    getInputCommands();  //  call getInputCommands function to get all the inputs
    int inputContentSize = inputContent.size();     

    for (int i = 0; i < inputContentSize; i++) {  
        outputFile << "> " << inputContent[i];   //  display each input with a leading "> "
        outputFile << outputContent[i] << "\n";  //  display corresponding output
    }
}
