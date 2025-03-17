#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

map<char, vector<string>> grammar = {
    {'S', {"AB" , "D"}},
    {'A', {"a", "ε"}},
    {'B', {"b", "ε"}},
    {'C', {"(S)", "c"}},
    {'D', {"AC"}}
};

map<char, set<char>> firstSets, followSets;
set<char> visitedFirst, visitedFollow;

void computeFirst(char symbol);
void computeFollow(char symbol);

void computeFirst(char symbol) {
    if (visitedFirst.count(symbol)) return;
    visitedFirst.insert(symbol);
    for (auto &prod : grammar[symbol]) {
        if (islower(prod[0]) || prod[0] == '(' || prod[0] == ')') {
            firstSets[symbol].insert(prod[0]);
        } else {
            int i = 0;
            while (i < prod.size()) {
                char nextSymbol = prod[i];
                computeFirst(nextSymbol);
                firstSets[symbol].insert(firstSets[nextSymbol].begin(), firstSets[nextSymbol].end());
                if (firstSets[nextSymbol].count('ε')) {
                    firstSets[symbol].erase('ε');
                    i++;
                } else break;
            }
            if (i == prod.size()) firstSets[symbol].insert('ε');
        }
    }
}

void computeFollow(char symbol) {
    if (visitedFollow.count(symbol)) return;
    visitedFollow.insert(symbol);
    if (symbol == 'S') followSets[symbol].insert('$');
    for (auto &[nonTerm, productions] : grammar) {
        for (auto &prod : productions) {
            for (int i = 0; i < prod.size(); i++) {
                if (prod[i] == symbol) {
                    int j = i + 1;
                    while (j < prod.size()) {
                        char nextSymbol = prod[j];
                        if (islower(nextSymbol) || nextSymbol == '(' || nextSymbol == ')') {
                            followSets[symbol].insert(nextSymbol);
                            break;
                        } else {
                            followSets[symbol].insert(firstSets[nextSymbol].begin(), firstSets[nextSymbol].end());
                            if (firstSets[nextSymbol].count('ε')) {
                                followSets[symbol].erase('ε');
                                j++;
                            } else break;
                        }
                    }
                    if (j == prod.size() || (j < prod.size() && firstSets[prod[j]].count('ε'))) {
                        computeFollow(nonTerm);
                        followSets[symbol].insert(followSets[nonTerm].begin(), followSets[nonTerm].end());
                    }
                }
            }
        }
    }
}

int main() {
    for (auto &[nonTerm, _] : grammar) computeFirst(nonTerm);
    for (auto &[nonTerm, _] : grammar) computeFollow(nonTerm);
    
    for (auto &[nonTerm, first] : firstSets) {
        cout << "First(" << nonTerm << ") = { ";
        for (char ch : first) cout << ch << " ";
        cout << "}\n";
    }
    
    for (auto &[nonTerm, follow] : followSets) {
        cout << "Follow(" << nonTerm << ") = { ";
        for (char ch : follow) cout << ch << " ";
        cout << "}\n";
    }
    return 0;
}
