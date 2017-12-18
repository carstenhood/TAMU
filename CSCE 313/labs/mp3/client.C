/*
    File: client.C
    Author: Carsten Hood
    Date  : 2016/07/28

    Client main program for MP3 in CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <stdlib.h> // for atoi()
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>  // for setw()

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <errno.h>
#include <unistd.h>

#include "reqchannel.H"
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
  pthread_t stat_thread;      // person’s statistics thread
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

/*--------------------------------------------------------------------------*/
/* THREAD FUNCTIONS */
/*--------------------------------------------------------------------------*/

void * request_thread_function(void * _person) {
  Person * person = (Person *) _person;
  for (int i = 0; i < person->request_limit; ++i) {
    string text = "data " + person->name;
    request_buffer->deposit(Message(text, (char *)person));
  }
}

void * worker_thread_function(void * _request_channel) {
  RequestChannel * request_channel = (RequestChannel *) _request_channel;
  while (true) {
    Message message = request_buffer->remove();
    string reply = request_channel->send_request(message.text);
    if (message.text == "quit") {
      break;
    } else {
      message.text = reply;
      Person * person = (Person *)message.person;
      person->response_buffer->deposit(message);
    }
  }
}

void * stat_thread_function(void * _person) {
  Person * person = (Person *) _person;
  for (int i = 0; i < person->request_limit; ++i) {
    Message message = person->response_buffer->remove();
    int number = atoi(message.text.c_str());
    person->histogram[number] += 1;
  }
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {

  // default settings
  size_t requests_per_person = 10000;
  size_t worker_threads_count = 10;
  size_t buffer_size = 100;

  // read settings from program arguments
  char c;
  while ((c = getopt(argc, argv, "n:b:w:")) != -1) {
    switch (c) {
      case 'n':
        requests_per_person = atoi(optarg);
        break;
      case 'b':
        buffer_size = atoi(optarg);
        break;
      case 'w':
        worker_threads_count = atoi(optarg);
        break;
    }
  }

  pid_t p = fork(); // fork to establish server process
  cout << "p: " << p << endl;
  if (p == 0) {
    execl("./dataserver", (char *)0);
  } else if (p > 0) {

    cout << "CLIENT STARTED:" << endl;

    cout << "Establishing control channel... " << flush;
    RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
    cout << "done." << endl;

    // --------------------------------------------
    // initialize buffers and channels

    // initialize buffer to hold requests
    request_buffer = new BoundedBuffer<Message>(buffer_size);

    // initialize data for each person (including buffers)
    size_t people_count = 3;
    Person * people[] = {
      new Person({ "Joe Smith", buffer_size, requests_per_person }),
      new Person({ "Jane Smith", buffer_size, requests_per_person }),
      new Person({ "John Doe", buffer_size, requests_per_person }),
    };

    // initialize request channel for each worker thread
    RequestChannel * request_channels[worker_threads_count];
    for (int i = 0; i < worker_threads_count; ++i)
      request_channels[i] = new RequestChannel(chan.send_request("newthread"),
        RequestChannel::CLIENT_SIDE);

    // --------------------------------------------
    // start recording time

    timeval start_time;
    gettimeofday(&start_time, NULL);

    // --------------------------------------------
    // create and commence threads

    // commence statistics threads for each person
    for (int i = 0; i < people_count; ++i)
      pthread_create(&(people[i]->stat_thread), NULL,
        stat_thread_function, people[i]);

    // commence total number of worker threads
    pthread_t worker_threads[worker_threads_count];
    for (int i = 0; i < worker_threads_count; ++i)
      pthread_create(&worker_threads[i], NULL, worker_thread_function,
        request_channels[i]);

    // commence request threads for each person
    for (int i = 0; i < people_count; ++i)
      pthread_create(&(people[i]->request_thread), NULL,
        request_thread_function, people[i]);

    // --------------------------------------------
    // wait for conclusion of threads

    // wait for end of request threads
    for (int i = 0; i < people_count; ++i)
      pthread_join(people[i]->request_thread, NULL);

    // close worker channels
    for (int i = 0; i < worker_threads_count; ++i)
      request_buffer->deposit(Message("quit", NULL));

    // wait for conclusion of worker threads
    for (int i = 0; i < worker_threads_count; ++i)
      pthread_join(worker_threads[i], NULL);

    // wait for conclusion of statistics threads
    for (int i = 0; i < people_count; ++i)
      pthread_join(people[i]->stat_thread, NULL);

    // close channel processes
    string quit_response = chan.send_request("quit");
    cout << "Reply to request 'quit' is '" << quit_response << "'" << endl;

    // --------------------------------------------
    // end recording time

    timeval end_time;
    gettimeofday(&end_time, NULL);

    // --------------------------------------------
    // output information and terminate program

    usleep(1000000);  // pause before outputing

    // output data for each person (histograms)
    cout << "__printing output data__" << endl;
    for (int i = 0; i < people_count; ++i)
      print_data(people[i]);

    // output input information
    cout << "__parameter information__" << endl;
    cout << "number of people: " << people_count << endl;
    cout << "requests per person: " << requests_per_person << endl;
    cout << "buffer size: " << buffer_size << endl;
    cout << "number of worker threads: " << worker_threads_count << endl;
    cout << endl;

    // output clock information
    cout << "__time information__" << endl;
    long seconds = end_time.tv_sec - start_time.tv_sec;
    long microsecs = end_time.tv_usec - start_time.tv_usec;
    if (microsecs < 0) {
      microsecs += 1000000;
      seconds -= 1;
    }
    long microsec_time = seconds*1000000 + microsecs;
    cout << "secs: " << seconds << ", usecs: " << microsecs << endl;
    cout << "total time: " << microsec_time << " microseconds" << endl << endl;

    // for copying into data sheet
    cout << "Tab-delimited input/output info to copy into data sheet:" << endl;
    cout << "reqs\tbuff\tworkers\ttime" << endl;
    cout << requests_per_person << "\t" << buffer_size << "\t"
         << worker_threads_count << "\t"
         << seconds << "." << microsecs << "\t"
         << endl << endl;

    cout << "Client program terminated successfully!" << endl << endl;
  }
}

// end client.C
