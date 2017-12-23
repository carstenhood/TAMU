// file: NetworkRequestChannel.C
// student: Carsten Hood
// date: 2016/08/12
// project: MP5 in CSCE 313

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>

// additions
#include <sys/socket.h>
#include <sstream>
#include <netdb.h>
#include <arpa/inet.h>

#include "NetworkRequestChannel.H"

using namespace std;

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS NetworkRequestChannel */
/*--------------------------------------------------------------------------*/

// constructor for establishing client connection
NetworkRequestChannel::NetworkRequestChannel(
 const string _server_host_name, const unsigned short _port_no
){
  // declare data structures
  struct hostent * he;    // host entity
  struct sockaddr_in socket_address;
  memset(&socket_address, 0, sizeof(socket_address));

  // connecting host
  he = gethostbyname(string_to_cstring(_server_host_name));
  if (!he) {
    cout << "Connecting host '" << _server_host_name << "' failed" << endl;
    exit(-1);
  }
  memcpy(&socket_address.sin_addr, he->h_addr, he->h_length);

  // connecting server
  socket_address.sin_port = htons(_port_no);
  if (!socket_address.sin_port) {
    cout << "Connecting server '" << _port_no << "' failed" << endl;
    exit(-1);
  }

  // use internet addresses, “safest option”
  socket_address.sin_family = AF_INET;

  // establish file descriptor
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  int c = connect(
    socket_fd, (struct sockaddr *)&socket_address, sizeof(socket_address)
  );
  file_descriptor = socket_fd;

  cout << "Connected client with host name: " << _server_host_name
                              << ", port #: " << _port_no << endl;
}
/* Creates a CLIENT-SIDE local copy of the channel. The channel is
connected to the given port number at the given server host.
THIS CONSTRUCTOR IS CALLED BY THE CLIENT. */

// constructor for establishing server connection
NetworkRequestChannel::NetworkRequestChannel(
 const unsigned short _port_no,
 void * (*connection_handler) (void *),
 size_t _backlog
){
  // initialize server address structure
  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));

  // connecting server
  server_address.sin_port = htons(_port_no);
  if (!server_address.sin_port) {
    cout << "Connecting server '" << _port_no << "' failed" << endl;
    exit(-1);
  }

  // use internet addresses, “safest option”
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;

  // creating socket
	int socket_fd  = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) {
    cout << "Creating socket failed (" << socket_fd << ")" << endl;
    exit(-1);
  }

  // bind socket & server
  int bind_result = bind(
    socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)
  );
  if (bind_result < 0) {
    cout << "Binding '" << socket_fd << "' failed"
                << " (" << bind_result << ")" << endl;
    exit(-1);
  }

  cout << "Connected server with port #: "  << _port_no
                           << ", backlog: " << _backlog << endl;
  cout << "Waiting for clients... " << endl;

  listen(socket_fd, (int)_backlog);  // check for connections

  // declare thread and thread attributes
  pthread_t thread;
  pthread_attr_t thread_attr;
  pthread_attr_init(&thread_attr);

  // loop until socket is accepted
  int server_address_size = (int)sizeof(server_address);
  while (true) {
    int accept_socket = accept(
      socket_fd, (struct sockaddr *)&server_address,
      (socklen_t *)&server_address_size
    );
    if (accept_socket >= 0) {
      pthread_create(
        &thread, &thread_attr, connection_handler,
        (void *)(new int(accept_socket))
      );
    }
	}
}
/* Creates a SERVER-SIDE local copy of the channel that is accepting
connections at the given port number.
NOTE that multiple clients can be connected to the same server-side end of
the request channel. Whenever a new connection comes in, it is accepted by
the server, and the given connection handler is invoked. The parameter to
the connection handler is the file descriptor of the slave socket returned
by the accept call.
NOTE that the connection handler does not want to deal with
closing the socket. You will have to close the socket once the
connection handler is done. */

NetworkRequestChannel::~NetworkRequestChannel() {
  close(file_descriptor);
}
/* Destructor of the local copy of the channel. */

/*--------------------------------------------------------------------------*/
/* Auxiliary functions for converting to cstrings */
/*--------------------------------------------------------------------------*/

// constructors’ helper function converts “const unsigned short” to a cstring
char * NetworkRequestChannel::string_to_cstring(const string _string) {
  char * string_cstring = new char[_string.length()+1];
  strcpy (string_cstring, _string.c_str());
  return string_cstring;
}

// for printing the file descriptor
string NetworkRequestChannel::fd_as_string() {
  stringstream fd_stream;
  fd_stream << file_descriptor;
  return fd_stream.str();
}

/*--------------------------------------------------------------------------*/
/* READ/WRITE FROM/TO REQUEST CHANNELS  */
/*--------------------------------------------------------------------------*/

const int MAX_MESSAGE = 255;

string NetworkRequestChannel::send_request(string _request) {
  cwrite(_request);
  string s = cread();
  return s;
}
/* Send a string over the channel and wait for a reply. */

string NetworkRequestChannel::cread() {
  char buf[MAX_MESSAGE];
  if (read(file_descriptor, buf, MAX_MESSAGE) < 0) {
    perror(string("File Descriptor " + fd_as_string() + ": Error reading from pipe!").c_str());
  }
  string s = buf;
  return s;
}
/* Blocking read of data from the channel. Returns a string of characters read from the channel. Returns NULL if read failed. */

int NetworkRequestChannel::cwrite(string _msg) {
  if (_msg.length() >= MAX_MESSAGE) {
    cerr << "Message too long for Channel!\n";
    return -1;
  }
  const char * s = _msg.c_str();
  if (write(file_descriptor, s, strlen(s)+1) < 0) {
    perror(string("File Descriptor " + fd_as_string() + ": Error writing to pipe!").c_str());
    return -1;
  }
  return 0;
}
/* Write the data to the channel. The function returns the number
of characters written to the channel. */

int NetworkRequestChannel::read_fd() {
  return file_descriptor;
}
/* Returns the file descriptor used to read from the channel. */
