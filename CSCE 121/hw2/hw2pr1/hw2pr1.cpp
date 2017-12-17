//
//  main.cpp
//  hw2pr1
//
//  Created by Carsten Hood on 9/4/13.
//  Copyright (c) 2013 Carsten Hood. All rights reserved.
//

//#include <iostream>
#include "std_lib_facilities_3.h"

int main(int argc, const char * argv[]) {
    cout << "Rock Paper Scissors!\nEnter a character to begin: ";
    char random_char; //user input randomizes machine's moves
    cin >> random_char;
    int random_int = (int)random_char;
    if (random_int < 0) {
        random_int *= -1;
    }
    
    vector<string> moves;
    moves.push_back("Rock");
    moves.push_back("Paper");
    moves.push_back("Scissors");
    int machine_index = random_int % 3;
    string machine_move = moves[machine_index];
    
    char user_move;
    int user_index;
    cout << "Make your move.\nEnter \"r\", \"p\", or \"s\": ";
    cin >> user_move;    
    switch (user_move) {
        case 'r':user_index=0;
            break;
        case 'p':user_index=1;
            break;
        case 's':user_index=2;
            break;
        default: user_index=-1;
            break;
    }
    if (user_index==-1) { //invalid entry
        cout << "Invalid Entry. You forfeit the game.";
        return -1;
    }
    
    cout << "\nYour move: " << moves[user_index] << "\nMachine move: " << moves[machine_index] << "\n";
    string result = "I don't even know.";
    if (user_index == machine_index) {
        result = "Tie game!";
    }
    else if (user_index+1 == machine_index || (user_index==2 && machine_index==0)) {
        result = moves[machine_index] + " beats " + moves[user_index] + ".\nMachine wins.";
    }
    else if (user_index == machine_index+1 || (user_index==0 && machine_index==2)) {
        result = moves[user_index] + " beats " + moves[machine_index] + ".\nYou win!";
    }
    cout << result;
    
    return 0;
}

