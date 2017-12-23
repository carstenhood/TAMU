//
//  main.cpp
//  hw3pr2
//
//  Created by Carsten Hood on 9/13/13.
//  Copyright (c) 2013 Carsten Hood. All rights reserved.
//

#include "std_lib_facilities_3.h"

void fibonacci(int x, int y, int n, vector<int> &sequence) {
    sequence.push_back(x);
    if (n>1) {
        sequence.push_back(y);
        for (int k=2; k<n; ++k) {
            int next = sequence[k-2]+sequence[k-1];
            sequence.push_back(next);
        }
    }
}

void print(string title, vector<int> &sequence) {
    cout << title;
    string punctation = ", ";
    for (int k=0; k<sequence.size(); k++) {
        if (k == sequence.size()-1) {
            punctation = ";";
        }
        cout << sequence[k] << punctation;
    }
}

int main() {
    try {
        int x=0;
        int y=0;
        int n=0;
        cout << "Enter 1st integer: ";
        cin >> x; if(!cin) throw 1;
        cout << "Enter 2nd integer: ";
        cin >> y; if(!cin) throw 1;
        cout << "Enter length: ";
        cin >> n; if(!cin) throw 1;
        if (x<0 || y<0 || n<=0) {
            throw false;
        }
        
        vector<int> sequence;
        fibonacci(x, y, n, sequence);
        print("Your sequence: ", sequence);
    } catch (...) {
        cout << "Invalid input. Program terminating.";
    }

    return 0;
}
