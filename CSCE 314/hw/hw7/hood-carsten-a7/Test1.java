// Assignment p7, CSCE-314
// Section: 502
// Student Name: CARSTEN HOOD
// UIN: 922009787

public final class Test1 {

    public static void main(String[] args) {
    	
    	Node<Integer> node = null;
        Node<Integer> last = null;
        for (int i = 5; i > 0; i--) {
        	node = new Node<Integer>(new Integer(i), last);
        	last = node;
        }
        
        // test list
        print(node);
        sum(node);
	} 
    
    public static void print(Node<Integer> node) {
    	String str = "[";
    	if (node != null) {
    		for (Integer val : node)
        		str += val + ",";
    		str = str.substring(0, str.length()-1); // remove last comma
    	}
    	
    	System.out.println("print(): " + str + "]");
	}
    
    public static void sum(Node<Integer> node) {
    	Integer sum = new Integer(0);
    	if (node != null) {
        	for (Integer val : node)
        		sum += val;
    	}
    	System.out.println("sum(): " + sum);
	} 
}