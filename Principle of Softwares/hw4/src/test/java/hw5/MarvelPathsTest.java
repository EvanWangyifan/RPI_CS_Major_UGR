package hw5;
import static org.junit.Assert.*;
import org.junit.Test;

public class MarvelPathsTest{
    @Test
    public void TestConstructor() {
        boolean exception=false;
        try{
            MarvelPaths path1 = new MarvelPaths();
        }
        catch(Exception e) {
            exception=true;
        }
        assertFalse(exception);
    }
    @Test
    public void TestInput() {
        boolean exception = false;
        MarvelPaths path2=new MarvelPaths();
        try {
            path2.createNewGraph("data/marvel.csv");
        }
        catch(Exception e) {
            exception= true;
        }
        assertFalse(exception);
    }
    @Test
    public void TestHavePath() {
        MarvelPaths path3=new MarvelPaths();
        path3.createNewGraph("data/HavePathTest.csv");
        assertEquals("path from B to C:"+"\n"+"B to C via 2\n",path3.findPath("B","C"));
    }
    @Test
    public void TestNoPath() {
        MarvelPaths path4=new MarvelPaths();
        path4.createNewGraph("data/NoPathTest.csv");
        assertEquals("path from A to C:"+"\n"+"no path found\n",path4.findPath("A","C"));
    }
    @Test
    public void TestWrongChar() {
        MarvelPaths path5=new MarvelPaths();
        path5.createNewGraph("data/marvel.csv");
        assertEquals("unknown character "+"NoBody"+"\n",path5.findPath("NoBody", "HAWK"));
        path5.createNewGraph("data/marvel.csv");
        assertEquals("unknown character "+"NoBody"+"\n",path5.findPath("HAWK", "NoBody"));
        path5.createNewGraph("data/marvel.csv");
        assertEquals("unknown character "+"NoBody1"+"\n"+"unknown character "+"NoBody2"+"\n",path5.findPath("NoBody1", "NoBody2"));
    }
    @Test
    public void TestNullChar() {
        boolean exception = false;
        MarvelPaths path6=new MarvelPaths();
        try{
            path6.findPath(null, null);
        }
        catch (Exception e){
            exception = true;
        }
        assertTrue(exception);
    }
    @Test
    public void TestSelfLoop() {
        MarvelPaths path7=new MarvelPaths();
        path7.createNewGraph("data/marvel.csv");
        assertEquals("path from "+"HAWK"+" to " +"HAWK"+":"+"\n",path7.findPath("HAWK","HAWK"));
    }
    @Test
    public void TestInvalidFile() {
        boolean exception=false;
        MarvelPaths path8=new MarvelPaths();
        try {
            path8.createNewGraph("InvalidFilename");
        }
        catch(Exception e) {
            exception=true;
        }
        assertTrue(exception);
    }
}