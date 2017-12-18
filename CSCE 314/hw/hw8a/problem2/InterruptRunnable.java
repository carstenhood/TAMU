import java.util.Date;

public class InterruptRunnable implements Runnable
{ 
  public InterruptRunnable(String aGreeting) {
    greeting = aGreeting;
  }

  public void run () {
    try 
    {
        for (int i=0; i<REPETITIONS; i++) 
        { 
	   Date now = new Date();
	   if (!Thread.interrupted()) { 
              System.out.println(now + " " + greeting);
	   } else {
              System.out.println("Interrupted");
              // this occurs if interrupt notification received
              // while thread active
              throw new InterruptedException();
              // idiomatic: allows cleanup to be done in one place
	   }
           //Thread.sleep(DELAY);
	}
    } 
    catch (InterruptedException exception)
    {
        System.out.println(Thread.currentThread().getName() + ":" + 
                           Thread.currentThread().getState());
	System.out.println("Interrupted and ousted");
	// this occurs if interrupt notification received while
	// thread sleeping
    }
  }
  private String greeting;
    
  private static final int REPETITIONS = 1000;
  private static final int DELAY = 10;


  public static void main(String[] args) {
    InterruptRunnable r1 = new InterruptRunnable("Hi!");
    InterruptRunnable r2 = new InterruptRunnable("Bye!");
    Thread t1 = new Thread(r1);
    Thread t2 = new Thread(r2);
    t1.start();
    t2.start();	
    try {
        Thread.sleep(100);
    } 
    catch (InterruptedException exception) {}
    System.out.println("main: " + Thread.currentThread().getState());
    t1.interrupt();
    t2.interrupt();
  }
}

