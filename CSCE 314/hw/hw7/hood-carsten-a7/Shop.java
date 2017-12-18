// Assignment p7, CSCE-314
// Section: 502
// Student Name: CARSTEN HOOD
// UIN: 922009787

import java.util.*;

public class Shop<T> {
   List<T> stock;
   
   public Shop() { 
	   stock = new java.util.LinkedList<T>();
   }
   
   void sell(T item) {
        stock.add(item);
   }
   
   public T buy() {
        return stock.remove(0);
   }

   void sell(Collection<? extends T> items) {
	   for (T e : items)
		   stock.add(e);
   }
   
   void buy(int n, Collection<? super T> items) {
	    for (T e : stock.subList(0, n))
	        items.add(e);
	    for (int i = 0; i < n; ++i) 
	    	stock.remove(0);
	}
}