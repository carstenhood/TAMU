// Assignment p7, CSCE-314
// Section: 502
// Student Name: CARSTEN HOOD
// UIN: 922009787

import java.util.Arrays;

public final class Test2 {
    public static void main(String[] args) {
    	 LinkedList<Integer> emptyList = new LinkedList<Integer>();
         LinkedList<Integer> list =
             new LinkedList<Integer>(Arrays.asList(1, 2, 3, 4, 5, 6));
         
         System.out.println("original empty list: " + emptyList);
         System.out.println("reversed empty list: " + emptyList.reverse());
         
         System.out.println("original list: " + list);
         int sum = 0;
         for (int e : list)
        	 sum += e;
         System.out.println("sum orig list: " + sum);
         
         System.out.println("reversed list: " + list.reverse());
         
         sum = 0;
         for (int e : list)
        	 sum += e;
         System.out.println("sum rev. list: " + sum);
	} 
}