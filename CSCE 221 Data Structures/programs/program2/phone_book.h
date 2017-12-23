/* phone_book
 *
 * Provides a simple interface over a balenced binary search tree for efficient
 * management of a phonebook. The interface provides simple functions to
 * read/save from/to file, examine if a contact exist, access contacts, and add
 * new contacts.
 */

#ifndef PHONEBOOK_H_
#define PHONEBOOK_H_

#include <string>
#include <map>
using namespace std;

class phone_book {
  public:
    struct contact_info {
        string name;
        string number;
    };
    
    phone_book(const string& _filename);
    
    //return if the contact exists
    bool is_contact(const string& _name) const;
    
    //return the number of the appropriate contact
    string get_number(const string& _name);
    
    //add a new entry into the phone book
    void add_contact(const string& _name, const string& _number);
    
    //save the data to filename
    void save();
    
  private:
    //read data from filename
    void read();
    
    string filename;
    map<string, contact_info> contacts;
};

#endif
