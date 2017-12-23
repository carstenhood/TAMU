// Carsten Hood
// UIN: 922009787
// CSCE 411-200H Fall 2016
// HW P due 2016/12/05
// binaryheap.h

// custom binary heap implementation

// significant references:
// http://www.sanfoundry.com/cpp-program-implement-binary-heap/;
// http://stackoverflow.com/questions/17009056/how-to-implement-ologn-decrease-key-operation-for-min-heap-based-priority-queu;

using namespace std;

// binary heap foundation for priority queue utilized by Prim's algorithm
class BinaryHeap {
private:
    vector<pair<int, int>> heap;
    vector<int> indices; // maps vertices to heap indices for decreasekey
    int left(int parent);
    int right(int parent);
    int parent(int child);
    void heapify_up(int index);
    void heapify_down(int index);
    void swap(int a, int b);
public:
    BinaryHeap() {}
    BinaryHeap(int size);
    void insert(pair<int, int> element);
    void delete_min();
    pair<int, int> extract_min();
    void decrease_key(int value, int new_key);
    void print_heap();
    int size();
};

// ==================================================
// public functions

BinaryHeap::BinaryHeap(int size) {
    // initially heap contains no vertices so vertex indices map
    // indicates null values
    for (int i=0; i<size; ++i)
        indices.push_back(-1);
}

int BinaryHeap::size() {
    return heap.size();
}

// insert element into heap and maintain heap structure
void BinaryHeap::insert(pair<int, int> element) {
    indices[element.second] = heap.size();
    heap.push_back(element);
    heapify_up(heap.size() - 1);
}

// remove minimal element from heap
void BinaryHeap::delete_min() {
    if (heap.size() == 0) {
        return;
    }
    indices[heap[0].second] = -1;   // maintain indices map
    heap[0] = heap[heap.size() - 1];
    heap.pop_back();
    heapify_down(0);
}

// return (but do not remove) minimal element
pair<int, int> BinaryHeap::extract_min() {
    if (heap.size() == 0) {
        return make_pair(-1, -1); // heap is empty
    } else
        return heap.front();
}

// decrease key of existing element, necessary for Prim's algorithm
void BinaryHeap::decrease_key(int value, int new_key) {
    int index = indices[value];
    heap[index] = make_pair(new_key, value);
    heapify_up(index);
}

// ==================================================
// private functions for maintaining heap structure

void BinaryHeap::heapify_down(int in) {
    int child = left(in);
    int child1 = right(in);
    if (child >= 0 && child1 >= 0 && heap[child].first > heap[child1].first) {
        child = child1;
    }
    if (child > 0) {
        swap(in, child);
        heapify_down(child);
    }
}

void BinaryHeap::heapify_up(int in) {
    if (in >= 0 && parent(in) >= 0 && heap[parent(in)].first > heap[in].first) {
        swap(in, parent(in));
        heapify_up(parent(in));
    }
}

void BinaryHeap::swap(int a, int b) {
    // maintain indices map
    indices[heap[a].second] = b;
    indices[heap[b].second] = a;
    // swap elements in heap structure
    pair<int, int> temp = heap[a];
    heap[a] = heap[b];
    heap[b] = temp;
}

int BinaryHeap::left(int parent) {
    int l = 2 * parent + 1;
    if (l < heap.size())
        return l;
    else
        return -1;
}

int BinaryHeap::right(int parent) {
    int r = 2 * parent + 2;
    if (r < heap.size())
        return r;
    else
        return -1;
}

int BinaryHeap::parent(int child) {
    int p = (child - 1)/2;
    if (child == 0)
        return -1;
    else
        return p;
}

// ==================================================
// print heap for debugging

void BinaryHeap::print_heap() {
    cout << "Heap: ";
    for (int i=0; i < heap.size(); ++i)
        cout << heap[i].first << "," << heap[i].second << "  ";
    cout << endl;
    
    cout << "Indices: ";
    for (int i=0; i<indices.size(); ++i)
        cout << indices[i] << " ";
    cout << endl;
}
