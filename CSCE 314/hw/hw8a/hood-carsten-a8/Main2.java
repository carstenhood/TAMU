// Assignment p7, CSCE-314
// Section: 502
// Student Name: CARSTEN HOOD
// UIN: 922009787

/*public class SynchronizedCounter {

    public final class Counter {
	public synchronized long getValue() {
	    return value; 
	}
	public synchronized void increment() {
	    ++value;
	}
	private long value = 0;
    }
}*/

import java.util.*;

class Counter {
	public int value = 0;
	public void increment() {
		++value;
	}
}

class Timer implements Runnable {
	Counter counter;
	Timer(Counter counter) {
		this.counter = counter;
	}
	
	public void run() {
		while (true) {
			try { Thread.sleep(1000); } catch (InterruptedException e) {}
			counter.increment();
			synchronized(counter) { counter.notifyAll(); }
			System.out.print(counter.value + " ");
		}
	}
}

class FirstMessage implements Runnable {
	Counter counter;
	FirstMessage(Counter counter) {
		this.counter = counter;
	}
	
	public void run() {
		while(true) {
			synchronized(counter) {
				try { counter.wait(); } catch (InterruptedException e) {}
				if (counter.value % 7 == 0)
					System.out.println("\n7 second message");
			}
		}
	}
}

class SecondMessage implements Runnable {
	Counter counter;
	SecondMessage(Counter counter) {
		this.counter = counter;
	}
	
	public void run() {
		while(true) {
			synchronized(counter) {
				try { counter.wait(); } catch (InterruptedException e) {}
				if (counter.value % 15 == 0)
					System.out.println("\n15 second message");
			}
		}
	}
}

public class Main2 {
	public static void main (String[] args) {
		Counter counter = new Counter();
		
		Timer timer = new Timer(counter);
		FirstMessage firstMessage = new FirstMessage(counter);
		SecondMessage secondMessage = new SecondMessage(counter);
		
		Thread timerThread = new Thread(timer);
		Thread firstMessageThread = new Thread(firstMessage);
		Thread secondMessageThread = new Thread(secondMessage);
		
		timerThread.start();
		firstMessageThread.start();
		secondMessageThread.start();
	}
}
