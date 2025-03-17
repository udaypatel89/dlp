#include <iostream>
#include <string>

using namespace std;

class RecursiveDescentParser {
private:
    string input;
    int index;

    bool S();
    bool L();
    bool L_prime();

public:
    RecursiveDescentParser(string str) : input(str), index(0) {}
    bool parse();
};

bool RecursiveDescentParser::S() {
    if (index < input.length() && input[index] == '(') {
        index++;
        if (L()) {
            if (index < input.length() && input[index] == ')') {
                index++;
                return true;
            }
        }
        return false;
    } else if (index < input.length() && input[index] == 'a') {
        index++;
        return true;
    }
    return false;
}

bool RecursiveDescentParser::L() {
    if (S()) {
        return L_prime();
    }
    return false;
}

bool RecursiveDescentParser::L_prime() {
    if (index < input.length() && input[index] == ',') {
        index++;
        if (S()) {
            return L_prime();
        }
        return false;
    }
    return true; // epsilon case
}

bool RecursiveDescentParser::parse() {
    return S() && index == input.length();
}

int main() {
    string input;
    cout << "Enter input string: ";
    cin >> input;
    RecursiveDescentParser parser(input);
    if (parser.parse()) {
        cout << "Valid string" << endl;
    } else {
        cout << "Invalid string" << endl;
    }
    return 0;
}
