//Carsten Hood; CSCE 121

#include "std_lib_facilities_3.h"

class No_entries { };
class Bad_data { };

template<class T> class Vector_ref { vector<T*> v;
    vector<T*> owned;
public :
    Vector_ref() {}
    Vector_ref(T* a, T* b = 0, T* c = 0, T* d = 0);
    
    ~Vector_ref() { for (int i=0; i<owned.size(); ++i)
        //delete owned[i]; //problematic line, causes run-time errors
        continue; //allows program to run; possibly causes memory leak
    }
    
    void push_back(T& s) {v.push_back(&s); }
    void push_back(T* p) { v.push_back(p); owned.push_back(p); }
    
    T& operator[](int i) { return *v[i]; }
    
    const T& operator[](int i) const { return *v[i]; }
                                                 
    int size() const { return v.size(); }
};

struct Pub {
    virtual void write(ofstream& out)=0;
};

struct Book : public Pub {
    string author;
    string title;
    string city;
    string publisher;
    string year;
    
    void write(ofstream& out) {
        out << author << ". ";
        out << " _" << title << "_. ";
        out << city << ": " << publisher << ", " << year << '.';
        out << "\n\n";
    }
    
    Book(ifstream& in) {
        getline(in, author); if(!in) throw Bad_data();
        getline(in, title); if(!in) throw Bad_data();
        getline(in, city); if(!in) throw Bad_data();
        getline(in, publisher); if(!in) throw Bad_data();
        getline(in, year); if(!in) throw Bad_data();
        cout << "Read in a book.\n";
    }
};

struct Encyc : public Pub {
    string title;
    string encyclopedia;
    string year;
    
    void write(ofstream& out) {
        out << '\"' << title << ".\" _";
        out << encyclopedia << "_. ";
        out << year << '.';
        out << "\n\n";
    }
    
    Encyc(ifstream& in) {
        getline(in, title); if(!in) throw Bad_data();
        getline(in, encyclopedia); if(!in) throw Bad_data();
        getline(in, year); if(!in) throw Bad_data();
        cout << "Read in an encyclopedia article.\n";
    }
};

struct Journ : public Pub {
    string author;
    string title;
    string journal;
    string volume;
    string issue;
    string start_page;
    string end_page;
    string year;
    
    void write(ofstream& out) {
        out << author << ". ";
        out << '(' << year << ')';
        out << " \"" << title << ".\"";
        out << " _" << journal << "_ ";
        out << volume << '(' << issue << ')';
        out << ':' << start_page << '-' << end_page << ", ";
        out << year << '.';
        out << "\n\n";
    }
    
    Journ(ifstream& in) {
        getline(in, author);  if(!in) throw Bad_data();
        getline(in, title); if(!in) throw Bad_data();
        getline(in, journal); if(!in) throw Bad_data();
        getline(in, volume); if(!in) throw Bad_data();
        getline(in, issue); if(!in) throw Bad_data();
        getline(in, start_page); if(!in) throw Bad_data();
        getline(in, end_page); if(!in) throw Bad_data();
        getline(in, year); if(!in) throw Bad_data();
        cout << "Read in a journal article.\n";
    }
};

string get_file(bool writing) {
    string input_file = "data.txt";
    string output_file = "out.txt";

    string default_name = input_file;
    if(writing) default_name = output_file;
    
    string file_name = default_name;
    while (true) {
        cout << "\n- Enter nothing to use default file.";
        cout << "\n- Enter \"" << input_file << "\" to use reading file.";
        cout << "\n- Enter \"" << output_file << "\" to use output file.";
        cout << "\nFile name: ";
        
        getline (cin,file_name);
        if (file_name == "")
            file_name = default_name;
    
        cout << "Loading file...";
        ifstream in;
        in.open(file_name);
        if (in.is_open()) {
            cout << "\nFile opened successfully.\n";
            return file_name;
        }

        cout << "\nCould not open file. Try again.\n";
    }
}

Vector_ref<Pub> read_file(ifstream& in) {
    cout << "Reading file...\n";
    string input;
    Vector_ref<Pub> pubs;
    while (true) {
        getline(in, input);
        if(!in) { break; }
        if (input == "" || input == "\n" || input.length()>2) continue;

        char type;
        stringstream type_stream(input);
        type_stream >> type;
        
        switch (type) {
            case 'B': pubs.push_back(new Book(in)); break;
            case 'E': pubs.push_back(new Encyc(in)); break;
            case 'J': pubs.push_back(new Journ(in)); break;
            default: break;
        }
    }
    return pubs;
}

void write(ofstream &out, Vector_ref<Pub> pubs) {
    cout << "Writing to file...\n";
    out << "Bibliography:\n\n";
    for (int i=0; i<pubs.size(); i++) {
        pubs[i].write(out);
    }
}

int main() {
    try {
        //get input file name
        cout << "Welcome! Program functions to convert raw data into a correctly formatted bibliography.\n";
        cout << "\nChoose a raw data file...";
        string file_name = get_file(false);
        
        //read raw data from file
        ifstream in;
        in.open(file_name);
        Vector_ref<Pub> pubs = read_file(in);
        if (pubs.size()<1) throw No_entries();
        
        //get output file name
        cout << "\nChoose a file for your formatted entries...";
        file_name = get_file(true);
        
        //write formatted entries to file
        ofstream out;
        out.open(file_name);
        write(out, pubs);
        
        //end of main code
        cout << "\nBibliography formatted successfully. Program terminating.\n";
    } catch (No_entries) {
        cout << "\nNo raw data detected. File may be empty or already formatted. Program Terminating.\n";
        return 0;
    } catch (Bad_data) {
        cout << "\nError reading file. Program terminating.\n";
        return 0;
    } catch (...) {
        cout << "\nError. Program Terminating.\n";
        return 1;
    }
    
    return 0;
}












