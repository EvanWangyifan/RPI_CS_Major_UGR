package hw5;

import hw4.GraphWrapper;
import java.util.*;
import java.io.*;

public class MarvelPaths {
    // The graph to be filled
    private GraphWrapper graph;
    // Constructor of a new MarvelPath
    public MarvelPaths() { graph = new GraphWrapper(); }

    /**
     * Create a new graph using the data obtained
     * @param filename name of the file to read from
     * @throws IllegalArgumentException if any of the parameter is null
     * @effect build graph with obtained csv data
     * @modify graph
     */
    public void createNewGraph(String filename){
        if (filename==null) {
            throw new IllegalArgumentException("parameter is null!");
        }
        Set<String> allChar = new HashSet<String>();
        HashMap<String,Set<String>> charInBook = new HashMap<>();
        try {
            MarvelParser.readData(filename, charInBook, allChar);
        }
        catch(IOException e) {
            throw new IllegalArgumentException("Invalid filename");
        }
        for (String c: allChar){
            graph.addNode(c);
        }
        for (String book: charInBook.keySet()){
            for (String c1: charInBook.get(book)){
                for(String c2: charInBook.get(book)){
                    if (c1.equals(c2)){
                        continue;
                    }
                    graph.addEdge(c1, c2, book);
                }
            }
        }
    }

    /**
     * BFS to find the shortest path between two nodes.
     * @param node1 name of first node
     * @param node2 name of second node
     * @return The formatted output string with highest alphabetical order
     */
    public String findPath(String node1, String node2){
        StringBuffer output = new StringBuffer();
        Iterator<String> allNodes = graph.listNodes();
        boolean Node1Exist=false;
        boolean Node2Exist=false;
        while(allNodes.hasNext()) {
            String node=allNodes.next();
            if(node1.equals(node)){
                Node1Exist=true;
            }
            if(node2.equals(node)) {
                Node2Exist=true;
            }
        }
        if(Node1Exist){
            if (!Node2Exist) { return "unknown character " + node2 + "\n"; }
        }
        else{
            if (Node2Exist){ return "unknown character " + node1 + "\n"; }
            else{
                if (!node1.equals(node2)){ return "unknown character " + node1 + "\n" + "unknown character " + node2 + "\n"; }
                else{ return "unknown character " + node1 + "\n"; }
            }
        }
        LinkedList<String> Q =new LinkedList<>();
        HashMap<String, ArrayList<String>> M =new HashMap<>();
        Q.add(node1);
        ArrayList<String> empty = new ArrayList<String>();
        M.put(node1, empty);
        output.append("path from " + node1 + " to " + node2 + ":" + "\n");
        while(Q.size()!=0) {
            String temp = Q.poll();
            Iterator<String> neighbors = graph.listChildren(temp);
            if(node2.equals(temp)){
                for (String s: M.get(temp)){
                    output.append(s+"\n");
                }
                return output.toString();
            }
            if(neighbors!=null) {
                while(neighbors.hasNext()) {
                    String target = neighbors.next();
                    String book=target.substring(target.indexOf("(")+1,target.indexOf(")"));
                    String char2 = target.substring(0, target.indexOf("("));
                    if(!M.containsKey(char2)) {
                        ArrayList<String> visits=new ArrayList<String>();
                        for (String visit: M.get(temp)){
                            visits.add(visit);
                        }
                        Q.add(char2);
                        M.put(char2,visits);
                        visits.add(temp+" to "+char2+" via "+book);
                    }
                }
            }
        }
        return output.toString()+"no path found\n";
    }
}