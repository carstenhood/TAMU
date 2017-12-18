/* This program tests the binary tree implmentation */

#include "binary_tree.h"
#include <iostream>
#include <cstdlib>
#include <cassert>

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

int main() {
    
    //initialize binary_tree and fill
    binary_tree<int> T = generate_complete_tree(30);
    binary_tree<int> L = generate_linked_list(30);
    
    //test empty
    if(T.is_empty())
        cerr << "Error: Tree T is not empty." << endl;
    if(L.is_empty())
        cerr << "Error: Tree L is not empty." << endl;
    
    //test size
    if(T.get_size() != 30)
        cerr << "Error: Tree T size is not correct." << endl;
    if(L.get_size() != 30)
        cerr << "Error: Tree L size is not correct." << endl;
    
    //test depth
    if(T.get_depth() != 4)
        cerr << "Error: Tree T depth is not correct." << endl;
    if(L.get_depth() > 30)
        cerr << "Error: Tree L depth is not correct." << endl;
    
    //print out tree
    cout << T;
    cout << L;
    
    //test preorder
    binary_tree<int>::NodeDeque pre;
    T.preorder(pre, T.get_root());
    cout << "preorder:";
    for(size_t i=0; i<pre.size(); i++)
        cout << pre[i]->get_element() << ", ";
    cout << endl;
    assert(pre.front() == T.get_root());
    
    //test postorder
    binary_tree<int>::NodeDeque post;
    T.postorder(post, T.get_root());
    cout << "postorder:";
    for(size_t i=0; i<post.size(); i++)
        cout << post[i]->get_element() << ", ";
    cout << endl;
    assert(post.back() == T.get_root());
    
    //test inorder
    binary_tree<int>::NodeDeque in;
    T.inorder(in, T.get_root());
    cout << "inorder:";
    for(size_t i=0; i<in.size(); i++)
        cout << in[i]->get_element() << ", ";
    cout << endl;
    assert(in.at(in.size()/2) == T.get_root());
    
    //test if containing all nodes during traversal
    assert(pre.size() == T.get_size());
    assert(post.size() == T.get_size());
    assert(in.size() == T.get_size());
    
    //test simple_search
    const int query = 5;
    if(T.simple_search(query) == NULL)
        cerr << "Error: Simple search is not correct." << endl;
    if(L.simple_search(query) == NULL)
        cerr << "Error: Simple search is not correct." << endl;
    
    cout << "Passed!" << endl;
    
    T.~binary_tree();
    
    return 0;
}

