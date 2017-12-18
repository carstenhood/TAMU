/* binary_tree.h:
 * A C++ template which implements a binary tree ADT.
 */
#ifndef BINARYTREE_H
#define BINARYTREE_H
#include <iostream>
#include <deque>
#include "euler_tour.h"
using namespace std;

template <typename T>
class binary_tree {
    
    
public:
    class child_exists_exception{};
    
    class node {				// node in the binary tree
    private:
        T element;					// element
        node*  parent;					// parent
        node*  leftchild;				// left child
        node*  rightchild;				// right child
        int    depth;					// depth of node
        
    public:
        node(const T& _e = T(), node* _p = NULL, node* _l = NULL, node* _r = NULL)
        : element(_e), parent(_p), leftchild(_l), rightchild(_r) { 	// constructor for node
            set_element(_e);
            if (_p != NULL)
                depth = (_p->depth)+1;
            else
                depth = 0;
        }
        
        node(const node& n) {   // copy constructor for node
            element = n.get_element();
            depth = n.get_depth();
            if (n.get_left_child() != NULL) {
                set_left_child(new node(*n.get_left_child()));
                get_left_child()->set_parent(this);
            }
            if (n.get_right_child() != NULL) {
                set_right_child(new node(*n.get_right_child()));
                get_right_child()->set_parent(this);
            }
            
        }
        
        ~node() {   // destructor for node
            delete rightchild;
            rightchild = NULL;
            delete leftchild;
            leftchild = NULL;
        }
        
        //accessor functions:
        T get_element() const {return element;}
        node* get_parent() const {return parent;}
        node* get_left_child() const {return leftchild;}
        node* get_right_child() const {return rightchild;}
        int get_depth() const {return depth;}
        bool is_external() {return leftchild == NULL && rightchild == NULL;}
        
        //modifier functions
        void set_element(const T& _e) {element = _e;}
        void set_parent(node* _p) {parent = _p;}
        void set_left_child(node* _l) {leftchild = _l;}
        void set_right_child(node* _r) {rightchild = _r;}
        
    };
    
    typedef node* NodePtr;				// pointer to the node
    typedef deque<NodePtr> NodeDeque;
    
private:
    NodePtr root;						// root of binary tree
    int size;                           // number of nodes
    int depth;                          // depth of tree
    
public:
    //default constructor for binary tree
    binary_tree() : root(new node()), size(0), depth(0) { }
    //copy constructor for binary tree
    binary_tree(const binary_tree &C) {
        if (C.root == NULL)
            root = NULL;
        else
            root = new node(*C.root);
        size = C.size;
        depth = C.depth;
    }
    //assignment operator for binary tree
    binary_tree<T>& operator=(const binary_tree<T>& C) {
        if (this != &C) {
            if (root != NULL)
                delete root;
            if  (C.root != NULL)
                root = new node(*C.root);
            depth = C.depth;
            size = C.size;
        }
        return *this;
    }
    //create a binary tree and initialize the root
    binary_tree(const T& _e) : root(new node(_e)), size(1), depth(0) { }
    //destructor for binary tree
    ~binary_tree() {
        delete root;
        root = NULL;
    }
    
    //return size of binary tree
    size_t get_size() const {return size;}
    //return the depth of the binary tree
    int get_depth() const {return depth;}
    //is the binary tree empty?
    bool is_empty() {return size == 0;}
    //return the root of the binary tree
    NodePtr get_root() const {return root;}
    //print out the tree
    friend ostream& operator<<(ostream& _out, binary_tree<T>& _T) {
        deque<NodePtr> in;
        in.clear();
        _T.inorder(in, _T.get_root());
        for (size_t i=0; i<in.size(); i++) {
            for (int j=0; j<in[i]->get_depth(); j++)
                _out << "  ";
            _out << in[i]->get_element() << endl;
        }
        return _out;
    }
    
    //insert a node with element _e to the left of _p
    NodePtr insert_left(NodePtr _p, const T& _e);
    //insert a node with element _e to the right of _p
    NodePtr insert_right(NodePtr _p, const T& _e);
    //give a deque that contains the preorder traversal of binary tree
    void preorder(deque<NodePtr>& _v, NodePtr _r);
    //give a deque that contains the iterative inorder traversal of the binary tree
    void inorder(deque<NodePtr>& _v, NodePtr _r);
    //give a deque that contains the recursive inorder traversal of the binary tree
    void inorder_recursive(deque<NodePtr>& _v, NodePtr _r);
    //give a deque that contains the postorder traversal of the binary tree
    void postorder(deque<NodePtr>& _v, NodePtr _r);
    //search the binary tree for the element _e
    NodePtr simple_search(const T& _e);
    
};

//insert a node with element _e to the left of _p
template <typename T>
typename binary_tree<T>::NodePtr
binary_tree<T>::insert_left(binary_tree<T>::NodePtr _p, const T& _e) {
    
    //check child does not already exist
    if (_p->get_left_child() != NULL)
        throw child_exists_exception();
    
    NodePtr _n = new node(_e, _p, NULL, NULL);
    _p->set_left_child(_n);
    
    //if new node increases tree depth, adjust tree depth
    if (_n->get_depth() > depth)
        ++depth;
    
    ++size;
    
    return _n;
}

//insert a node with element _e to the right of _p
template <typename T>
typename binary_tree<T>::NodePtr
binary_tree<T>::insert_right(binary_tree<T>::NodePtr _p, const T& _e) {
    
    //check child does not already exist
    if (_p->get_right_child() != NULL)
        throw child_exists_exception();
    
    NodePtr _n = new node(_e, _p, NULL, NULL);
    _p->set_right_child(_n);
    
    //if new node increases tree depth, adjust tree depth
    if (_n->get_depth() > depth)
        depth = _n->get_depth();
    
    ++size;
    
    return _n;
}

//give a deque that contains the preorder traversal of the binary tree
template <typename T>
void
binary_tree<T>::preorder(deque<binary_tree<T>::NodePtr>& _v, binary_tree<T>::NodePtr _r) {
    recursive_preorder<T>(_v).euler_tour_traversal(_r);
}

//give a deque that contains the inorder traversal of the binary tree
template <typename T>
void
binary_tree<T>::inorder(deque<binary_tree<T>::NodePtr>& _v, binary_tree<T>::NodePtr _r) {
    
    deque<binary_tree<T>::NodePtr> _a; //temporarily holds ancestors
    
    while (!_a.empty() || _r != NULL) {
        //add all elements in left chain to temporary deque
        if (_r != NULL) {
            _a.push_back(_r);
            _r = _r->get_left_child();
        } else { //if next node is null, visit back node in deque
            _r = _a.back();
            _a.pop_back();
            _v.push_back(_r);
            _r = _r->get_right_child(); //move to right subtree
        }
    }
}

//give a deque that contains the postorder traversal of the binary tree
template <typename T>
void
binary_tree<T>::inorder_recursive(deque<binary_tree<T>::NodePtr>& _v, binary_tree<T>::NodePtr _r) {
    recursive_inorder<T>(_v).euler_tour_traversal(_r);
}

//give a deque that contains the postorder traversal of the binary tree
template <typename T>
void
binary_tree<T>::postorder(deque<binary_tree<T>::NodePtr>& _v, binary_tree<T>::NodePtr _r) {
    recursive_postorder<T>(_v).euler_tour_traversal(_r);
}

//search the binary tree for element _e
template <typename T>
typename binary_tree<T>::NodePtr
binary_tree<T>::simple_search(const T& _e) {
    
    //use inorder traversal to get deque of all elements
    deque<binary_tree<T>::NodePtr> _v;
    inorder(_v, root);
    
    //loop through deque to search for element
    NodePtr _p;
    while (_v.size() > 0) {
        _p = _v.back();
        if (_p->get_element() == _e)
            return _p;
        _v.pop_back();
    }
    
    return NULL; //return NULL if element is not found
}

#endif

