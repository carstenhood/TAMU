// file: bounded_buffer.H
// student: Carsten Hood
// date: 2016/08/12
// project: MP5 in CSCE 313

#ifndef _bounded_buffer_H_                   // include file only once
#define _bounded_buffer_H_

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <queue>
#include "semaphore.H"

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

struct Message {
  string text;    // actual request message
  char * person;  // pointer to person’s information

  Message(string _text, char * _person) : text(_text), person(_person)
  { }
};

/*--------------------------------------------------------------------------*/
/* CLASS   B o u n d e d  B u f f e r */
/*--------------------------------------------------------------------------*/

template<class T>
class BoundedBuffer {

public:
  /* -- CONSTRUCTOR / DESTRUCTOR */
  BoundedBuffer(unsigned int _size);
  BoundedBuffer();
  ~BoundedBuffer();

  /* -- BUFFER FUNCTIONS */
  void deposit(T item);
  T remove();

private:
  /* -- DATA VARIABLES */
  unsigned int size;
  queue<T> buffer_queue;

  /* -- SEMAPHORE VARIABLES */
  Semaphore mutex;
  Semaphore empty;
  Semaphore full;
};

/*--------------------------------------------------------------------------*/
/* DEFINITIONS   B o u n d e d  B u f f e r */
/*--------------------------------------------------------------------------*/

/* -- CONSTRUCTOR / DESTRUCTOR */

template<class T> BoundedBuffer<T>::BoundedBuffer(unsigned int _size)
: mutex(1), empty(_size), full(0) {
  size = _size;
}

template<class T> BoundedBuffer<T>::BoundedBuffer() : size(0) { }

template<class T> BoundedBuffer<T>::~BoundedBuffer() { }

/* -- BUFFER FUNCTIONS */

template<class T> void BoundedBuffer<T>::deposit(T _item) {
  empty.P();
    mutex.P();
      buffer_queue.push(_item);
    mutex.V();
  full.V();
}

template<class T> T BoundedBuffer<T>::remove() {
  full.P();
    mutex.P();
      T item = buffer_queue.front();
      buffer_queue.pop();
    mutex.V();
  empty.V();
  return item;
}

#endif
