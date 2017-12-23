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

public class SynchronizedCounter {
    public final class Counter {
	public synchronized long getValue() {
	    return value; 
	}
	public synchronized void increment() {
	    ++value;
	}
	private long value = 0;
    }
}


class Timer implements Runnable {
	SynchronizedCounter counter;
	Timer(SynchronizedCounter counter) {
		this.counter = counter;
	}
	
	public void run() {
		while(true) {
			try { Thread.sleep(1000);} catch (InterruptedException e) {}
			counter.increment();
			synchronized(counter) { counter.notifyAll(); }
			System.out.print(counter.count + " ");
		}
	}
}

class FirstMessage implements Runnable {
	SynchronizedCounter counter;
	FirstMessage(SynchronizedCounter counter) {
		this.counter = counter;
	}
	
	public void run() {
		while(true) {
			synchronized(counter) {
				try { counter.wait(); } catch (InterruptedException e) {}
				if(counter.getValue() % 7 == 0)
					System.out.println("\n7 second message");
			}
		}
	}
}

class SecondMessage implements Runnable {
	SynchronizedCounter counter;
	SecondMessage(SynchronizedCounter counter) {
		this.counter = counter;
	}
	
	public void run() {
		while(true) {
			synchronized(counter) {
				try { counter.wait(); } catch (InterruptedException e) {}
				if(counter.getValue() % 15 == 0)
					System.out.println("\n15 second message");
			}
		}
	}
}

public class Main2b {
	public static void main (String[] args) {
		SynchronizedCounter counter = new SynchronizedCounter();
		
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