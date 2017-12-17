#include "std_lib_facilities_3.h"

//------------------------------------------------------------------------------

class Token {
public:
    char kind;        // what kind of token
    bool value;     // for numbers: a value
    Token(char ch)    // make a Token from a char
    :kind(ch), value(false) { }
    Token(char ch, bool val)     // make a Token from a char and a bool
    :kind(ch), value(val) { }
};

//------------------------------------------------------------------------------

class Token_stream {
public:
    Token_stream();   // make a Token_stream that reads from cin
    Token get();      // get a Token (get() is defined elsewhere)
    void putback(Token t);    // put a Token back
private:
    bool full;        // is there a Token in the buffer?
    Token buffer;     // here is where we keep a Token put back using putback()
};

//------------------------------------------------------------------------------

// The constructor just sets full to indicate that the buffer is empty:
Token_stream::Token_stream()
:full(false), buffer(0)    // no Token in buffer
{
}

//------------------------------------------------------------------------------

// The putback() member function puts its argument back into the Token_stream's buffer:
void Token_stream::putback(Token t)
{
    if (full) error("putback() into a full buffer");
    buffer = t;       // copy t to buffer
    full = true;      // buffer is now full
}

//------------------------------------------------------------------------------

Token Token_stream::get()
{
    if (full) {       // do we already have a Token ready?
        // remove token from buffer
        full=false;
        return buffer;
    }
    
    char ch;
    cin >> ch;
    
    switch (ch) {
        case ';':    // for "print"
        case 'q':    // for "quit"
        case '(': case ')': case '&': case '|': case '^':
            return Token(ch);        // let each character represent itself
        case '0': case '1': {
            cin.putback(ch);         // put value back into the input stream
            int int_val;
            cin >> int_val;              // read a floating-point number
            bool val = int_val;
            return Token('3',val);   // let '3' represent a boolean
        }
        default:
            error("Bad token");
    }
}

//------------------------------------------------------------------------------

Token_stream ts;        // provides get() and putback()

//------------------------------------------------------------------------------

bool expression();    // declaration so that primary() can call expression()

//------------------------------------------------------------------------------

// deal with booleans and parentheses
bool primary()
{
    Token t = ts.get();
    switch (t.kind) {
        case '(':    // handle '(' expression ')'
        {
            bool d = expression();
            t = ts.get();
            if (t.kind != ')') error("')' expected");
            return d;
        }
        case '3':            // we use '3' to represent a boolean
            return t.value;  // return the boolean's value
        default:
            error("primary expected");
    }
}

//------------------------------------------------------------------------------
//deals with &
bool sub()
{
    bool left = primary();
    Token t = ts.get();        // get the next token from token stream
    bool r;
    while(true) {
        switch (t.kind) {
            case '&':
                r = primary();
                left = left && r;
                t = ts.get();
                break;
            default:
                ts.putback(t);     // put t back into the token stream
                return left;
        }
    }
}

//------------------------------------------------------------------------------
//deals with ^
bool term()
{
    bool left = sub();
    Token t = ts.get();        // get the next token from token stream
    bool r;
    while(true) {
        switch (t.kind) {
            case '^':
                r = sub();
                left = left ^ r;
                t = ts.get();
                break;
            default:
                ts.putback(t);     // put t back into the token stream
                return left;
        }
    }
}

//------------------------------------------------------------------------------
//deals with |
bool expression()
{
    bool left = term();     // read and evaluate a Term
    Token t = ts.get();     // get the next token from token stream
    bool r;
    while(true) {
        switch(t.kind) {
            case '|':
                r = term();
                left = left || r;    // evaluate Term
                t = ts.get();
                break;
            default:
                ts.putback(t);     // put t back into the token stream
                return left;       //return the answer
        }
    }
}

//------------------------------------------------------------------------------

int main() {
try {
    bool val = false;
    while (cin) {
        Token t = ts.get();
        if (t.kind == 'q')
            break; // 'q' for quit
        if (t.kind == ';')     // ';' for "print now"
            cout << "=" << val << '\n';
        else
            ts.putback(t);
        val = expression();
    }
}
catch (exception& e) {
    cerr << "error: " << e.what() << '\n';
        return 1;
}
catch (...) {
    cerr << "Oops: unknown exception!\n"; 
        return 2;
}
    return -1;
}

//------------------------------------------------------------------------------
