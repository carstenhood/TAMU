// Carsten Hood
// CSCE 315-100
// Due: June 6, 2016
// project1.cpp
// (styled according to Dr. Stroustrup's "PPP Style Guide")

#include <iostream>
#include <cmath>
#include <vector> // for test values

using namespace std;

// declaration of custom test function
void __attribute__((weak)) check(double alt_sqrt(double));

void __attribute__((weak)) check(double alt_sqrt(double))
    // custom test function
{
    // initialize vector of test values
    vector<double> values;
    values.push_back(0.0);
    values.push_back(1.0);
    values.push_back(2.0);
    values.push_back(9.0);
    values.push_back(123456789.0);
    values.push_back(0.1234567890);
    
    // attempt each test value and output successfulness
    for (vector<double>::size_type i=0; i!=values.size(); ++i) {
        double input = values[i];           // test value
        double altroot = alt_sqrt(input);   // custom square root
        double root = sqrt(input);          // built-in square root
        
        // output whether custom root is within margin of actual root
        if (altroot == root) cout << "PASS";
        else cout << "FAIL";
        cout << " | input: " << input << " | my_sqrt: " << altroot
                             << " | sqrt: " << root << ";\n";
    }

    return;
}

bool isRoot(double root, double x)
    // checks if a root attempt is within margin of x
{
    // initialize margin of error constants
    const double pos_margin = nextafter(+0.0,+1.0); // smallest positive double
    const double neg_margin = nextafter(-0.0,-1.0); // greatest negative double
    
    // difference between x and the square of the root attempt
    const double diff = root*root - x;
    
    // return whether difference is in acceptable range
    return (diff >= neg_margin && diff <= pos_margin);
}

double my_sqrt(double x)
    // use bisection method to return approximate square root of x ( via --->
    // http://www.mathpath.org/Algor/squareroot/algor.square.root.binary.htm )
{
    double root = 0.0;  // initial root attempt is 0.0
    double left = 0.0;  // initialize left (aka lower) bound to 0.0
    double right = x;   // initialize right (aka upper) bound to x
    if (right<1.0) right = 1.0; // upper bound must be at least 1.0
        
    int counter = pow(10,6);    // maximum number of bisections

    // bisect interval until root is satisfactory or counter expires
    while (!isRoot(root,x) && counter>0) {
        if (root*root>x)
            right = root;
        else
            left = root;
        root = 0.5*(left+right); // set next attempt to bisection of interval
        --counter;
    }
    
    return root;
}

int main()
    // do not change this function
{
    check(my_sqrt);
    return 0;
}
