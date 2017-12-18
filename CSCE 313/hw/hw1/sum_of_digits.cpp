//
//  sum_of_digits.cpp
//  homework 1 problem 10
//  Created by Carsten Hood on 7/19/16.
//

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <string.h>
#include <signal.h>

using namespace std;

//static int position;
static int num;
static int sum;

void add_digit() {
    if (num != 0) {
        pid_t p = 0;
        p = fork();
        if (p == 0) {   // child
            sum += num % 10;
            num /= 10;
            add_digit();
        } else {
            exit(1);
        }
    }
}

int main(int argc, const char * argv[]) {
    if (!argv[1]) {
        cout << "Terminating – requires integer argument.\n";
        return 0;
    }
    num = atoi(argv[1]);
    sum = 0;
    add_digit();
    cout << sum << endl;
    
    return 0;
}

