//Assignment 6, CSCE 314
//Section: 502
//Student Name: Taylor Stephenson
//UIN: 120005093
//Help from research online and discussion with other students.

import java.util.*;


class Main1 {
	static void pause(long n) {
		try { Thread.sleep(n); } catch (InterruptedException e) {}
	}
	public static void main (String[] args) {
		final String bond = "Bond";
		final String blofeld = "Blofeld";
		final String osato = "Mr. Osato";
		final PostBox pBond = new PostBox(bond);
		final PostBox pBlofeld = new PostBox(blofeld, pBond);
		final PostBox pOsato = new PostBox(osato, pBond);
		
		// send out some messages on another thread
		new Thread( new Runnable() {
			public void run() {
				pBond.send(blofeld, "Yes, this is my second life"); pause(1000);
				pBlofeld.send(bond, "You only live twice, Mr. Bond."); pause(500);
				String msg = "I gave Number 11 the strictest orders to eliminate him.";
				pOsato.send(blofeld, msg); pause(2000);
				pOsato.send(bond, msg);
				for (int i=0; i<20; ++i) pOsato.send(bond, "flooding the message queue...");
			}
		}).start();
 
		PostBox[] boxes = { pBond, pBlofeld, pOsato };
		long startTime = System.currentTimeMillis();
		
		// poll for messages in a loop for 5 secs
		while (true) {
			for (PostBox box : boxes) {
				for (String m : box.retrieve()) System.out.println(m);
			}
			if (System.currentTimeMillis() - startTime > 5000) break;
		}
		
		// stop each mailbox
		for(PostBox box : boxes) {
			box.stop();
		}
	} // end of main()
} // end of Main1



class PostBox implements Runnable{
	
	static final int MAX_SIZE = 10;
	private final Object lock;
	
	class Message{
		
		String sender;
		String recipient;
		String msg;
		Message(String sender, String recipient, String msg){
			
			this.sender = sender;
			this.recipient = recipient;
			this.msg = msg;
		}
		
		public String read(){
			
			return "From " + sender + " To " + recipient + ": " + msg;
		}
	}
	
	private final LinkedList<Message> messages;
	private LinkedList<Message> myMessages;
	private String myId;
	private boolean stop = false;
	
	public PostBox(String myId){
		
		messages = new LinkedList<Message>();
		this.myId = myId;
		this.myMessages = new LinkedList<Message>();
		new Thread(this).start();
		this.lock = new Object();
	}
	
	public PostBox(String myId, PostBox p){
		
		this.myId = myId;
		this.messages = p.messages;
		this.myMessages = new LinkedList<Message>();
		new Thread(this).start();
		this.lock = p.lock;
	}
	
	public String getId(){
		
		return myId;
	}
	
	public void stop(){
		
		//make it so that this Runnable will stop
		stop = true;
	}
	
	public void send(String recipient, String msg){
		
		//add a message to the shared message queue
		
		//adds a message to shared queue
		synchronized(lock){
			
			messages.add(new Message(myId,recipient,msg));
		}
	}
	
	public List<String> retrieve(){
		
		//return the contents of myMessages
		//and empty myMessages
		
		//synchronized so run() isn't called between lines (1) and (2)
		synchronized(lock){
			
			LinkedList<String> msgs = new LinkedList<String>();
			
			for(Message m : myMessages){ //add contents of myMessages (1)
				
				msgs.add(m.read());
			}
			
			myMessages.clear(); //empty myMessages (2)
			return msgs; // return list of messages as strings
		}
	}
	
	
	
	public void run(){
		
		//looop forever
		//	1.	approximately once ever second move all messages
		//		addressed to this post box from the shared message
		//		queue to the private myMessages queue
		//	2.	also approximately once every second, if the message
		//		queue has more than MAX_SIZE messages, delete oldest messages
		//		so that size is at most MAX_SIZE
		
		try{
			
			while(!stop){
				
				long StartTime = System.currentTimeMillis();
				
				synchronized(lock){ // synchronized so a message isn't added to messages while moving
									// messages to myMessages or deleting messages
									// makes sure the size doesn't change while messages are deleted
					
					int x = messages.size() - MAX_SIZE;
					
					if(x>0){ //if greater than max size delete oldest messages
						
						for(int i=0; i < x; i++){
							
							messages.remove();
						}
					}
					
					for(int i = 0; i < messages.size(); i++){
						
						Message m = messages.get(i);
						if(m.recipient == myId){
							
							myMessages.add(messages.remove(i));
							i--;
						}
					}		
				}
				
				Thread.sleep(1000 - (System.currentTimeMillis() - StartTime));
			}
		}
		
		catch (InterruptedException e){}
	}
}