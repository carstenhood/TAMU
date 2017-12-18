// file: client.C
// student: Carsten Hood
// date: 2016/08/12
// project: MP5 in CSCE 313

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <stdlib.h> // for atoi()
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>  // for setw()
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <errno.h>
#include <unistd.h>

#include "NetworkRequestChannel.H"
#include "bounded_buffer.H"

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

// threads share pointers to these structs
// holds all info relevant to an individual
struct Person {
  string name;                // name of this person, e.g., “Joe Smith”
  size_t request_limit;       // max number of requests for this person
  BoundedBuffer<Message> * response_buffer; // holds replies to requests
  pthread_t request_thread;   // person’s request thread
  pthread_t statistics_thread;// person’s statistics thread
  vector<int> histogram;      // vector of all data resulting from requests

  // constructor
  Person(string _name, size_t _buffer_size, size_t _request_limit)
  : name(_name), request_limit(_request_limit), histogram(100)
  { response_buffer = new BoundedBuffer<Message>(_buffer_size); }

  // destructor
  ~Person() { delete response_buffer; }
};

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

size_t request_channels_count;  // requests per person
size_t total_requests;          // requests per person times number of people
BoundedBuffer<Message> * request_buffer;  // holds requests for worker threads

/*--------------------------------------------------------------------------*/
/* HELPER FUNCTIONS */
/*--------------------------------------------------------------------------*/

// prints all of a person's received data information in a grid
void print_data(Person * person) {
  cout << "\nPrinting data of " << person->name << ": " << endl;
  int total = 0;
  for (int i = 0; i < 10; ++i) {      // loops through rows
    for (int j = 0; j < 10; ++j) {    // print row divider
      cout << setw(6) << "----";
    }
    cout << endl;
    for (int j = 0; j < 10; ++j) {    // prints row’s indices
      cout << setw(6) << (i*10 + j);
    }
    cout << endl;
    for (int j = 0; j < 10; ++j) {    // prints row data
      int value =  person->histogram[i*10 + j];
      cout << setw(6) << value;
      total += value;
    }
    cout << endl;
  }
  cout << "total responses recorded: " << total << endl;
  cout << endl;
}

// loads next request into channel of given index, returns its person
Person * write_message_to_channel(NetworkRequestChannel * _request_channel) {
  Message message = request_buffer->remove();
  _request_channel->cwrite(message.text);
  return (Person *) message.person;
}

// reads response of given index and transfers it to given person’s buffer
void read_message_for_person(
  NetworkRequestChannel * _request_channel, Person * person
){
  string response_text = _request_channel->cread();
  Message message = Message(response_text, (char *)person);
  person->response_buffer->deposit(message);
}

/*--------------------------------------------------------------------------*/
/* THREAD FUNCTIONS */
/*--------------------------------------------------------------------------*/

void * request_thread_function(void * _person) {
  Person * person = (Person *) _person;
  for (int i = 0; i < person->request_limit; ++i) {
    string text = "data " + person->name;
    request_buffer->deposit(Message(text, (char *)person));
  }
  return NULL;
}

void * event_handler_thread_function(void * _request_channels_pointer) {
  // obtain control channel from main function
  vector<NetworkRequestChannel*> * request_channels
    = (vector<NetworkRequestChannel*> *) _request_channels_pointer;

  // maps request channels to people
  Person * people_by_channel[request_channels_count];

  // warm-up phase: load each request channel with a starting request
  for (int i = 0; i < request_channels_count; ++i)
    people_by_channel[i] = write_message_to_channel((* request_channels)[i]);

  // initialize variables for communications-handling loop
  fd_set request_channels_set;        // populated by select() function
  struct timeval timeout = {0, 100};  // for select() function
  int max_fd = 0; // maximum request channel file descriptor for select()

  // write and read  via request channels
  size_t response_counter = 0;
  while (response_counter < total_requests) {

    // reset set of request channels’ file descriptors
    FD_ZERO(&request_channels_set);
    for (int i = 0; i < request_channels_count; ++i)
      FD_SET((* request_channels)[i]->read_fd(), &request_channels_set);

    // find maximum file descriptor of request channels for select() call
    for (int i = 0; i < request_channels_count; ++i)
      if ((* request_channels)[i]->read_fd() > max_fd)
        max_fd = (* request_channels)[i]->read_fd();

    // identify available request channels via select()
    if (select(max_fd + 1, &request_channels_set, NULL, NULL, &timeout))
      for (int i = 0; i < request_channels_count; ++i)
        if (FD_ISSET(
          (* request_channels)[i]->read_fd(),&request_channels_set)
        ){
          // reads response into its associated person’s buffer
          read_message_for_person(
            (* request_channels)[i], people_by_channel[i]
          );
          ++response_counter;

          // if more requests then add next request to request channel
          if (response_counter+request_channels_count-1 < total_requests)
            people_by_channel[i] = write_message_to_channel((* request_channels)[i]);
        }
      // end of for loop
  } // end of while loop
  return NULL;
} // end of event handler thread function

void * statistics_thread_function(void * _person) {
  Person * person = (Person *) _person;
  for (int i = 0; i < person->request_limit; ++i) {
    Message message = person->response_buffer->remove();
    int number = atoi(message.text.c_str());
    person->histogram[number] += 1;
  }
  return NULL;
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {

  // default settings
  size_t requests_per_person = 10000;
  request_channels_count = 100;      // global variable used in event handler
  size_t buffer_size = 100000;

  string host_name = "localHost";
  int port_number = 10360;

  // read settings from program arguments
  char c;
  while ((c = getopt(argc, argv, "n:b:w:h:p:")) != -1) {
    switch (c) {
      case 'n': requests_per_person = atoi(optarg);     break;
      case 'b': buffer_size = atoi(optarg);             break;
      case 'w': request_channels_count = atoi(optarg);  break;
      case 'h': host_name = optarg;                     break;
      case 'p': port_number = atoi(optarg);             break;
    }
  }
  usleep(1000000);

  cout << "CLIENT STARTED:" << endl;

  // --------------------------------------------
  // initialize buffers, channels, and data

  // initialize data for each person (including buffers)
  size_t people_count = 3;
  Person * people[] = {
    new Person({ "Joe Smith", buffer_size, requests_per_person }),
    new Person({ "Jane Smith", buffer_size, requests_per_person }),
    new Person({ "John Doe", buffer_size, requests_per_person }),
  };
  total_requests = people_count * requests_per_person;
  if (request_channels_count > total_requests)
    request_channels_count = total_requests;

  // initialize buffer to hold requests
  size_t person_buffer_size = buffer_size;
  if (person_buffer_size < requests_per_person)
    person_buffer_size = requests_per_person;
  request_buffer = new BoundedBuffer<Message>(buffer_size);

  // initialize network request channels
  vector<NetworkRequestChannel*> * request_channels
    = new vector<NetworkRequestChannel*>(request_channels_count);
  for (int i = 0; i < request_channels_count; ++i)
    (*request_channels)[i] = new NetworkRequestChannel(
      host_name, port_number
    );

  // --------------------------------------------
  // start recording time

  timeval start_time;
  gettimeofday(&start_time, NULL);

  // --------------------------------------------
  // create and commence threads

  // commence statistics threads for each person
  for (int i = 0; i < people_count; ++i)
    pthread_create(&(people[i]->statistics_thread), NULL,
      statistics_thread_function, people[i]);

  // commence event handler thread
  pthread_t event_handler_thread;
    pthread_create(&event_handler_thread, NULL,
      event_handler_thread_function, (void *) request_channels);

  // commence request threads for each person
  for (int i = 0; i < people_count; ++i)
    pthread_create(&(people[i]->request_thread), NULL,
      request_thread_function, people[i]);

  // --------------------------------------------
  // wrap up threads and channels

  // wait for conclusion of request threads
  for (int i = 0; i < people_count; ++i)
    pthread_join(people[i]->request_thread, NULL);

  // wait for conclusion of event handler thread
    pthread_join(event_handler_thread, NULL);

  // close all request channels
  for (int i = 0; i < request_channels_count; ++i)
    (* request_channels)[i]->send_request("quit");

  // delete all request channels
  for (int i = 0; i < request_channels_count; ++i)
    delete (* request_channels)[i];

  // wait for conclusion of statistics threads
  for (int i = 0; i < people_count; ++i)
    pthread_join(people[i]->statistics_thread, NULL);

  // --------------------------------------------
  // end recording time

  timeval end_time;
  gettimeofday(&end_time, NULL);

  // --------------------------------------------
  // output information and terminate program

  usleep(1000000);  // pause before outputing

  // output data for each person (histograms)
  cout << "\nOUTPUT DATA:" << endl;
  for (int i = 0; i < people_count; ++i)
    print_data(people[i]);

  // output input information
  cout << "\nINPUT PARAMETERS:" << endl;
  cout << "number of people: " << people_count << endl;
  cout << "requests per person: " << requests_per_person << endl;
  cout << "buffer size: " << buffer_size << endl;
  cout << "number of request channels: " << request_channels_count << endl;
  cout << "host name: " << host_name << endl;
  cout << "port number: " << port_number << endl;

  // output clock information
  cout << "\nOUTPUT RESULTS:" << endl;
  long seconds = end_time.tv_sec - start_time.tv_sec;
  long microsecs = end_time.tv_usec - start_time.tv_usec;
  if (microsecs < 0) {
    microsecs += 1000000;
    seconds -= 1;
  }
  double seconds_time = (double)seconds + ((double)microsecs) * 0.000001;
  cout << "total time elapsed: " << seconds_time << " seconds" << endl;

  cout << "\nClient program terminated successfully!" << endl;
  cout << endl;
}

// end client.C
