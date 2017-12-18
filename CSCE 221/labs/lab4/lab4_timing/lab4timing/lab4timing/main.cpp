//Carsten Hood
//CSCE 221
//Timing

#include <iostream>
#include <iomanip>   //for setprecision
#include <vector>
#include <algorithm> //for generate
#include <numeric>   //for accumulate
#include <ctime>     //for clock
#include <sstream>

using namespace std;

// Tests the sort function on vectors of varying size
// Averages the running time over _num_itr iterations and returns the average time.
double run_test(size_t _size, int _num_itr);

int main(int argc, char** argv) {
    size_t lower = 2;
    size_t upper = 200000000;
    
    if (argc >= 3) {
        lower = atoi(argv[1]);
        upper = atoi(argv[2]);
    }
    
    cout << setprecision(10);
    cout << "Size, Iter, Time(sec)" << endl;
    
    size_t size = lower;
    while(size <= upper) {
        cout << size << ", ";
        cout << run_test(size, 10+int(10000000/size)) << endl;
        size *=2;
    }
    
    return 0;
}

double run_test(size_t _size, int _num_itr) {
    vector<double> random_vector(_size);
    
    clock_t k=clock();
    clock_t start;
    do start = clock();  //begin at new tick
    while (start == k);
    
    int iter;
    for(iter=0; iter<_num_itr; ++iter) {
        generate(random_vector.begin(), random_vector.end(), rand);
        sort(random_vector.begin(), random_vector.end());
    }
    
    clock_t end = clock();
    
    cout << iter << ", "; //show num of iterations
    
    double elapsed_time = double(end - start) / double(CLOCKS_PER_SEC);
    return elapsed_time / double(_num_itr);
    
}
