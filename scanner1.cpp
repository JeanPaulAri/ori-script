#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <list>
#include <sstream>
#include <fstream>
#include <stack>

using namespace std;

bool isToken(const string& str, const set<string>& querySet) {
    return querySet.find(str) != querySet.end();
}

set<string> tokens = {"ID", "LOAD", "SAVE", "EXPORT", "PLAY", "PAUSE", "STOP", "GOTO", "SPEED", "VOLUME", "MUTE", "FILTER", "FORMAT", "$"};

// Updated token values
map<string,string> tokensValue = {
    {"ID", "TEXT"},
    {"LOAD", "LOAD_VIDEO"},
    {"SAVE", "SAVE_VIDEO"},
    {"EXPORT", "EXPORT_VIDEO"},
    {"PLAY", "PLAY_VIDEO"},
    {"PAUSE", "PAUSE_VIDEO"},
    {"STOP", "STOP_VIDEO"},
    {"GOTO", "GOTO_POSITION"},
    {"SPEED", "CHANGE_SPEED"},
    {"VOLUME", "ADJUST_VOLUME"},
    {"MUTE", "MUTE_AUDIO"},
    {"FILTER", "APPLY_FILTER"},
    {"FORMAT", "SET_FORMAT"},
    {"$", "EOP"}
};

struct Token{
    string type = "";
    string value = "";

    int line, column;

    Token(string type="", string value="",int line=0, int column=0){
        this->type = type;
        this->value = value;
        this->line = line;
        this->column = column;
    }
};

vector<Token> scan(const string& input){
    vector<Token> tokensVector;
     size_t pos = 0;
     int line = 1, column = 1;
 
     auto getchr = [&]() -> char {
         if (pos >= input.size()) {
             return '\0';
         }
         char ch = input[pos++];
         if (ch == '\n') {
             line++;
             column = 1;
         } else {
             column++;
         }
         return ch;
     };
 
     auto peekchar = [&]() -> char {
         if (pos >= input.size()) {
             return '\0';
         }
         return input[pos];
     };
 
     auto getToken = [&]() -> Token {
         char ch;
         while ((ch = getchr()) != '\0' && isspace(ch)) {
             // Ignora los espacios en blanco
         }
 
         if (ch == '\0') {
             return {"$", "", line, column};
         }
 
         Token token;
         token.line = line;
         token.column = column - 1;
 
         string str(1, ch);
         
         // video keywords
         if (isalpha(ch)) {
             // Read the entire word
             while (isalpha(peekchar())) {
                 str += getchr();
             }
             
             // verifica si es un keyword
             if (isToken(str, tokens)) {
                 token.type = str;
                 token.value = tokensValue[str];
             } else {
                 token.type = "ID";
                 token.value = str;
             }
         } else if (isdigit(ch)) {
             while (isdigit(ch = peekchar())) {
                 str += getchr();
             }
             token.type = "ID";
             token.value = str;
         } else if (ispunct(ch) && !isToken(string(1, ch), tokens)) {
             while (ispunct(peekchar()) && !isToken(string(1, peekchar()), tokens)) {
                 str += getchr();
             }
             token.type = "ID";
             token.value = str;
         } else if (ch == '$') {
             token.type = "$";
             token.value = "EOP";
         } else {
             cerr << " SCAN ERROR en (" << line << ":" << column << "): caracter inesperado '" << ch << "'\n";
             token.type = "ERROR";
             token.value = str;
         }
 
         return token;
     };
 
     Token token;
     while ((token = getToken()).type != "$") {
         if(token.type != "ERROR"){
             tokensVector.push_back(token);
         }
     }
     tokensVector.push_back({"$", "EOP", line, column}); // Add end-of-program marker
     
     return tokensVector;
 }

 void printTokens (const vector<Token>& tokens) {
    cout << "INFO SCAN - Start scanning...\n";
    for (const auto& token : tokens) {
        cout << "DEBUG SCAN - " << token.type << " [ " << token.value << " ] found at (" << token.line << ":" << token.column << ")\n";
    }
    cout << "INFO SCAN - Completed with 0 errors\n"<<endl;
}

int main(){

    //initTables();
    ifstream file("input.txt");

    if (!file) {
        cerr << "No se pudo abrir el archivo input.txt";
        return 1; // Retorna un código de error
    }

    stringstream buffer;

    buffer << file.rdbuf();
    string input = buffer.str();
    vector<Token> tokens = scan(input);
    printTokens(tokens);
   
    return 0;
}
