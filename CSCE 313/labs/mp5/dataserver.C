// file: dataserver.C
// student: Carsten Hood
// date: 2016/08/12
// project: MP5 in CSCE 313

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

// added
#include <stdio.h>

#include "NetworkRequestChannel.H"

using namespace std;

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

const int MAX_MESSAGE = 255;

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- SUPPORT FUNCTIONS */
/*--------------------------------------------------------------------------*/

string int2string(int _number) {
   stringstream ss; //create a stringstream
   ss << _number;    //add number to the stream
   return ss.str(); //return a string with the contents of the stream
}

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- INDIVIDUAL REQUESTS */
/*--------------------------------------------------------------------------*/

void process_hello(int * _fd) {
  string response = "hello to you too";
  write(* _fd, response.c_str(), response.length()+1);
}

void process_data(int * _fd) {
  usleep(1000 + (rand() % 5000));
  string response = int2string(rand() % 100);
  write(* _fd, response.c_str(), response.length()+1);
}

void process_quit(int * _fd) {
  string response = "quitting";
  write(* _fd, response.c_str(), response.length()+1);
}

void process_unknown(int * _fd, const string & _request) {
  string response = "unknown request: " ;
  write(* _fd, response.c_str(), response.length()+1);
}

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- THE PROCESS REQUEST LOOP */
/*--------------------------------------------------------------------------*/

void process_request(int * _fd, const string & _request) {
  if (_request.compare(0, 5, "hello") == 0) {
    process_hello(_fd);
  } else if (_request.compare(0, 4, "data") == 0) {
    process_data(_fd);
  } else if (_request.compare(0, 4, "quit") == 0) {
    process_quit(_fd);
  } else {
    process_unknown(_fd, _request);
  }
}

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- THREAD FUNCTIONS */
/*--------------------------------------------------------------------------*/

void * connection_handler_function(void * _fd_pointer) {
  int * file_descriptor = (int *)_fd_pointer;
  while (true) { // then read request from server
    char buf[MAX_MESSAGE];
    read(* file_descriptor, buf, MAX_MESSAGE);
    string request = buf;
    process_request(file_descriptor, request);
    if (!request.compare("quit")) break;
  }
  usleep(10000);
  return NULL;
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {

  // default settings
  unsigned short port_number = 10360;
  size_t backlog = 10;

  // read settings from program arguments
  char c;
  while ((c = getopt(argc, argv, "p:b:")) != -1) {
    switch (c) {
      case 'p': port_number = atoi(optarg); break;
      case 'b': backlog = atoi(optarg); break;
    }
  }

  NetworkRequestChannel server_channel = NetworkRequestChannel(
    port_number, connection_handler_function, backlog
  );
}
