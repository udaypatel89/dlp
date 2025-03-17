#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <stack>

using namespace std;

// Grammar rules
map<char, vector<string>> grammar = {
    {'S', {"AD"}},
    {'A', {"a", ""}},
    {'B', {"b", ""}},
    {'C', {"(C)", "c"}},
    {'D', {"BC"}}
};

// First sets
map<char, set<char>> first = {
    {'S', {'a', 'b', '(', 'c'}},
    {'A', {'a', '\0'}},
    {'B', {'b', '\0'}},
    {'C', {'(', 'c'}},
    {'D', {'a', '('}}
};

// Follow sets
map<char, set<char>> follow = {
    {'S', {')', '$'}},
    {'A', {'b', '(', ')', '$'}},
    {'B', {'c', ')', '$'}},
    {'C', {')', '$'}},
    {'D', {')', '$'}}
};

// Predictive Parsing Table
map<pair<char, char>, string> parsingTable;

void constructParsingTable() {
    for (auto &rule : grammar) {
        char nonTerminal = rule.first;
        for (auto &production : rule.second) {
            char firstSymbol = production[0];
            if (isalpha(firstSymbol) || firstSymbol == '(') {
                parsingTable[{nonTerminal, firstSymbol}] = production;
            } else if (firstSymbol == '\0') {
                for (char followSymbol : follow[nonTerminal]) {
                    parsingTable[{nonTerminal, followSymbol}] = production;
                }
            }
        }
    }
}

void printParsingTable() {
    cout << "\nPredictive Parsing Table:\n";
    for (auto &entry : parsingTable) {
        cout << "M[" << entry.first.first << ", " << entry.first.second << "] = " << entry.second << "\n";
    }
}

bool isLL1() {
    for (auto &rule : grammar) {
        char nonTerminal = rule.first;
        set<char> firstSet;
        for (auto &production : rule.second) {
            char firstSymbol = production[0];
            if (firstSet.find(firstSymbol) != firstSet.end()) {
                return false;
            }
            firstSet.insert(firstSymbol);
        }
    }
    return true;
}

bool validateString(string input) {
    stack<char> parseStack;
    parseStack.push('$');
    parseStack.push('S');
    input += '$';
    int index = 0;

    while (!parseStack.empty()) {
        char top = parseStack.top();
        char current = input[index];
        parseStack.pop();

        if (top == current) {
            index++;
        } else if (parsingTable.find({top, current}) != parsingTable.end()) {
            string production = parsingTable[{top, current}];
            if (production != "") {
                for (int i = production.length() - 1; i >= 0; --i) {
                    parseStack.push(production[i]);
                }
            }
        } else {
            return false;
        }
    }
    return (index == input.length());
}

int main() {
    constructParsingTable();
    printParsingTable();

    if (isLL1()) {
        cout << "\nThe grammar is LL(1)." << endl;
    } else {
        cout << "\nThe grammar is not LL(1)." << endl;
        return 0;
    }

    vector<string> testCases = {"abc", "ac", "(abc)", "c", "(ac)", "a", "()", "(ab)", "abcabc", "b"};
    
    for (string test : testCases) {
        if (validateString(test)) {
            cout << "Valid string: " << test << endl;
        } else {
            cout << "Invalid string: " << test << endl;
        }
    }

    return 0;
}
