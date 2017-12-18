//
//  main.cpp
//  hw2pr2
//
//  Created by Carsten Hood on 9/4/13.
//  Copyright (c) 2013 Carsten Hood. All rights reserved.
//

#include "std_lib_facilities_3.h"

int main(int argc, const char * argv[]) {
    cout << "Enter name and score pairs:\n";
    vector<string> names;
    vector<int> scores;
    string name;
    int score;
    while (cin >> name) {
        if (cin >> score) {
            for (int i=0; i<names.size(); i++) {
                if ((names[i] == name)) {
                    cout << "Error. Repeated Entry.";
                    return -1;
                }
            }
            names.push_back(name);
            scores.push_back(score);
        }
        else
            break;
    }    
    cout << "\nEntries:\n";
    for (int i=0; i<names.size(); i++) {
        cout << "(" <<names[i] << "," << scores[i] << ")\n";
    }

    return 0;
}

