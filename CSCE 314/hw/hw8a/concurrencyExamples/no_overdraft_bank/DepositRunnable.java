public class DepositRunnable implements Runnable 
{
    public DepositRunnable(BankAccount acc, double anAmount, int aCount) {
	account = acc; amount = anAmount; count = aCount;
    }
    public void run() {
	try 
	    {
		for (int i=0; i<count; i++)
		    {
			account.deposit(amount);
			Thread.sleep(DELAY);
		    }
	    }
	catch (InterruptedException exception) {}
    }
    private static final int DELAY = 1;
    private BankAccount account;
    private double amount;
    private int count;
}