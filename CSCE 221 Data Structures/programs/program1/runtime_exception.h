//runtime_exception.h 
//Defines the runtime exception class
//from which stack exceptions are derived
//as defined in array_stack.h and list_stack.h

#ifndef RUNTIMEEXCEPTION_H_
#define RUNTIMEEXCEPTION_H_

#include <string>

using namespace std;

//runtime exception
class runtime_exception {
  public:
    //constructor
    runtime_exception (const string& _err) : error_msg(_err) {} 
    //returns error message
    string get_message() const { return error_msg; } 
  
  private:
    string error_msg;  //error message
};

//displays the error message of an exception
inline std::ostream& operator<<(std::ostream& _out, const runtime_exception& _e){ 
  return _out << _e.get_message();
}

#endif
