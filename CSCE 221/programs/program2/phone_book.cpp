/* phone_book.cpp
 *
 * Implementation of phone_book.h
 */

#include "phone_book.h"
#include <iostream> //remove
#include <fstream>
#include <sstream>
using namespace std;

phone_book::phone_book(const string& _filename) : filename(_filename) {
    read();
}

//return if the contact exists
bool phone_book::is_contact(const string& _name) const {
    return contacts.find(_name) != contacts.end();
}

//return the number of the appropriate contact
string phone_book::get_number(const string& _name) {
    return contacts[_name].number;
}

//add a new entry into the phone book
void phone_book::add_contact(const string& _name, const string& _number) {
    contact_info ci; //create new contact with given info
    ci.name = _name;
    ci.number = _number;
    contacts.insert(pair<string, contact_info>(ci.name, ci));
}

//save the data to filename
void phone_book::save() {
    fstream fs(filename);
    
    //iterate through contacts and write info to file
    map< string, contact_info>::iterator iter = contacts.begin();
    while (true) {
        fs << iter->second.number << " " << iter->first;
        if (++iter == contacts.end())
            break;
        fs << '\n';
    }
}

//read data from filename
void phone_book::read() {
    fstream fs(filename);
    string input;

    while (true) {
        getline(fs, input); //get line for the next contact
        stringstream ss(input);
        
        contact_info ci;

        //read in the number
        ss >> ci.number;
        
        //read in the name
        getline(ss, ci.name);
        ci.name.erase(0, 1); //remove white space at beginning
        
        contacts.insert(pair<string, contact_info>(ci.name, ci));
        
        if (!fs) //check if file is empty
            break;
    }
}
