#include "Sort.h"
#include "Comparator.h"

#include <iostream>
#include <cstdlib>
#include <vector>

#include <ctime> //timing
#include <iomanip>   //for set precision

using namespace std;

//generates best case for quick sort using last element as pivot
//moves the median of each subsequence to the last element
template<template<typename> class Seq>
void median_last(Seq<int> &s, int x, int y) {
    
    //stop algorithm when subsequence is too small
    if (y-x < 3)
        return;
    
    //calculate middle element
    int z = (x+y)/2;
    int median = s.elemAtRank(z);
    
    //move median element to end
    s.removeAtRank(z);
    s.insertLast(median);
    
    //recurse on subsequences
    median_last(s, x, z-1);
    median_last(s, z, y-1);
}

//populates an input sequence given a size and order of elements
template<template<typename> class Seq>
void generate_sequence(string order, int size, Seq<int> &sequence) {

    if (order == "ordered") {
        for (int i = 0; i<size; ++i) {
            sequence.insertLast(i);
        }
    } else if (order == "reverse") {
        for (int i = size-1; i>=0; --i) {
            sequence.insertLast(i);
        }
    } else if (order == "same") {
        for (int i = 0; i<size; ++i) {
            sequence.insertLast(0);
        }
    } else if (order == "median-last") {
        for (int i = 0; i<size; ++i) {
            sequence.insertLast(i);
        }
        median_last(sequence, 0, size-1);
    } else { //random
        for (int i = 0; i<size; ++i) {
            sequence.insertLast(rand()%size);
        }
    }
}

//averages the running time over _num_itr iterations and returns the average time.
template<template<typename, typename> class Sort, template<typename> class Seq>
double run_test(Sort<int, Comp> s, Seq<int> &original_seq, int _num_itr) {
    
    //save original sequence order
    Seq<int> seq = original_seq;
    
    //start timing at new tick
    clock_t k = clock();
    clock_t start;
    do start = clock();
    while (start == k);
    
    //sort sequence _num_itr times
    for (int itr=0; itr<_num_itr; ++itr) {
        s.sort(seq);
        seq = original_seq; //reset to original sequence
    }
    
    //end timing
    clock_t end = clock();
    
    //show size and iterations
    //cout << seq.size() << ", ";
    //cout << _num_itr << ", ";
    
    double elapsed_time = double(end - start) / double(CLOCKS_PER_SEC);
    //cout << elapsed_time << ", "; //shows total time
    return elapsed_time / double(_num_itr);
}

//runs a test over a range of sizes
template<template<typename, typename> class Sort, template<typename> class Seq>
void initiate_test(Sort<int, Comp> s, Seq<int> seq2, string order, bool slower) {
    size_t lower = 2;
    size_t upper = 16384;
    if (slower)
        upper/=2;
    
    int size = lower;
    while(size <= upper) {
        int iterations = int(4*upper/size);
        
        //initialize and populate sequence
        Seq<int> seq;
        generate_sequence(order, size, seq);
        
        //test sorting algorithm on sequence
        cout << run_test(s, seq, iterations) << endl;
        
        size *=2;
    }
}

//choose input senquences and initiate tests for each sort
template<template<typename, typename> class Sort, template<typename> class Seq>
bool TestSort(string name) {
    
    Sort<int, Comp> s;
    Seq<int> seq;
    
    bool slower = false; //must avoid biggest sizes when testing slow sorts
    vector<string> orders; //dictates order of input sequence
    vector<string> cases {"best", "worst", "average"};
    
    if (name == "InsertionSort") {
        orders.push_back("ordered"); //best
        orders.push_back("reverse"); //worst
        orders.push_back("random"); //average
        slower = true;
    } else if (name == "SelectionSort") {
        orders.push_back("ordered"); //best
        orders.push_back("reverse"); //worst
        orders.push_back("random"); //average
        slower = true;
    } else if (name == "HeapSort") {
        orders.push_back("ordered"); //average
        orders.push_back("same"); //average
        orders.push_back("random"); //average
    } else if (name == "MergeSort") {
        orders.push_back("ordered"); //average
        orders.push_back("same"); //average
        orders.push_back("random"); //average
    } else if (name == "QuickSortLast") {
        orders.push_back("median-last"); //best
        orders.push_back("ordered"); //worst
        orders.push_back("random"); //average
    } else if (name == "QuickSortMedium") {
        orders.push_back("median-last"); //best
        orders.push_back("ordered"); //worst
        orders.push_back("random"); //average
    } else if (name == "QuickSortRandom") {
        orders.push_back("median-last"); //best
        orders.push_back("ordered"); //worst
        orders.push_back("random"); //average
    } else if (name == "RadixSort") {
        orders.push_back("ordered"); //average
        orders.push_back("same"); //average
        orders.push_back("random"); //average
    }
        
    for (int i=0; i<orders.size(); ++i) {
        cout << '\n' << name << endl;
        cout << cases[i] << " case: ";
        cout << orders[i] << " sequence " << endl;
        cout << "Size | Iter | Avg Time" << endl;
        initiate_test(s, seq, orders[i], slower);
    }
    
    return true;
}

int main() {
    
    bool passed = true;
    
    if (!TestSort<InsertionSort, NodeSequence>("InsertionSort"))
        passed = false;
    if (!TestSort<SelectionSort, NodeSequence>("SelectionSort"))
        passed = false;
    if (!TestSort<HeapSort, NodeSequence>("HeapSort"))
        passed = false;
    if (!TestSort<MergeSort, NodeSequence>("MergeSort"))
        passed = false;
    if (!TestSort<QuickSortLast, VectorSequence>("QuickSortLast"))
        passed = false;
    if (!TestSort<QuickSortMedian, VectorSequence>("QuickSortMedium"))
        passed = false;
    if (!TestSort<QuickSortRandom, VectorSequence>("QuickSortRandom"))
        passed = false;
    if (!TestSort<RadixSort, VectorSequence>("RadixSort"))
        passed = false;
    
    if (passed)
        cout << "All tests passed." << endl;
    else
        cout << "Tests failed." << endl;
    
    return 0;
}
