
// Assignment p7, CSCE-314
// Section: 502
// Student Name: CARSTEN HOOD
// UIN: 922009787

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Lock;
import java.util.*;

class PostBox implements Runnable {
	static final int MAX_SIZE = 10;
	  
	class Message {
		String sender;
		String recipient;
		String msg;
		Message(String sender, String recipient, String msg) {
			this.sender = sender;
			this.recipient = recipient;
			this.msg = msg;
	    }
	}
	
	private final LinkedList<Message> messages;
	private LinkedList<Message> myMessages;
	private String myId;
	private boolean stop = false;
	private final Object lock; 

	public PostBox(String myId) {
		messages = new LinkedList<Message>();
		this.myId = myId;
	    this.myMessages = new LinkedList<Message>();
	    new Thread(this).start();
		this.lock = new Object();
	}
	
	public PostBox(String myId, PostBox p) {
	    this.myId = myId;
	    this.messages = p.messages;
	    this.myMessages = new LinkedList<Message>();
	    new Thread(this).start();
	    this.lock = p.lock;
	}
	
	public String getId() { return myId; }
	public void stop() {
	    // make it so that this Runnable will stop
		stop = true;
	}
 
	public void send(String recipient, String msg) {
		// add a message to the shared message queue
		
		// synchronized to preclude modifying the shared queue
		// while it is being operated on elsewhere
		synchronized (lock) {
    		messages.add(new Message(this.getId(), recipient, msg));
        }
	}
	
	public List<String> retrieve() {
	    // return the contents of myMessages
	    // and empty myMessages
		
		// synchronized to preclude a call of run() between
		// the loop (line 1) and clear call (line 2)
		synchronized (lock) { 
			List<String> formattedMessages = new LinkedList<String>(); 
    		for (Message m : myMessages) { // (1) format received messages
    			String formattedMessage = "From " + m.sender
    					+ " to " + m.recipient + ": " + m.msg;
    			formattedMessages.add(formattedMessage);
    		}
    		myMessages.clear(); // (2) delete all messages
    		return formattedMessages;
        }
	}

	public void run() {
	    // loop forever
	    //   1. approximately once every second move all messages
	    //      addressed to this post box from the shared message
	    //      queue to the private myMessages queue
	    //   2. also approximately once every second, if the message
	    //      queue has more than MAX_SIZE messages, delete oldest messages
	    //      so that size is at most MAX_SIZE
		
		
		try {
			while (!stop) {
				long startTimeMillis = System.currentTimeMillis();

				// synchronized to preclude adding messages to the shared queue
				// and thereby modifying its size while looping through and removing
				// messages from the shared queue
				synchronized (lock) { 
					for (int i = 0; i < messages.size(); i++) {
						Message message = messages.get(i);
						if (message.recipient.equals(this.getId())) {
							myMessages.add(messages.remove(i));
							i--;
						}
					}
					while (messages.size() > MAX_SIZE) {
						messages.removeFirst();
					}
				}
				Thread.sleep(1000 - (System.currentTimeMillis() - startTimeMillis));
			}
		} catch (InterruptedException e) { } 
	}
}