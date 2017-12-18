/* stack_calc.cpp
 *
 * This program is run as:
 *    stack_calc <filename>
 *
 * Input files describe a mathematical expression in prefix notation. stack_calc
 * computes a value. stack_calc does not handle erroneous input files or input
 * expressions.
 */

#include <stack>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
using namespace std;

//class token to store a number or operator
class Token {
public:
    Token(double _n) : num(_n), op('!') {}
    Token(char _o) : num(0.0), op(_o) {}
    
    double GetNum() const {return num;}
    bool IsOp() const {return op != '!';}
    char GetOp() const {return op;}
    
private:
    double num;
    char op;
};

//read the data from the file
void ParseFile(const string& _filename, stack<Token>& _calc) {
    ifstream ifs(_filename.c_str());
    
    double val;
    char c;
    
    while (ifs) {
        while (isspace(c = ifs.get())) { }
        if(c=='*' || c=='/' || c=='+' || c=='-')
            _calc.push(Token(c));
        else if (ifs.putback(c) >> val)
            _calc.push(Token(val));
    }
}

//calculate answer
double Calculate(stack<Token>& _calc) {
    while (_calc.size() > 1) {
        double val2 = _calc.top().GetNum();
        _calc.pop();
        double val1 = _calc.top().GetNum();
        _calc.pop();
        char op = _calc.top().GetOp();
        _calc.pop();
        
        switch (op) {
            case '/':
                _calc.push(Token(val1/val2));
                break;
            case '*':
                _calc.push(Token(val1*val2));
                break;
            case '+':
                _calc.push(Token(val1+val2));
                break;
            case '-':
                _calc.push(Token(val1-val2));
                break;
            default:
                break;
        }
    }
    
    return _calc.top().GetNum();
}

int main(int argc, char** argv) {
    //ensure the user only provides one filename
    if(argc != 2) {
        cerr << "Error stack_calc takes one argument only." << endl;
        exit(1);
    }
    
    //stack calculator to store the data
    stack<Token> calc;
    
    //read the data from the file
    string file = argv[1];
    ParseFile(file, calc);
    
    //calculate answer and report on screen
    cout << Calculate(calc) << endl;
    
    return 0;
}
