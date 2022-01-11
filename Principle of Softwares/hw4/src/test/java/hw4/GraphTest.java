package hw4;

import static org.junit.Assert.*;
import org.junit.Test;

public class GraphTest {
    private Graph g1 = new Graph();
    private Graph g2 = new Graph();
    private Graph g3 = new Graph();
    private Graph g4 = new Graph();
    private Graph g5 = new Graph();
    private Graph g6 = new Graph();
    private Graph g7 = new Graph();

    @Test
    public void TestConstructor() {
        boolean exception=false;
        try {
            Graph test0=new Graph();
        }
        catch(Exception e) {
            exception=true;
        }
        assertFalse(exception);
    }
    @Test
    public void TestNullNode() {
        boolean exception=false;
        try {
            g1.addNode(null);
        }
        catch(Exception e) {
            exception=true;
        }
        assertTrue(exception);
    }
    @Test
    public void TestAddNode() {
        g2.addNode("Node");
        assertEquals("Node",g2.nodeItr().next());
    }
    @Test
    public void TestSameNode() {
        boolean exception=false;
        try {
            g3.addNode("Node");
            g3.addNode("Node");
        }
        catch(Exception e) {
            exception=true;
        }
        assertTrue(exception);
    }
    @Test
    public void TestAddEdge() {
        g4.addNode("Node1");
        g4.addNode("Node2");
        g4.addEdge(new Edge("Node1", "Node2", "100"));
        assertEquals("Node2(100)",g4.childrenItr("Node1").next());
    }
    @Test
    public void TestNullEdge() {
        boolean exception=false;
        try {
            g5.addEdge(new Edge("Node1","Node2" ,null));
        }
        catch(Exception e) {
            exception=true;
        }
        assertFalse(exception);
    }
    @Test
    public void TestTwoSameEdge(){
        boolean exception=false;
        try {
            g6.addNode("Node1");
            g6.addNode("Node2");
            g6.addEdge(new Edge("Node1","Node2" ,"1"));
            g6.addEdge(new Edge("Node1","Node2" ,"1"));
        }
        catch(Exception e) {
            exception = true;
        }
        assertFalse(exception);
    }
    @Test
    public void TestNodeItr(){
        g7.addNode("D");
        g7.addNode("B");
        g7.addNode("C");
        g7.addNode("A");
        assertEquals("A",g7.nodeItr().next());
    }
}