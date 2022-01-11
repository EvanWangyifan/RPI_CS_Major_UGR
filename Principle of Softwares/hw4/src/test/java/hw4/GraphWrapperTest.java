package hw4;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import org.junit.Test;
import java.util.Iterator;
import static org.junit.Assert.*;

public class GraphWrapperTest {
    private GraphWrapper g1 = new GraphWrapper();
    private GraphWrapper g2 = new GraphWrapper();
    private GraphWrapper g3 = new GraphWrapper();
    private GraphWrapper g4 = new GraphWrapper();
    private GraphWrapper g5 = new GraphWrapper();
    private GraphWrapper g6 = new GraphWrapper();
    private GraphWrapper g7 = new GraphWrapper();
    private GraphWrapper g8 = new GraphWrapper();
    private GraphWrapper g9 = new GraphWrapper();

    @Test
    public void AddNullNode() {
        boolean exception = false;
        try {
            g1.addNode(null);
        }
        catch(Exception e) {
            exception = true;
        }
        assertTrue(exception);
    }
    @Test
    public void TestAddNode() {
        boolean exception = true;
        try {
            g1.addNode("A");
        }
        catch(Exception e) {
            exception = false;
        }
        assertTrue(exception);
    }

    @Test
    public void AddSameNode() {
        boolean exception = false;
        try {
            g2.addNode("Node");
            g2.addNode("Node");
        }
        catch(Exception e) {
            exception = true;
        }
        assertTrue(exception);
    }
    @Test
    public void TestAddEdge() {
        boolean exception=false;
        g3.addNode("Node1");
        g3.addNode("Node2");
        try {
            g4.addEdge("Node1", "Node2", "100");
        }
        catch(Exception e) {
            exception = true;
        }
        assertFalse(exception);
    }
    @Test
    public void TestListOneNode() {
        g6.addNode("Node");
        assertEquals("Node",g6.listNodes().next());
    }
    @Test
    public void TestListTwoNode() {
        g7.addNode("Node1");
        g7.addNode("Node2");
        Iterator<String> list = g7.listNodes();
        assertEquals("Node1",list.next());
        list.remove();
        assertEquals("Node2",list.next());
    }
    @Test
    public void TestListChildren() {
        g8.addNode("A");
        g8.addNode("B");
        g8.addNode("C");
        g8.addEdge("A", "B", "1");
        g8.addEdge("C", "A", "2");
        g8.addEdge("B", "C", "3");
        assertEquals("B(1)", g8.listChildren("A").next());
        assertEquals("A(2)", g8.listChildren("C").next());
        assertEquals("C(3)", g8.listChildren("B").next());
    }
    @Test
    public void TestTwoSameEdge() {
        boolean exception = false;
        try {
            g9.addNode("Node1");
            g9.addNode("Node2");
            g9.addEdge("Node1","Node2" ,"1");
            g9.addEdge("Node1","Node2" ,"1");
        }
        catch(Exception e) {
            exception = true;
        }
        assertFalse(exception);
    }
}