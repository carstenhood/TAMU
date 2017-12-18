//list_stack.h -- a list-based implementation of the stack
//Refer to array_stack.h (Stack implemenation)
//for array based implemenation.

#ifndef LISTSTACK_H_
#define LISTSTACK_H_

#include "runtime_exception.h"

using namespace std;

template <typename T>
class list_stack {
public:
    
    class stack_empty_exception : public runtime_exception {
    public:
        stack_empty_exception() : runtime_exception("Access to empty stack") {}
    };
    
    //constructor - simply set top element to NULL and size to 0
    list_stack();
    
    //copy constructor
    list_stack(const list_stack& _st);
    
    //destructor
    ~list_stack();
    
    //assignment operator
    list_stack& operator=(const list_stack& _st);
    
    //is the stack empty?
    bool is_empty() const {return length == 0;}
    
    //number of elements in the stack
    size_t size() const {return length;};
    
    //number of elements in the stack
    size_t my_size();
    
    //return the top of the stack
    T& top() throw(stack_empty_exception);
    
    //push object onto the stack
    void push(const T& _elem);
    
    //pop the stack
    T pop() throw(stack_empty_exception);
    
protected:
    
    //node in the node_list
    struct node {
        T  element;     //element
        node*   next;   //next node
        
        //constructor
        node(const T& _e = T(), node* _n = NULL) : element(_e), next(_n) {}
    };
    
    typedef node* node_ptr; //pointer to a node
    
private:
    //member data
    size_t  length; //current length of stack
    node_ptr head; //top of the stack
};

//constructor - simply set top element to NULL and size to 0
template <typename T>
list_stack<T>::list_stack() : head(NULL), length(0) {}

//copy constructor
template <typename T>
list_stack<T>::list_stack(const list_stack& _st) {
    //copy variables
    length=_st.length;
    
    //copy elements
    if (_st.head == NULL) {
        head = NULL;
    } else {
        head = new node(_st.head->element, NULL);
        node_ptr curr = head;
        node_ptr base = _st.head->next;
        while (base != NULL) {
            curr->next = new node(base->element, NULL);
            base = base->next;
            curr = curr->next;
        }
    }
}

//destructor
template <typename T>
list_stack<T>::~list_stack() {
    while (head) {
        node_ptr deleted = head;
        head = head->next;
        delete deleted;
        --length;
    }
}

//assignment operator
template <typename T>
list_stack<T>&
list_stack<T>::operator=(const list_stack& _st) {
    if (this != &_st) {
        //delete old contents
        while (head) {
            node_ptr deleted = head;
            head = head->next;
            delete deleted;
        }
        
        //copy variables
        length=_st.length;
        node_ptr base = _st.head;
        
        //copy contents
        if (base == NULL) {
            head = NULL;
        } else {
            head = new node(_st.head->element, NULL);
            node_ptr curr = head;
            base = _st.head->next;
            while (base != NULL) {
                curr->next = new node(base->element, NULL);
                base = base->next;
                curr = curr->next;
            }
        }
    }
    return *this;
}

//number of elements in the stack
template <typename T>
size_t
list_stack<T>::my_size() {
    size_t s = 0;
    node_ptr iter = head;
    while (iter) {
        iter = iter->next;
        ++s;
    }
    return s;
}

//return the top of the stack
template <typename T>
T&
list_stack<T>::top() throw(stack_empty_exception) {
    if (is_empty())
        throw stack_empty_exception();
    return head->element;
}

//push object onto the stack
template <typename T>
void
list_stack<T>::push(const T& _elem) {
    node_ptr pushed = new node(_elem, head);
    head = pushed;
    ++length;
}

//pop the stack
template <typename T>
T
list_stack<T>::pop() throw(stack_empty_exception) {
    if (is_empty())
        throw stack_empty_exception();
    node_ptr popped = head;
    head = popped->next;
    T elem = popped->element;
    delete popped;
    --length;
    return elem;
}

#endif
