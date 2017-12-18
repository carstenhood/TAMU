// Carsten Hood
// CSCE 221
// hw 1

// 1
// Problem R-5.7 in the text (p. 223). Modify the stack ADT implementation described in Section 5.1.5 (p. 202) to be a generic class through the use of templates. (30 points)
template <typename T>
class LinkedStack {
public:
    LinkedStack();
    int size() const;
    bool empty() const;
    const T& top() const throw(StackEmpty);
    void push(const T& e);
    void pop() throw(StackEmpty);
private:
    SLinkedList<T> S;
    int n;
};

template <typename T>
LinkedStack<T>::LinkedStack()
: S(), n(0) {}

template <typename T>
int LinkedStack<T>::size() const {
    return n;
}

template <typename T>
bool LinkedStack<T>::empty() const {
    return n == 0;
}

template <typename T>
const T& LinkedStack<T>::top() const throw(StackEmpty) {
    if (empty()) throw StackEmpty("Top of empty stack");
        return S.front();
        }

template <typename T>
void LinkedStack<T>::push(const T& e) {
    ++n;
    S.addFront(e);
}

template <typename T>
void LinkedStack<T>::pop() throw(StackEmpty) {
    if (empty()) throw StackEmpty("Pop from empty stack");
        −−n;
    S.removeFront();
}

// 2
// Problem C-5.5 in the text (p. 224). Describe how to implement the stack ADT using two queues. What is the running time of the push() and pop() functions in this case? (30 points)
/*
 We keep track of 2 queues, q1 and q2. Elements would be stored in q1. q2 simply holds elements temporarily during the push() operation.
 When push() is called, q1's elements would be dequeued and enqueued into q2. q2 would hold them temporarily as the new "pushed" element is enqueued on the now empty q1 (putting it at the front of the queue). Then the elements in q2 would be dequeued and enqueued back to q1. The push operation takes O(n) time.
 When pop() is called, we simply dequeue from q1. The element "popped" will be the most recent addition, functioning like a stack. The pop operation takes O(1) time.
 */
 
 
 // 3
 //Design a version of the size() operation of a stack that uses only pop(), push(obj), and isEmpty() operations, and does not use any internal variable of the underlying array (i.e., you cannot use the implementation of size() in the text). (40 points)
 //(a) Express your algorithm using pseudo-code, including comments to explain the various steps.
 
 
 //(b) What is the running time of your size() operation assuming there are N elements in the stack when size() is invoked? Express your answer using asymptotic notion.
 
 
 //(c) Compare the efficiency of this new version of size() with the version in the text that uses the internal variable t. Which one is more efficient?
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
