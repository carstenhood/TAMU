//  Carsten Hood
//  CSCE 121
//
//  HW 10

#include "std_lib_facilities_3.h"

string read_file_with_name(string file_name)
{
    ifstream in;
    in.open(file_name);
    
    stringstream dict_stream;
    while (true) {
        string next_word;
        getline(in, next_word);
        if (next_word == "") break;
        dict_stream << next_word << '\n';
    }
    
    return dict_stream.str();
}

vector<char> gen_letters(int num)
{
    cout << "Enter any character to begin: "; // have the user enter a line
    string input;
    getline(cin, input);
    stringstream char_stream(input);
    char schar; // take the first character in the line the user entered
    char_stream >> schar;
    int sint = (int)schar; // convert the random character into an int
    
    vector<char> letters;
    for (int i=0; i<num+sint; ++i) { // use the int to randomize letters
        int ri = rand()%26;
        char rc = (char)(ri+65);
        if (i>=sint) letters.push_back(rc);
    }
    
    return letters;
}

int getpoints(string input, vector<string> &entries, string &dict, vector<char> letters)
{
    // check word is over 3 letters
    if (input.length() < 3) {
        cout << "Word must be at least 3 letters. ";
        return 0;
    }
    
    // make all letters lowercase
    stringstream lc_stream;
    for (int i=0; i<input.length(); ++i) {
        char input_char = input.at(i);
        if ((int)input_char < 97)
            input_char = (char)((int)input_char+32);
        lc_stream << input_char;
    }
    input = lc_stream.str();
    
    // check word uses provided letters
    int matches=0;
    for (int i=0; i<input.length(); ++i) {
        for (int k=0; k<letters.size(); ++k) {
            if (input.at(i) == (char)((int)letters[k]+32)) {
                letters.erase(letters.begin()+k);
                matches++;
            }
        }
    }
    if (matches<input.length()) {
        cout << "Word must use the given letters. ";
        return 0;
    }
    
    // check word is in dictionary
    bool match = false;
    stringstream dict_stream;
    dict_stream << dict;
    string dict_word = " ";
    while (dict_word != "") {
        getline(dict_stream, dict_word);
        if (dict_word == input) {
            match = true;
            break;
        }
    }
    if (!match) {
        cout << "Word not in dictionary. ";
        return 0;
    }
    
    // check word hasn't already been used
    for (int i=0; i<entries.size(); ++i) {
        if (entries[i]==input) {
            cout << "Word already entered. ";
            return 0;
        }
    }
    
    cout << "Word found! ";
    entries.push_back(input);
    return 1;
}

int main()
{
    cout << "Welcome to word scramble. Assemble words for points.\n";
    cout << "Loading dictionary...\n";
    string dict = read_file_with_name("dictionary.txt");
    vector<char> letters = gen_letters(9);
    
    int score=0;
    string input = " ";
    vector<string> entries;
    while (input != "")
    {
        cout << "\nLetters:";
        for(char c : letters)
            cout << " " << c;
        
        cout << "\nEnter a word (enter nothing to quit): ";
        getline(cin, input);
        
        int points = getpoints(input, entries, dict, letters);
        score+=points;
        string pt_str = " points";
        if (points==1) pt_str = " point";
        cout << points << pt_str << " awarded. Score: " << score << '\n';
    }
    
    cout << "\nFinal score: " << score;
    cout << "\n\nProgram terminating.\n";
    
    return 0;
}

