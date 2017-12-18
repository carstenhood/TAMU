/*
 * This program calculates the processing time of certain
 * functions in array_stack and list_stack. The program uses
 * the function clock(), the clock_t type, and the constant
 * CLOCKS_PER_SEC defined in the C++ standard library "ctime".
 */

#include "array_stack.h"
#include "list_stack.h"

#include <ctime>
#include <iomanip>   //for setprecision

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

using namespace std;

//averages the running time over _num_itr iterations and returns the average time.
template <typename stack_type>
double run_test(size_t _size, int _num_itr, string which_test);

//call run_test from lower to upper size and print results
template <typename stack_type>
void start_test(string which_test, size_t lower, size_t upper);

//---------------------------------------------------------------------

int main() {
    size_t lower = 2;
    size_t upper = 1048576;
    
    //determines which tests are performed
    vector<string> test_types {"push", "size", "my_size"};
    
    cout << setprecision(10);

    for (int i=0; i<test_types.size(); ++i) {
        //display which test is being performed
        cout << "testing " << test_types[i] << "() function:" << endl;
        
        cout << "\narray_stack: " << endl;
        cout << "Size, Iter, Time(sec)" << endl;
        start_test<array_stack<double>>(test_types[i], lower, upper);
        
        cout << "\nlist_stack:" << endl;
        cout << "Size, Iter, Time(sec)" << endl;
        start_test<list_stack<double>>(test_types[i], lower, upper);
        
        cout << endl;
    }
    
    return 0;
}

//---------------------------------------------------------------------

//call run_test from lower to upper size and print results
template <typename stack_type>
void start_test(string which_test, size_t lower, size_t upper) {
    size_t size = lower;
    while(size <= upper) {
        int iterations = int(upper*2/size);
        
        //speed of the size() function requires more iterations
        if (which_test == "size")
            iterations*=128;
        
        cout << run_test<stack_type>(size, 10+iterations, which_test) << endl;
        size *=2;
    }
}

//averages the running time over _num_itr iterations and returns the average time.
template <typename stack_type>
double run_test(size_t _size, int _num_itr, string which_test) {
    vector<double> random_vector(_size);
    generate(random_vector.begin(), random_vector.end(), rand);
    
    //initialize stack for use in size and my_size functions prior to timing
    stack_type s;
    for (int i=0; i<random_vector.size(); ++i)
        s.push(random_vector[i]);

    //start timing at new tick
    clock_t k=clock();
    clock_t start;
    do start = clock();
    while (start == k);
    
    //run appropriate function _num_itr times
    if (which_test == "push") {
        for (int itr=0; itr<_num_itr; ++itr) {
            stack_type s;
            for (int i=0; i<random_vector.size(); ++i)
                s.push(random_vector[i]);
        }
        cout << random_vector.size() << ", ";
    } else if (which_test == "size") {
        cout << s.size() << ", "; //print counts as first iteration
        for (int itr=1; itr<_num_itr; ++itr)
            s.size();
    } else if (which_test == "my_size") {
        cout << s.my_size() << ", "; //print counts as first iteration
        for (int itr=1; itr<_num_itr; ++itr)
            s.my_size();
    }
    
    clock_t end = clock();      //end timing
    
    cout << _num_itr << ", ";   //show num of iterations
    
    double elapsed_time = double(end - start) / double(CLOCKS_PER_SEC);
    return elapsed_time / double(_num_itr);
}

