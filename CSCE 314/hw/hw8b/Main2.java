import java.util.*;

class Main2 {
	
	public static void main(String args[]){
		
		int i = 0;
		for(;;){
			
			try{
				
				Thread.sleep(1000);
			}
			catch(Exception e){
				
				System.out.print("Exception : " + e.getMessage());
			}
			
			if((i > 0) && (i % 7) == 0){
				
				System.out.print("\n" + "7 second message" + "\n" + i + " ");
				i++;
			}
			else if((i > 0) && (i % 15) == 0){
				
				System.out.print("\n" + "15 second message" + "\n" + i + " ");
				i++;
			}
			else{
				System.out.print(i + " ");
				i++;
			}
		}
	}		
}