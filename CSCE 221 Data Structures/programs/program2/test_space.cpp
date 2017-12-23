/* This program tests the space usage of
 the binary tree implmentation */

#include "binary_tree.h"

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <sstream>

#include <ctime> //timing
#include <iomanip>   //for set precision

using namespace std;

//generate a single chain structure
binary_tree<int> generate_linked_list(int _n){
    binary_tree<int> T(0);
    binary_tree<int>::node* p = T.get_root();
    for(int i=0; i<_n-1; i++){
        binary_tree<int>::node* c = T.insert_right(p, i+1);
        p = c;
    }
    return T;
}

//generate a complete binary tree structure
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

int main() {
    
    int size = 1;
    string input = "";
    
    //query user for a size to test
    cout << "Size: ";
    getline(cin, input);
    stringstream ss(input);
    ss >> size;
    
    binary_tree<int> TT(generate_complete_tree(size));
    binary_tree<int>::NodeDeque DQ;
    
    //begin the operation
    cout << "Start? ";
    getline(cin, input);
    
    TT.preorder(DQ, TT.get_root());

    //confirm terminating the program
    cout << "Done? ";
    getline(cin, input);
    cout << "Terminating." << endl;
    
    //failing to delete the tree eases observing
    //recursive depth of the traversal algorithm
    
    return 0;
}

