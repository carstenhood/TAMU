/*
    File: semaphore.C
    Author: Carsten Hood
    UIN: 922009787
    Date  : 07/21/16
*/

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "semaphore.H"
#include <pthread.h>

/*--------------------------------------------------------------------------*/
/* DEFINITIONS */
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

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
  return 0;
}
