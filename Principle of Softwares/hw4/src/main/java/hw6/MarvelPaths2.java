package hw6;

import hw4.*;
import hw5.MarvelParser;
import java.util.*;
import java.io.*;

public class MarvelPaths2 {
    // The graph to be filled
    private Graph<String, Double> graph;

    /**
     * Create a new graph using the data obtained
     * @param filename name of the file to read from
     * @throws IllegalArgumentException if any of the parameter is null
     * @effect build graph with obtained csv data
     * @modify graph
     */
    public void createNewGraph(String filename){
        graph = new Graph<String,Double>();
        if (filename==null) {
            throw new IllegalArgumentException("parameter is null!");
        }
        this.graph = new Graph<String,Double>();
        Set<String> allChar = new HashSet<String>();
        HashMap<String,Set<String>> charInBook = new HashMap<>();
        try {
            MarvelParser.readData(filename, charInBook, allChar);
        }
        catch(IOException e) {
            throw new IllegalArgumentException("Invalid filename");
        }
        // Count weights
        HashMap<Edge<String,Double>,Double> count = new HashMap<Edge<String, Double>,Double>();
        for (String book: charInBook.keySet()){
            for (String c1: charInBook.get(book)){
                for (String c2: charInBook.get(book)){

                    if (c1.equals(c2)) {
                        continue;
                    }
                    Edge<String, Double> e = new Edge<String, Double>(c1, c2, 0.0);
                    if (count.containsKey(e)) {
                        count.replace(e, count.get(e) + 1.0);
                    } else {
                        count.put(e, 1.0);
                    }

                }
            }
        }
        // add nodes
        for (String c: allChar){
            graph.addNode(c);
        }
        // add edges
        for (Edge<String,Double> e: count.keySet()){
            graph.addEdge(new Edge<String, Double>(e.getStart(),e.getEnd(),1/count.get(e)));
        }
    }
    public Graph getGraph(){
        return this.graph;
    }
    /**
     * @param CHAR1
     * @param CHAR2
     * @param result the lLinkedList to store result (passed in with empty)
     * @param havePath an ArrayList indicate if a path exist empty if not exist, size>0 if exist (passed in with empty)
     * @return The shortest path calculated by Dijkstra's algorithm
     */
    public String findPath(String CHAR1, String CHAR2, LinkedList<AbstractMap.SimpleEntry<String, Double>> result, ArrayList<Integer> havePath, Graph<String,Double> g){
        if (CHAR1==null){
            throw new IllegalArgumentException("CHAR1 is null!");
        }
        if (CHAR2==null){
            throw new IllegalArgumentException("CHAR2 is null!");
        }
        // Check for unknown chars & 0 cost
        Iterator<String> allNodes = g.nodeItr();
        boolean Node1Exist=false;
        boolean Node2Exist=false;
        while(allNodes.hasNext()) {
            String node=allNodes.next();
            if(CHAR1.equals(node)){
                Node1Exist=true;
            }
            if(CHAR2.equals(node)) {
                Node2Exist=true;
            }
        }
        if(Node1Exist){
            if (!Node2Exist) { return "unknown character " + CHAR2 + "\n"; }
        }
        else{
            if (Node2Exist){ return "unknown character " + CHAR1 + "\n"; }
            else{
                if (!CHAR1.equals(CHAR2)){ return "unknown character " + CHAR1 + "\n" + "unknown character " + CHAR2 + "\n"; }
                else{ return "unknown character " + CHAR1 + "\n"; }
            }
        }
        if (CHAR1.equals(CHAR2)){
            return "path from "+CHAR1+" to "+CHAR2+":\ntotal cost: 0.000\n";
        }
        //initialization
        String start = CHAR1;
        String dest = CHAR2;
        PriorityQueue<LinkedList<AbstractMap.SimpleEntry<String, Double>>> active = new PriorityQueue<LinkedList<AbstractMap.SimpleEntry<String, Double>>>
                (new Comparator<LinkedList<AbstractMap.SimpleEntry<String, Double>>>() {
                    @Override
                    // Compare the total cost of the paths
                    public int compare(LinkedList<AbstractMap.SimpleEntry<String, Double>> o1, LinkedList<AbstractMap.SimpleEntry<String, Double>> o2) {
                        return o1.getLast().getValue().compareTo(o2.getLast().getValue());
                    }
                });
        HashSet<String> finished = new HashSet<String>();
        LinkedList<AbstractMap.SimpleEntry<String, Double>> temp = new LinkedList<AbstractMap.SimpleEntry<String, Double>>();
        temp.add(new AbstractMap.SimpleEntry<String, Double>(start , 0.0));
        active.add(temp);
        // Start Dijkstra's algorithm
        while(!active.isEmpty()){
            LinkedList<AbstractMap.SimpleEntry<String, Double>> minPath = active.poll();
            String minDest = minPath.getLast().getKey();
            Double cost = minPath.getLast().getValue();
            if(minDest.equals(dest)) {
                // Assign result the shortest path
                Iterator<AbstractMap.SimpleEntry<String, Double>> it = minPath.iterator();
                while (it.hasNext()) {
                    AbstractMap.SimpleEntry<String, Double> element = it.next();
                    result.add(element);
                }
                // show that there is a path
                havePath.add(1);
                return ToString(minPath, CHAR1, CHAR2);
            }
            if(finished.contains(minDest))
                continue;

            Iterator<Edge<String,Double>> itr = g.getEdges(minDest);
            while (itr.hasNext()){
                Edge<String,Double> tmp = itr.next();
                if (!finished.contains(tmp.getEnd())){
                    LinkedList<AbstractMap.SimpleEntry<String, Double>> newPath = new LinkedList<AbstractMap.SimpleEntry<String, Double>>(minPath);
                    newPath.add(new AbstractMap.SimpleEntry<String, Double>(tmp.getEnd(), tmp.getWeight()+cost));
                    active.add(newPath);
                }
            }
            finished.add(minDest);
        }
        return String.format("path from %s to %s:\nno path found\n", CHAR1, CHAR2);
    }


    /**
     * @param path the linkedList path selected
     * @param CHAR1 --- the starting node
     * @param CHAR2 --- the destination node
     * @return the formatted string to be print out
     */
    private String ToString(LinkedList<AbstractMap.SimpleEntry<String, Double>> path, String CHAR1, String CHAR2) {
        StringBuffer output = new StringBuffer();
        output.append("path from " + CHAR1 + " to " + CHAR2 + ":\n");
        Iterator<AbstractMap.SimpleEntry<String, Double>> itr = path.iterator();
        AbstractMap.SimpleEntry<String, Double> aStep = itr.next();
        String previous;
        Double weight = 0.0;
        while (itr.hasNext()){
            previous = aStep.getKey();
            weight = aStep.getValue();
            aStep = itr.next();
            output.append(String.format("%s to %s with weight %.3f\n", previous, aStep.getKey(), aStep.getValue()-weight));
        }
        output.append(String.format("total cost: %.3f\n", weight+aStep.getValue()-weight));
        return output.toString();
    }
}