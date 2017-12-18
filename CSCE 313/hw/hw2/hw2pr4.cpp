// condition_variable example
#include <iostream>           // cout
#include <thread>             // thread
#include <mutex>              // mutex, unique_lock
#include <condition_variable> // condition_variable

#include <pthread.h>
#include <sys/time.h>

using namespace std;

/* Servers can be designed to limit the number of open connections. For example, a server may wish to have only N connections active at any time. As soon as N connections are established, the server will not handle more connections until at last one existing connection is closed. How would you use semaphores to limit the number of concurrent connections? Assume that you have a function called HandleNextIncomingConnection() and one called CloseExistingConnection(), which are called before and at the end of handling a connection. How would you add the necessary synchronization code to limit the number of concurrent connections to at most N? */

Semaphore server(N);
HandleNextIncomingConnection() {
    server.P();
    // handle next connection
}
CloseExistingConnection() {
    // close existing connection
    server.V();
}


pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

void custom_sleep(int delay) {
    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    int microsecs_in_sec = 1000000;
    int seconds = delay/microsecs_in_sec;
    int microsecs = delay%microsecs_in_sec;
    
    struct timespec end_time;
    end_time.tv_sec = start_time.tv_sec + seconds;
    end_time.tv_nsec = start_time.tv_usec + microsecs;
    
    pthread_mutex_lock(&mutex1);
    pthread_cond_timedwait(&cond1, &mutex1, &end_time);
    pthread_mutex_unlock(&mutex1);
}

void* fun(void* arg) {
    printf("\nIn thread\n");
    cout << "hello" << endl;
    mywait(10000000);
    cout << "goodbye" << endl;
    return NULL;
}

int main() {
    pthread_t thread;
    void *ret;
    pthread_create(&thread, NULL, fun, NULL);
    pthread_join(thread,&ret);
}