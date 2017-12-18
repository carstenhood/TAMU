import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Condition;

public class BankAccount {
    public BankAccount() { 
	balance = 0; 
	balanceChangeLock = new ReentrantLock(); 
    }
    public void deposit(double amount) {
	balanceChangeLock.lock();
	try {
	    System.out.println("Depositing " + amount);
	    balance = balance + amount;
	    //System.out.println("New balance is " + nb);
	} finally {
	    balanceChangeLock.unlock();
	}
    }
    public void withdraw(double amount) {
	balanceChangeLock.lock();
	try {
            while (balance < amount) {}
	    //System.out.println("Withdrawing " + amount);
	    //double nb = balance - amount;
	    balance = balance - amount;
	    //System.out.println("New balance is " + nb);
	    //balance = nb;
	} finally {
	    balanceChangeLock.unlock();
	}
    }
    public double getBalance() { return balance; }
    private double balance;
    private Lock balanceChangeLock;

    public static void main(String args[]) {
	BankAccount account = new BankAccount();
	final double AMOUNT = 100;
	final int REPETITIONS = 100;
	
	DepositRunnable d1 = new DepositRunnable(account, AMOUNT, REPETITIONS);
	WithdrawRunnable w1= new WithdrawRunnable(account, AMOUNT, REPETITIONS);
	DepositRunnable d2 = new DepositRunnable(account, AMOUNT, REPETITIONS);
	WithdrawRunnable w2= new WithdrawRunnable(account, AMOUNT, REPETITIONS);
	Thread t1 = new Thread(d1);
	Thread t2 = new Thread(w1);
	Thread t3 = new Thread(d2);
	Thread t4 = new Thread(w2);
        System.out.println("Threads are created.");

	t1.start();
	t2.start();
	t3.start();
	t4.start();
    }
}

