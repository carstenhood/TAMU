//test_stack.cpp : Test the correctness of 
//the array-based implementation of stack 
//defined in array_stack.h and list-based 
//implementation of stack  defined in 
//list_stack.h

#include "array_stack.h"
#include "list_stack.h"

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

const int NUM = 10;

using namespace std;

//Test correctness over all stack functions
template <typename ST, typename T>
void 
test_stack_correctness(ST _s1, T& _input, size_t _size){
  
  //test correctness of pushing onto an empty stack
  try{
    for(size_t i = 0; i < _size; ++i){
      _s1.push(_input[i]);
            
      //checking if push and top are implemented correctly
      if(_s1.size() != i+1){
        cerr << "Error::incorrect push" << endl;
        exit(1);
      }
      else if(_s1.top() != _input[i]){
        cerr << "Error::incorrect top/push" << endl;
        exit(1);
      }
      else {
        cout << "Pushing " << _input[i] << endl;
        cout << "Stack size after push " << _s1.size() << endl;
      }
      //checking if mySize is implemented correctly
      if(_s1.my_size() != _s1.size()){
        cerr << "Error::incorrect my_size()" << endl;
        exit(1);
      }
      if(_s1.top() != _input[i]){
        cerr << "Error::my_size() destroyed the order" << endl;
        exit(1);
      }
      
    }
    cout << "PASSED::push, size, my_size, top" << endl;
  }
  catch(...){
    cerr << "Error::stack_empty_exception, something is amiss!" << endl;
    exit(1);
  }

  //test correctness of assignment, copy, and pop
  ST s2;
  s2 = _s1;
  ST s3(s2);
  
  //double check the order on being popped against ordering in v
  try{
    for(size_t i = _size; i > 0; --i){
       //checking if pop is implemented correctly
       if(_s1.pop() != _input[i-1]){
         cerr << "Error::pop/my_size did not preserve order or pop incorrect " << endl;
         exit(1);
       }
       else if(_s1.size() != i-1){
        cerr << "Error::pop incorrect" << endl;
        exit(1);
      } 
      else {
        cout << "Popping " << _input[i-1] << endl;
        cout << "Stack size after popping: " << _s1.size() <<endl;
      } 

      //checking if assignment operator is implemented correctly
      if(s2.pop() != _input[i-1]){
        cerr << "Error::assignment/my_size did not preserve order or pop not correct" << endl;
        exit(1);
      }

      //checking if copy constructor implemented correctly
      if(s3.pop() != _input[i-1]){
        cerr << "Error::copy constructor/my_size did not preserve order or pop not correct" << endl;
        exit(1);
      }
    }
   cout << "PASSED::pop, assignment, copy constructor" << endl;
  }
  catch(...){
    cerr << "Error::stack_empty_exception, something is amiss!" << endl;
    exit(1);
  }
}

//Testing array_stack
template <typename T, typename IT>
void test_array_stack(IT& _in, int _n){
  cout << "Please enter stack capacity: ";
  int stack_capacity;
  cin >> stack_capacity;
  array_stack<T> as1(stack_capacity);
  test_stack_correctness<array_stack<T>, IT>(as1, _in, _n);
}

//Testing list_stack
template <typename T, typename IT>
void test_list_stack(IT& _in, int _n){
  list_stack<T> ls1;
  test_stack_correctness<list_stack<T>, IT>(ls1, _in, _n);
}


int main(){
  string in[NUM] = {
    " 1: cashew", " 2: Brazil nut",
    " 3: chestnut", " 4: pistachio",
    " 5: walnut", " 6: pecan",
    " 7: peanut", " 8: coconut",
    " 9: hazelnut", " 10: pine nut"};
  
  vector<int> v;
  for(size_t i = 0; i < 1000; ++i){
    int r = rand();
    v.push_back(r);
  }

  //Test array stacks
  //////////////////////////////////////////////////////////
  // Test the array stack using array of
  // strings as input to stack
  //////////////////////////////////////////////////////////
  test_array_stack<string, string[NUM]>(in, NUM);
  ///////////////////////////////////////////////////////////
  // Test array stack using vector of 
  // random numbers as input to stack
  ///////////////////////////////////////////////////////////
  test_array_stack<int, vector<int> >(v, 1000);
  

  //Test list stack
  //////////////////////////////////////////////////////////
  // Test list stack using array of
  // strings as input to stack
  //////////////////////////////////////////////////////////
  test_list_stack<string, string[NUM]>(in, NUM);
  //////////////////////////////////////////////////////////
  // Test list stack using vector of
  // random numbers as input to stack
  //////////////////////////////////////////////////////////
  test_list_stack<int, vector<int> >(v, 1000);
  
}
