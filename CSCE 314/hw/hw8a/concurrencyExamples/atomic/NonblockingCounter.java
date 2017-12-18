import java.util.concurrent.atomic.AtomicInteger;

public class NonblockingCounter {
  private AtomicInteger value;

  public int getValue() {
    return value.get();
  }

  public void increment() {
    int v;
    do {
      v = value.get();
    }
    while (!value.compareAndSet(v, v + 1));
  }
}
