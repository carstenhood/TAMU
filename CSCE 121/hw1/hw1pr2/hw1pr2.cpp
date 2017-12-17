//
//  main.cpp
//  hw1pr2
//
//  Created by Carsten Hood on 9/2/13.
//  Copyright (c) 2013 Carsten Hood. All rights reserved.
//

//#include <iostream>
#include "std_lib_facilities_3.h"

int main(int argc, const char * argv[])
{
    int pennies, nickels, dimes, quarters, half_dollars = 0;

    cout << "How many pennies? ";
    cin >> pennies;
    cout << "How many nickels? ";
    cin >> nickels;
    cout << "How many dimes? ";
    cin >> dimes;
    cout << "How many quarters? ";
    cin >> quarters;
    cout << "How many half dollars? ";
    cin >> half_dollars;
    
    string singular = ".";
    string plural = "s.";
    string ending = "ies."; //init with special "pennies" ending
    if (pennies == 1) ending = "y.";
    cout << "\nYou have " << pennies << " penn" << ending;
    if (nickels == 1) ending = singular;
    else ending = plural;
    cout << "\nYou have " << nickels << " nickel" << ending;
    if (dimes == 1) ending = singular;
    else ending = plural;
    cout << "\nYou have " << dimes << " dime" << ending;
    if (quarters == 1) ending = singular;
    else ending = plural;
    cout << "\nYou have " << quarters << " quarter" << ending;
    if (half_dollars == 1) ending = singular;
    else ending = plural;
    cout << "\nYou have " << half_dollars << " half dollar" << ending;
    
    int total = pennies + nickels*5 + dimes*10 + quarters*25 + half_dollars*50;
    int dollars = total/100;
    int cents = total%100;
    cout << "\nYour total is $" << dollars << "." << cents << ".";
    
    return 0;
}

