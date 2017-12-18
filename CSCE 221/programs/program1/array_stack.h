// array_stack.h
// This class provides a template stack. Internally
// the stack is stored as an array.

#ifndef ARRAYSTACK_H_
#define ARRAYSTACK_H_

#include "runtime_exception.h"

#define CAPACITY 2 //default capacity of the stack

template <typename T>
class array_stack {
public:
    //exception classes thrown by various functions
    class stack_empty_exception : public runtime_exception {
    public:
        stack_empty_exception() : runtime_exception("Access to empty stack") {}
    };
    
    class stack_full_exception : public runtime_exception {
    public:
        stack_full_exception() : runtime_exception("Stack overflow") {}
    };
    
    //constructor given max capacity
    array_stack(size_t _cap = CAPACITY);
    
    //copy constructor
    array_stack(const array_stack& _st);
    
    //destructor
    ~array_stack() { delete [] s; }
    
    //assignment operator
    array_stack& operator=(const array_stack& _st);
    
    //is the stack empty?
    bool is_empty() const { return t < 0; }
    
    //number of elements in the stack
    size_t size() const { return t+1; }
    
    //number of elements in the stack
    size_t my_size();
    
    //return the top of the stack
    T& top() throw(stack_empty_exception);
    
    //push object onto the stack
    void push(const T& _elem) throw(stack_full_exception);
    
    //pop the stack
    T pop() throw(stack_empty_exception);
    
private:
    //member data
    size_t       capacity; //actual length of stack array
    T*   s; //the stack array
    int    t; //index of the top of the stack
    int k; //controls amount to increase capacity when full
};

//constructor given max capacity
template <typename T>
array_stack<T>::array_stack(size_t _cap) {
    capacity  = _cap;
    s         = new T[capacity];
    t         = -1;
    k         = 0;
}

//copy constructor
template <typename T>
array_stack<T>::array_stack(const array_stack& _st) {
    //initialize internal variables
    capacity  = _st.capacity;
    t         = _st.t;
    s         = new T[capacity];
    k         = _st.k;
    
    //copy contents
    for (int i = 0; i <= t; ++i) {
        s[i] = _st.s[i];
    }
}

//assignment operator
template <typename T>
array_stack<T>&
array_stack<T>::operator=(const array_stack& _st) {
    //avoid self copy (x = x)
    if(this != &_st) {
        
        //delete old contents
        delete [] s;
        
        //initialize internal variables
        capacity  = _st.capacity;
        t         = _st.t;
        s         = new T[capacity];
        k         = _st.k;
        
        //copy contents
        for(int i = 0; i <= t; ++i) {
            s[i] = _st.s[i];
        }
    }
    return *this;
}

//number of elements in the stack
template <typename T>
size_t
array_stack<T>::my_size() {
    size_t s = 0;
    array_stack temp_stack = *this;
    
    while (!temp_stack.is_empty()) {
        temp_stack.pop();
        ++s;
    }
    
    return s;
}

//return the top of the stack
template <typename T>
T&
array_stack<T>::top() throw(stack_empty_exception) {
    if (is_empty())
        throw stack_empty_exception();
        return s[t];
}

//push object onto the stack
template <typename T>
void
array_stack<T>::push(const T& _elem) throw(stack_full_exception) {
    //increase capacity by k units; k of 0 means doubling
    if (size() == capacity) {
        if (k>0) {
            capacity+=k;
        } else {
            capacity*=2;
        }
        
        //copy elements to a new stack
        T* bigger_stack = new T[capacity];
        for (int i=0; i < size(); ++i)
            bigger_stack[i] = s[i];
            if (s != NULL)
                delete [] s;
        
        s = bigger_stack;
    }
    s[++t] = _elem;
}

//pop the stack
template <typename T>
T
array_stack<T>::pop() throw(stack_empty_exception) {
    if (is_empty())
        throw stack_empty_exception();
    return s[t--];
}

#endif
