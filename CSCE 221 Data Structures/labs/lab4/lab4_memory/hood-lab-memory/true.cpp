// Carsten Hood
// CSCE 221H
// Determines if all bools in array are true using iteration

#include <iostream>

int main()
{
    const int n = 10000;
    
    bool* a = new bool[n];
    
    for (int i = 0; i < n; ++i)
        a[i] = true;
    
    bool x = true;
    for (int i = 0; i < n; ++i)
        if (!a[i]) {
            x = false;
            break;
        }
    
    std::cout << std::boolalpha << x << std::endl;
    
    return 0;
}
