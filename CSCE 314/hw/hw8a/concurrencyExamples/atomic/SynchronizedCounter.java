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
	