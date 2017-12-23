
/*
    File: semaphore.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/11

*/

#ifndef _semaphore_H_                   // include file only once
#define _semaphore_H_

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <pthread.h>

/*--------------------------------------------------------------------------*/
/* CLASS   S e m a p h o r e  */
/*--------------------------------------------------------------------------*/

class Semaphore {
private:
  /* -- INTERNAL DATA STRUCTURES
     You may need to change them to fit your implementation. */
  int             value;
  pthread_mutex_t m;
  pthread_cond_t  c;

public:

  /* -- CONSTRUCTOR/DESTRUCTOR */
  Semaphore(int _val);
  ~Semaphore();

  /* -- SEMAPHORE OPERATIONS */
  int P();
  int V();
};

/*--------------------------------------------------------------------------*/
/* DEFINITIONS for S e m a p h o r e */
/*--------------------------------------------------------------------------*/

/* -- CONSTRUCTOR/DESTRUCTOR */

Semaphore::Semaphore(int _val) {
  value = _val;
  m = PTHREAD_MUTEX_INITIALIZER;
  c = PTHREAD_COND_INITIALIZER;
}

Semaphore::~Semaphore() { }

/* -- SEMAPHORE OPERATIONS */

int Semaphore::P() {
  pthread_mutex_lock(&m);
  --value;
  if (value < 0) {
    pthread_cond_wait(&c, &m);
  }
  pthread_mutex_unlock(&m);
  return value;
}

int Semaphore::V() {
  pthread_mutex_lock(&m);
  ++value;
  if (value <= 0) {
    pthread_cond_signal(&c);
  }
  pthread_mutex_unlock(&m);
  return value;
}

#endif
