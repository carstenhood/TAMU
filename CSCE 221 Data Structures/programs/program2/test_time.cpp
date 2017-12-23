/* This program tests the time usage
 of the binary tree implmentation */

#include "binary_tree.h"

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>

#include <ctime> //timing
#include <iomanip>   //for setprecision

using namespace std;

binary_tree<int> generate_linked_list(int _n){
    binary_tree<int> T(0);
    binary_tree<int>::node* p = T.get_root();
    for(int i=0; i<_n-1; i++){
        binary_tree<int>::node* c = T.insert_right(p, i+1);
        p = c;
    }
    return T;
}

binary_tree<int> generate_complete_tree(int _n){
    binary_tree<int> T(0);
    int I = 0;
    deque<binary_tree<int>::NodePtr> leaves;
    deque<binary_tree<int>::NodePtr> tmpleaves;
    leaves.push_back(T.get_root());
    I++;
    while (I < _n) {
        int size = leaves.size();
        tmpleaves.clear();
        for (int i=0; i<size && I<_n; i++){
            binary_tree<int>::node* l = T.insert_left(leaves[i], I++);
            tmpleaves.push_back(l);
            if (I < _n){
                binary_tree<int>::node* r = T.insert_right(leaves[i], I++);
                tmpleaves.push_back(r);
            }
        }
        leaves.clear();
        leaves = tmpleaves;
    }
    return T;
}

//averages the running time over _num_itr iterations and returns the average time.
double run_test(binary_tree<int> tree, string which_test, int _num_itr) {

    //start timing at new tick
    clock_t k=clock();
    clock_t start;
    do start = clock();
    while (start == k);
    
    //run appropriate function _num_itr times
    if (which_test == "recursive preorder") {
        for (int itr=0; itr<_num_itr; ++itr) {
            binary_tree<int>::NodeDeque pre;
            tree.preorder(pre, tree.get_root());
        }
    } else if (which_test == "recursive inorder") {
        for (int itr=0; itr<_num_itr; ++itr) {
            binary_tree<int>::NodeDeque pre;
            tree.inorder_recursive(pre, tree.get_root());
        }
    } else if (which_test == "iterative inorder") {
        for (int itr=0; itr<_num_itr; ++itr) {
            binary_tree<int>::NodeDeque in;
            tree.inorder(in, tree.get_root());
        }
    } else if (which_test == "search") {
        for (int itr=0; itr<_num_itr; ++itr) {
            //generate random value to search
            int r = rand() % tree.get_size();
            tree.simple_search(r);
        }
    } 
    
    //end timing
    clock_t end = clock();
    
    //show size and iterations
    cout << tree.get_size() << ", ";
    cout << _num_itr << ", ";
    
    double elapsed_time = double(end - start) / double(CLOCKS_PER_SEC);
    //cout << elapsed_time << ", "; //shows total time
    return elapsed_time / double(_num_itr);
}

void initiate_test(string which_tree, string which_test) {
    size_t lower = 2;
    size_t upper = 65536;
    
    int size = lower;
    while(size <= upper) {
        int iterations = int(upper*32/size);
        
        //initialize binary_tree and fill
        if (which_tree == "chain")
            cout << run_test(generate_linked_list(size), which_test, 10+iterations) << endl;
        else if (which_tree == "complete")
            cout << run_test(generate_complete_tree(size), which_test, 10+iterations) << endl;
        
        size *=2;
    }
}
    
int main() {
    //determines which tests are performed
    vector<string> test_types {"recursive preorder", "recursive inorder", "iterative inorder", "search"};
    vector<string> tree_types {"chain", "complete"};
    
    cout << setprecision(10);
    
    for (int i=0; i<test_types.size(); ++i) {
        
        //display which test is being performed
        cout << "testing " << test_types[i] << "() function:" << endl;

        for (int t=0; t<tree_types.size(); ++t) {
            cout << "\ntree type: " << tree_types[t] << endl;
            cout << "Size, Iter, Time(sec)" << endl;
            
            initiate_test(tree_types[t], test_types[i]);
            
            cout << endl;
        }
    }

    return 0;
}

