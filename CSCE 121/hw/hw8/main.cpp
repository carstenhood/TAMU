//Carsten Hood; CSCE 121

#include "std_lib_facilities_3.h"

//------------------------------------------------------------------------------

struct God {
    string name;
    string mythology;
    string vehicle;
    string weapon;
    
    God(string n, string m, string v, string w)
    : name(n), mythology(m), vehicle(v), weapon(w) { }
};

//------------------------------------------------------------------------------

class Link {
public:
    God god;

    Link(God g, Link* p = 0, Link* s = 0)
    : god(g), prev(p), succ(s) { }
    
    Link* get_first();   // return the first link in a list
    Link* insert(Link* n);   // insert n before this object
    Link* add_ordered(Link* n);
    Link* add(Link* n);      // insert n after this object
    Link* erase();           // remove this object from list
    const Link* find(const string& s) const; // find s in list
    
    Link* advance(int n) const;     // move n positions in list
    
    Link* next() const { return succ; }
    Link* previous() const { return prev; }
private:
    Link* prev;
    Link* succ;
};

//------------------------------------------------------------------------------

Link* Link::get_first() // return the first link in a list
{
    Link *p = this;
    while (p->prev) p=p->prev;
    
    return p;
}

//------------------------------------------------------------------------------

Link* Link::insert(Link* n)   // insert n before this object; return n
{
    if (n==0) return this;
    if (this==0) return n;
    n->succ = this;           // this object comes after n
    if (prev) prev->succ = n;
    n->prev = prev;           // this object's predecessor becomes n's predecessor
    prev = n;                 // n becomes this object's predecessor
    
    return n;
}

//------------------------------------------------------------------------------

Link* Link::add_ordered(Link* n)
{
    if (n==0) return this;
    if (this==0) return n;
    
    Link *p = get_first(); //start at first link

    while(p) {
        if (n->god.name.compare(p->god.name) < 0) {
            return p->insert(n);
        }
        if (p->succ) {
            p = p->succ;
        } else { //the new name comes after the last; add it to the end
            n->prev = p;
            p->succ = n;
            n->succ = 0;
            return n;
        }
    }
    return this;
}

//------------------------------------------------------------------------------

Link* Link::erase()          // remove this object from the list; return this's successor
{
    if (this==0) return 0;
    if (succ) succ->prev = prev;
    if (prev) prev->succ = succ;
    
    Link *temp = succ;
    succ = 0;
    prev = 0;
    
    return temp;
}

//------------------------------------------------------------------------------

void print_all(Link* p)
{
    p = p->get_first();
    
    while (p) {
        cout << "\nName: " << p->god.name
        << "\nMythology: " << p->god.mythology
        << "\nVehicle: " << p->god.vehicle
        << "\nWeapon: " << p->god.weapon << "\n";
        
        p = p->next();
    }
    cout << "\n";
}

//------------------------------------------------------------------------------

int main()
{
    cout << "\n(1) Adding gods to 'all_gods' using 'add_ordered'...\n";
    
    Link *all_gods = new Link(God("Thor", "Norse", "", "Hammer"));
    all_gods = all_gods->add_ordered(new Link(God("Zeus", "Greek", "", "Thunderbolt")));
    all_gods = all_gods->add_ordered(new Link(God("Thoth", "Egyptian", "", "")));
    all_gods = all_gods->add_ordered(new Link(God("Loki", "Norse", "", "")));
    all_gods = all_gods->add_ordered(new Link(God("Athena", "Greek", "", "")));
    all_gods = all_gods->add_ordered(new Link(God("Isis", "Egyptian", "", "")));
    all_gods = all_gods->add_ordered(new Link(God("Freia", "Norse", "", "")));
    all_gods = all_gods->add_ordered(new Link(God("Horus", "Egyptian", "Chariot", "Spear")));
    all_gods = all_gods->add_ordered(new Link(God("Ares", "Greek", "Chariot", "Spear")));
    all_gods = all_gods->add_ordered(new Link(God("Poseidon", "Greek", "", "Trident")));
    all_gods = all_gods->add_ordered(new Link(God("Ra", "Egyptian", "Chariot", "Staff")));
    all_gods = all_gods->add_ordered(new Link(God("Odin", "Norse", "Flying Horse", "Spear")));
    
    cout << "\n(2) Printing out all gods using 'print_all'...\n";
    
    cout << "\nALL GODS:";
    print_all(all_gods);
    
    Link *norse_gods = 0;
    Link *greek_gods = 0;
    Link *egyptian_gods = 0;
    
    Link *g = 0; //temporary pointer for sorting
    all_gods = all_gods->get_first(); //go to first link
    
    cout << "\n(3) Sorting gods by pantheon into 3 lists...\n";
    while (all_gods) {
        g = all_gods;
        all_gods = all_gods->erase();
        if (g->god.mythology == "Greek") {
            greek_gods = greek_gods->add_ordered(g);
        } else if (g->god.mythology == "Norse") {
            norse_gods = norse_gods->add_ordered(g);
        } else if (g->god.mythology == "Egyptian") {
            egyptian_gods = egyptian_gods->add_ordered(g);
        }
    }
    
    cout << "\n(4) Printing out the sorted gods...\n";
    
    cout << "\nGREEK GODS:";
    print_all(greek_gods);
    
    cout << "\nNORSE GODS:";
    print_all(norse_gods);
    
    cout << "\nEGYPTIAN GODS:";
    print_all(egyptian_gods);
    
    cout << "\nProgram terminating.\n";
    
    return 0;
}

//------------------------------------------------------------------------------
