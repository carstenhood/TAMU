// Assignment p7, CSCE-314
// Section: 502
// Student Name: CARSTEN HOOD
// UIN: 922009787

import java.util.Iterator;

public final class Node<T> implements Iterable<T> {
    public final T v;
    public Node<T> next;
    public Node (T val, Node<T> link) { v = val; next = link; }
    
    public Iterator<T> iterator() {
        return new NodeIterator<T>(this);
    }
    
    class NodeIterator<T> implements Iterator<T> {
    	private Node<T> node;
    	
        public NodeIterator (Node<T> n) {
        	node = n;
        }
    	
    	@Override
    	public boolean hasNext() {
    	    return node != null;
    	}

    	@Override
    	public T next() {
    		T v = node.v;
    	    node = node.next;
    	    return v;
    	}
    }
}


