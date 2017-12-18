//
//  main.cpp
//  hw2pr3
//
//  Created by Carsten Hood on 9/4/13.
//  Copyright (c) 2013 Carsten Hood. All rights reserved.
//

#include "std_lib_facilities_3.h"

vector<double> checkRoot(double a, double b, double c) {
    vector<double> roots;
    double root_term = b*b - 4*a*c;
    if (root_term<0) {
        //throw no_roots;
        cout << "No real roots. Program terminating.";
    } else {
        double root = sqrt(root_term);
        roots.push_back(root);
        roots.push_back(-root);
        for (int i=0; i<roots.size(); i++) {
            cout << "Root " << i+1 << " = " << roots[i] << "\n";
        }
    }
    return roots;
}

int main() {
    vector<char> var_chars;
    var_chars.push_back('a');
    var_chars.push_back('b');
    var_chars.push_back('c');
    vector<double> vars;
    for (int i=0; i<var_chars.size(); i++) {
        cout << var_chars[i] << " = ";
        double var;
        try {
            cin >> var;
            if (!cin) {
                throw false;
            }
            else if (i==0 && var==0) {
                throw 1;
            }
        }
        catch (int i) {
            cerr << "'a' cannot equal 0. Program terminating.";
            return -1;
        }
        catch (...) {
            cout << "Invalid entry. Program Terminating.";
            return -1;
        }
        vars.push_back(var);
    }
    double a = vars[0];
    double b = vars[1];
    double c = vars[2];
    vector<double> roots = checkRoot(a, b, c);    
    for (int i=0; i<roots.size(); i++) {
        double x = (-b + roots[i])/(2 * a);
        cout << "x" << i+1 << " = " << x << "\n";
    }
    return 0;
}

