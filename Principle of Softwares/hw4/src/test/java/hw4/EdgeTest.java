package hw4;
import static org.junit.Assert.*;
import org.junit.Test;

public class EdgeTest {
    Edge e = new Edge("start", "end", "weight");
    @Test
    public void TestConstructor() {
        boolean exception = false;
        try {
            Edge e1 = new Edge("start","end","weight");
            Edge e2 = new Edge("A", "B");
        }
        catch (Exception e){
            exception = true;
        }
        assertFalse(exception);
    }
    @Test
    public void TestGetStart(){
        assertEquals("start", e.getStart());
    }
    @Test
    public void TestGetEnd(){
        assertEquals("end", e.getEnd());
    }
    @Test
    public void TestGetWeight(){
        assertEquals("weight", e.getWeight());
    }
    @Test
    public void TestSetWeight(){
        e.setWeight("NewWeight");
        assertEquals("NewWeight", e.getWeight());
    }

}
