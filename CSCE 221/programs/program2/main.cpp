/* main.cpp
 *
 * Provides simple command line tools to operate over a phone book object input
 * at the command line.
 */

#include <iostream>
#include <cstdlib>

#include "phone_book.h"

int main(int argc, char** argv) {
    //ensure the user only provides one filename
    if(argc != 2) {
        cerr << "Error phone_book takes one argument only." << endl;
        exit(1);
    }
    
    //create a phone_book object with input data
    phone_book contacts(argv[1]);
    
    //provide the user an interface over the phone book
    bool exitting = false;
    while(!exitting) {
        
        cout << endl
        << "*************************" << endl
        << "** Phone Book Options: **" << endl
        << "**                     **" << endl
        << "** 1. Look up contact  **" << endl
        << "** 2. Add contact      **" << endl
        << "** 3. Save phone book  **" << endl
        << "** 4. Quit             **" << endl
        << "*************************" << endl
        << endl << "Enter an option: ";
        
        size_t opt;
        cin >> opt;
        
        //variables for option 2
        string name, number;
        
        switch(opt) {
            case 1:
                cout << endl << "Enter contact name: ";
                getline(cin, name);
                getline(cin, name);
                if(contacts.is_contact(name))
                    cout << name << "'s number is " << contacts.get_number(name) << "." << endl;
                else
                    cerr << "Error: Contact does not exist." << endl;
                break;
                
            case 2:
                cout << endl << "Enter contact name: ";
                getline(cin, name);
                getline(cin, name);
                cout << "Enter contact number (###-####): ";
                cin >> number;
                if(!contacts.is_contact(name)) {
                    cout << "Adding contact (" << name << ", " << number << ")." << endl;
                    contacts.add_contact(name, number);
                }
                else
                    cerr << "Error: Contact already exists." << endl;
                break;
                
            case 3:
                cout << endl << "Saving." << endl;
                contacts.save();
                break;
                
            case 4:
                cout << endl << "Exitting." << endl;
                exitting = true;
                break;
                
            default:
                cerr << "Error: Unknown option." << endl;
        }
    }
}
