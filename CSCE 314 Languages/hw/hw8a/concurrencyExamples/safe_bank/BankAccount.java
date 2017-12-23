import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Condition;

public class BankAccount 
{
    public BankAccount() { 
		balance = 0; 
		balanceChangeLock = new ReentrantLock(); 
    }
    public void deposit(double amount) {
		balanceChangeLock.lock();
		try {
		    System.out.println("Depositing " + amount);
		    double nb = balance + amount;
		    System.out.println("New balance is " + nb);
		    balance = nb;
		} finally {
		    balanceChangeLock.unlock();
		}
    }
    public void withdraw(double amount) 
      throws InterruptedException // needed because await puts thread to sleep
    {
		balanceChangeLock.lock();
		try {
		    System.out.println("Withdrawing " + amount);
		    double nb = balance - amount;
		    System.out.println("New balance is " + nb);
		    balance = nb;
		} finally {
		    balanceChangeLock.unlock();
		}
    }
    public double getBalance() { return balance; }
    private double balance;
    private Lock balanceChangeLock;
}
