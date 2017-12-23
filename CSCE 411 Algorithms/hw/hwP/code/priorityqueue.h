// Carsten Hood
// UIN: 922009787
// CSCE 411-200H Fall 2016
// HW P due 2016/12/05
// priorityqueue.h

// custom priority queue implementation

#ifndef priorityqueue_h
#define priorityqueue_h

#include "binaryheap.h"

using namespace std;

// priority queue based on binary heap, utilized by Prim's algorithm
class PriorityQueue {
private:
    BinaryHeap bh;  // underlying binary heap structure
public:
    PriorityQueue(int size);
    void insert(pair<int, int> element);
    void decrease_key(int value, int new_key);
    int extract_min();
    bool is_empty();
    void print();   // display queue for debugging
};

PriorityQueue::PriorityQueue(int size) {
    bh = BinaryHeap(size);
}

bool PriorityQueue::is_empty() {
    return bh.extract_min().second == -1;
}

void PriorityQueue::insert(pair<int,int> element) {
    bh.insert(element);
}

void PriorityQueue::decrease_key(int value, int new_key) {
    bh.decrease_key(value, new_key);
}

int PriorityQueue::extract_min() {
    pair<int,int> element = bh.extract_min();
    bh.delete_min();
    return element.second;
}

void PriorityQueue::print() {
    bh.print_heap();
}

#endif /* priorityqueue_h */
