//Carsten Hood
//CSCE 121

#include "std_lib_facilities_3.h"

template<class Iter1, class Iter2> Iter2 my_copy(Iter1 f1, Iter1 e1, Iter2 f2)
{
    while (f1 != e1) {
        *f2 = *f1;
        ++f2;
        ++f1;
    }
    return f2;
}

int main()
{
    //1-3 initializing
    int a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    vector<int> v (a, a+sizeof(a)/sizeof(a[0]));
    list<int> l (v.begin(), v.end());
    
    //4 initializing by copying
    int a2[sizeof(a)/sizeof(a[0])];
    for (int i=0; i<10; ++i)
        a2[i]=a[i];
    
    vector<int> v2 = v;
    
    list<int> l2 = l;
    
    //5 incrementing
    for (int i=0; i<sizeof(a); ++i)
        a[i]+=2;
    
    for (int i=0; i<v.size(); ++i)
        v[i]+=3;
    
    auto p = l.begin();
    while (p != l.end()) {
        *p+=5;
        ++p;
    }
    
    //6 //method above
    
    //7 copying
    my_copy(a, a+sizeof(a)/sizeof(a[0]), v.begin());
    my_copy(l.begin(), l.end(), a);
    
    //8 finding index of values
    vector<int>::iterator loc3 = find(v.begin(), v.end(), 3);
    if (loc3!=v.end())
        cout << "Index of 3 in vector: " << (loc3-v.begin()) << "\n";
    else
        cout << "3 not found in vector.\n";
    
    list<int>::iterator loc27 = find(l.begin(), l.end(), 27);
    if (loc27!=l.end()) {
        list<int>::iterator p = l.begin();
        int index27=0;
        while (p != loc27) {
            ++index27;
            ++p;
        }
        cout << "Index of 27 in list: " << index27 << "\n";
    } else
        cout << "27 not found in list.\n";
    
    return 1;
}