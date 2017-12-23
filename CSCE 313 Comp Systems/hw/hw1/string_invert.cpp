//
//  string_invert.cpp
//  homework 1 problem 9
//  Created by Carsten Hood on 7/19/16.
//

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <string.h>

using namespace std;

static int position;
static const char * chars;

void print_char() {
    if (position > 0) {
        --position;
        pid_t p = 0;
        p = fork();
        if (p == 0) {
            print_char();
        } else {
            cout << chars[position];
        }
    }
}

int main(int argc, const char * argv[]) {
    chars = argv[1];
    if (!chars) {
        cout << "Require string argument.";
        return 0;
    }
    
    position = (int)(strlen(chars));
    print_char();
    
    return 0;
}
