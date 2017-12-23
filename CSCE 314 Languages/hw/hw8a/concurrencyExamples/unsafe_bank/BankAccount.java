public class BankAccount 
{
   public BankAccount() { balance = 0; }
   public void deposit(double amount) {
      System.out.println("Depositing " + amount);
      double nb = balance + amount;
      System.out.println("New balance is " + nb);
      balance = nb;
   }
   public void withdraw(double amount) {
      System.out.println("Withdrawing " + amount);
      double nb = balance - amount;
      System.out.println("New balance is " + nb);
      balance = nb;
   }
   public double getBalance() { return balance; }
   private double balance;

   public static void main(String args[]) { 
      BankAccount account = new BankAccount();
      final double AMOUNT = 100;
      final int REPETITIONS = 20;

      DepositRunnable d = new DepositRunnable(account, AMOUNT, REPETITIONS);
      WithdrawRunnable w = new WithdrawRunnable(account, AMOUNT, REPETITIONS);
      Thread t1 = new Thread(d);
      Thread t2 = new Thread(w);
      t1.start();
      t2.start();
   }
}

