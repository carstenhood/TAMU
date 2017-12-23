//import java.util.Date;

public class PingPong implements Runnable
{ 
    private static final int REPETITIONS = 10;

    private String pingOrPong;
    private int delay = 100;

    public PingPong(String whatToSay, int delayTime) {
	pingOrPong = whatToSay;
        delay = delayTime;
    }

    public void run () {
	try {
	for (int i=0; i<REPETITIONS; i++) { 
             System.out.println(Thread.currentThread().getName() + " says " 
                                + pingOrPong);
             System.out.println(Thread.currentThread().getName() + ":" 
                                + Thread.currentThread().getState());
             Thread.sleep(delay);
	}
	} catch (InterruptedException e) {
             // A sleeping thread may be interrupted to be terminated, 
             // which raises an exception		
             return; // end this thread
	}
    }
  
    public static void main(String[] args) {
        System.out.println(Thread.currentThread().getName());
        PingPong r1 = new PingPong("PING", 60); 
        PingPong r2 = new PingPong("pong", 100); 
        Thread t1 = new Thread(r1, "ping");
        System.out.println(t1.getState());
        Thread t2 = new Thread(r2, "pong");
        System.out.println("t2 ... " + t2.getState());
        t1.start();
        System.out.println(t1.getState());
        t2.start();
        System.out.println("t2 ... " + t2.getState());
        System.out.println(t1.getState());
        t1.start(); 
    }
}

