//
//  hw1pr1.cpp
//  hw1pr1
//
//  Created by Carsten Hood on 9/2/13.
//  Copyright (c) 2013 Carsten Hood. All rights reserved.
//

//#include <iostream>
#include "std_lib_facilities_3.h"

int main(int argc, const char * argv[])
{
    string operation;
    double operand1, operand2;
    double output = 0;
    cout << "Operation: ";
    cin >> operation;
    cout << "First operand: ";
    cin >> operand1;
    cout << "Second operand: ";
    cin >> operand2;
    
    if (operation == "+")
        output = operand1 + operand2;
    else if (operation == "-")
        output = operand1 - operand2;
    else if (operation == "*")
        output = operand1 * operand2;
    else if (operation == "/")
        output = operand1 / operand2;
    else {
        cout << "Invalid Operator";
        return -1;
    }
    cout << "Output: " << output;
    return 0;
}