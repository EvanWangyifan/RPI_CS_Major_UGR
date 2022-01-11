package hw6;

import static org.junit.Assert.*;

import hw4.Graph;
import org.junit.Test;

import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.LinkedList;

public class MarvelPaths2Test{
    LinkedList<AbstractMap.SimpleEntry<String, Double>> l = new LinkedList<AbstractMap.SimpleEntry<String, Double>>();
    ArrayList<Integer> havePath = new ArrayList<Integer>();
    @Test
    public void TestConstructor(){
        boolean exception=false;
        try{
            MarvelPaths2 path1 = new MarvelPaths2();
        }
        catch(Exception e) {
            exception=true;
        }
        assertFalse(exception);
    }
    @Test
    public void TestInput() {
        MarvelPaths2 a = new MarvelPaths2();
        a.createNewGraph("data/marvel.csv");
    }
    @Test
    public void TestInvalidFile() {
        MarvelPaths2 c = new MarvelPaths2();
        boolean exception=false;
        try{
            c.createNewGraph("InvalidFileName");
        }
        catch(Exception e) {
            exception=true;
        }
        assertTrue(exception);
    }
    @Test
    public void TestNodeNotFound(){
        Graph<String,Double> g = new Graph<String,Double>();
        MarvelPaths2 d = new MarvelPaths2();
        d.createNewGraph("data/marvel.csv");
        assertTrue(d.findPath("nobody", "CAPTAIN AMERICA",l,havePath,g).equals("unknown character nobody\n"));
        assertTrue(d.findPath("CAPTAIN AMERICA", "nobody",l,havePath,g).equals("unknown character nobody\n"));
        assertTrue(d.findPath("nobody", "nobody",l,havePath,g).equals("unknown character nobody\n"));
    }
    @Test
    public void TestPathNotFound(){
        Graph<String,Double> g = new Graph<String,Double>();
        MarvelPaths2 e = new MarvelPaths2();
        e.createNewGraph("data/test.csv");
        assertTrue(e.findPath("A", "B",l,havePath,g).equals("path from A to B:\n" + "no path found\n"));
    }
    @Test
    public void TestZeroCost(){
        Graph<String,Double> g = new Graph<String,Double>();
        MarvelPaths2 f = new MarvelPaths2();
        f.createNewGraph("data/test.csv");
        assertTrue(f.findPath("A", "A",l,havePath,g).equals("path from A to A:\n" +
                "total cost: 0.000\n"));
    }
    @Test
    public void TestExistPath(){
        MarvelPaths2 b = new MarvelPaths2();
        b.createNewGraph("data/marvel.csv");
        Graph<String,Double> g = b.getGraph();
        String result = b.findPath("CAPTAIN AMERICA", "3-D MAN/CHARLES CHAN",l,havePath,g);
        assertEquals( "path from CAPTAIN AMERICA to 3-D MAN/CHARLES CHAN:" + "\n"
                + "CAPTAIN AMERICA to JONES, RICHARD MILHO" + " with weight 0.013\n"
                + "JONES, RICHARD MILHO to 3-D MAN/CHARLES CHAN with weight 0.250\n"
                + "total cost: 0.263\n", result);
    }
}