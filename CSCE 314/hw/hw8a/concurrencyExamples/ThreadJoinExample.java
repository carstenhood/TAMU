import java.util.Vector;

/*  Example code for the Thread.join() method      *
 *  Written by Hyunyoung Lee for CSCE 314 students */

public class ThreadJoinExample {
  private Vector<Integer> results = new Vector<Integer>();

  ThreadJoinExample(int numThreads) {
    // Create an array of numThreads-many Threads
    Thread[] threads = new Thread[numThreads];
    // In a for loop create the threads with the loop index as 
    //    each thread task number (e.g., which row of the input array it is 
    //    assigned to)
    for (int i = 0; i < threads.length; i++) {
       threads[i] = new ThreadJoinExample.maxOf2DArray(i);
       threads[i].start();
    }
    // In a for loop, threads[i].join();  which will wait until every
    //     thread started in the previous step to complete its run method
    for (int i = 0; i < threads.length; i++) {
       try {
          threads[i].join();
       } catch (InterruptedException e) {}
    }
  }

  class maxOf2DArray extends Thread {
    private int id;
    maxOf2DArray(int i) { id = i; }
    public void run() {
       results.add(id); // simply record its id to the results vector
    }
  }
       
  public static void main(String[] args) {
   // For now, pass the number of rows as a command line argument
   int row = Integer.parseInt(args[0]); 
   ThreadJoinExample tJoin = new ThreadJoinExample(row);
   System.out.println(tJoin.results);
  }
}
