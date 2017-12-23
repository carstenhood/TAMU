import java.util.Date;

public class GreetingRunnable implements Runnable
{ 
    public GreetingRunnable(String aGreeting) {
        greeting = aGreeting;
    }

    public void run () {
	try {
		for (int i=0; i<REPETITIONS; i++) 
		    { 
			Date now = new Date();
			System.out.println(now + " " + greeting);
			Thread.sleep(DELAY);
		    }
	    } 
	catch (InterruptedException exception)
	    {
		// A sleeping thread may be interrupted to be terminated, 
		// which raises an exception		
	    }
    }
    private String greeting;
    
    private static final int REPETITIONS = 10;
    private static final int DELAY = 100;


    public static void main(String[] args) 
    {
	Runnable r1 = new GreetingRunnable("Hi!");
	Runnable r2 = new GreetingRunnable("Bye!");
	Thread t1 = new Thread(r1);
	Thread t2 = new Thread(r2);
	t1.start();
	t2.start();
    }
}

	    
	    
		
		
