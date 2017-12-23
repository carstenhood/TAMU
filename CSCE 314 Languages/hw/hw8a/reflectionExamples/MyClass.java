import java.lang.reflect.*; // for reflection operations
import java.util.*;
import javax.swing.*;
import static java.lang.System.out;
import static java.lang.System.err;

public class MyClass {
  private String aSecret() {
    return ("Here is the secret!");
  }
  public void work(int i, String s) {
    out.println("MyClass.work: " + i + " " + s);
  }
  public MyClass(String s) {
    out.println(s);
  }
  public MyClass() {
    out.println("1: "+aSecret());

    // class Outer as inner class of MyClass
    class Outer1 {
       public void show() {
         // inner class of class Outer
         class Inner {
            public void show() {
              out.print("2: "+getClass().getName() + " .. inner in .. ");
              out.println(getClass().getEnclosingClass());    
            }
         }
         out.print("3: "+getClass().getName() + " __ inner in __ ");
         out.println(getClass().getEnclosingClass());
         // invoke inner class show() function
         Inner i = new Inner();
         i.show();
      }
    }
    // invoke outer class show() function
    Outer1 o1 = new Outer1();
    o1.show();
    class Outer2 {
       public void show() { 
         out.print("4: "+getClass().getName() + " inner of "); 
         out.println(getClass().getEnclosingClass().getName());
         out.print("5: "+getClass().getCanonicalName() + " inner of "); 
         out.println(getClass().getEnclosingClass().getCanonicalName());
         out.print("6: "+getClass().getSimpleName() + " inner of "); 
         out.println(getClass().getEnclosingClass().getSimpleName());
       } 
    }
    Outer2 o2 = new Outer2();
    o2.show();
  } // end of MyClass()

  public static void main(String[] args) {
    // test getClass() and class
    MyClass a = new MyClass();
    Object b = new Object();
    Class c = b.getClass();
    out.println("Object class is " + c);
    c = System.console().getClass();
    out.println("Console class is " + c);

    c = boolean.class;
    out.println("boolean type is " + c);

    out.println("---1");
    // test forName()
    Class cls = null;
    try {
      cls = Class.forName("MyClass");
    } catch (ClassNotFoundException e) {
      err.println(e);
    }
    out.println("MyClass class is " + cls);

    // test other Class methods
    out.println("superclass is: " + 
                 cls.getSuperclass()); 
              // javax.swing.JButton.class.getSuperclass());
    Method[] methods = cls.getDeclaredMethods();
    for (Method m : methods)
    //    if (Modifier.isPublic(m.getModifiers()))
           out.println("   " + m); 

    // constructor(s)
    Constructor[] constructs = cls.getConstructors();
    for (Constructor co : constructs)
        out.println("constructor: " + co);

    out.println("---2");
    Class<?>[] ac = Character.class.getClasses(); // including inherited
    for (Class aa : ac)
        out.println("array of chars: " + aa);
    ac = Character.class.getDeclaredClasses(); // explicitly declared in this
    for (Class aa : ac)
        out.println("declared class: " + aa);
    out.println("enclosing class: " + Character.class.getEnclosingClass());

    // to test type erasure
    List<Integer> vi = new LinkedList<Integer>();
    List<String> vs = new LinkedList<String>();
    out.println("vi is "+vi.getClass().getName());
    out.println("vi is "+vi.getClass().getCanonicalName());
    out.println("vi is "+vi.getClass().getSimpleName());
    out.println("vs is "+vs.getClass());
    if (vi.getClass() == vs.getClass())
       out.println("vi and vs are the same!");
    else
       out.println("vi and vs are different!");
 
  }
}

