import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Condition;

public class BankAccount 
{
   public BankAccount() { 
      balance = 0; 
      balanceChangeLock = new ReentrantLock(); 
      sufficientFundsCondition = balanceChangeLock.newCondition();
   }
   public void deposit(double amount) {
      balanceChangeLock.lock();
      try {
         System.out.println("Depositing " + amount);
         double nb = balance + amount;
         System.out.println("New balance is " + nb);
         balance = nb;
         sufficientFundsCondition.signalAll();
      } finally {
         balanceChangeLock.unlock();
      }
   }
   public void withdraw(double amount) 
     throws InterruptedException // needed because await puts thread to sleep
   {
      balanceChangeLock.lock();
      try {
         while (balance < amount) 
	 sufficientFundsCondition.await();
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
   private Condition sufficientFundsCondition;

   public static void main(String args[]) {
      BankAccount account = new BankAccount();
      final double AMOUNT = 100;
      final int REPETITIONS = 1000;

      DepositRunnable d1 = new DepositRunnable(account, AMOUNT, REPETITIONS);
      WithdrawRunnable w1 = new WithdrawRunnable(account, AMOUNT, REPETITIONS);
      DepositRunnable d2 = new DepositRunnable(account, AMOUNT, REPETITIONS);
      WithdrawRunnable w2 = new WithdrawRunnable(account, AMOUNT, REPETITIONS);
      Thread t1 = new Thread(d1);
      Thread t2 = new Thread(w1);
      Thread t3 = new Thread(d2);
      Thread t4 = new Thread(w2);

      t1.start();
      t2.start();
      t3.start();
      t4.start();
   }
}

