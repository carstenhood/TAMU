// Assignment p7, CSCE-314
// Section: 502
// Student Name: CARSTEN HOOD
// UIN: 922009787

import java.util.Iterator;

class LinkedList<T> implements Iterable<T> {
	
	private Node<T> head;
	private Node<T> tail;
	private int size;

	public LinkedList() {
	    head = null;
	    tail = null;
	}
	
	public LinkedList(Iterable<T> iterable) {
    	Node<T> node = null;
    	Node<T> prev = null;
		for (T v : iterable) {
			node = new Node<T>(v, null);
    		if (prev == null) {
    			head = node;
    		} else {
    			prev.next = node;
    		}
    		prev = node;
    	}
		tail = node.next;
	}
	
	@Override
    public Iterator<T> iterator() {
		return new Iterator<T>() {
        	private Node<T> node = head;

    		@Override
            public boolean hasNext() {
    			return node != tail;
            }

            @Override
            public T next() {
        		T v = node.v;
        	    node = node.next;
        	    return v;
            }
        };
    }
	
	public LinkedList<T> reverse() {
		Node<T> node = null;
		Node<T> next;
		while (head != null) {
	        next = head.next;
	        head.next = node;
	        node = head;
	        head = next;
		}
		tail = head;
		head = node;
		return this;		
	}
	
	public String toString() {
    	String str = "[";
    	for (T val : this)
    		str += val + ",";
    	if (str.length() > 1)
    		str = str.substring(0, str.length()-1); // remove last comma
    	return str + "]";
	}
}