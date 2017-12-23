// Assignment p7, CSCE-314
// Section: 502
// Student Name: CARSTEN HOOD
// UIN: 922009787

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
	
	public PostBox(String myId) {
		messages = new LinkedList<Message>();
		this.myId = myId;
	    this.myMessages = new LinkedList<Message>();
	    new Thread(this).start();
	}
	
	public PostBox(String myId, PostBox p) {
	    this.myId = myId;
	    this.messages = p.messages;
	    this.myMessages = new LinkedList<Message>();
	    new Thread(this).start();
	}
	
	public String getId() { return myId; }
	public void stop() {
	    // make it so that this Runnable will stop
	}
 
	public void send(String recipient, String msg) {
		// add a message to the shared message queue
	}
	
	public List<String> retrieve() {
	    // return the contents of myMessages
	    // and empty myMessages
	}
	
	public void run() {
	    // loop forever
	    //   1. approximately once every second move all messages
	    //      addressed to this post box from the shared message
	    //      queue to the private myMessages queue
	    //   2. also approximately once every second, if the message
	    //      queue has more than MAX_SIZE messages, delete oldest messages
	    //      so that size is at most MAX_SIZE
	}
}