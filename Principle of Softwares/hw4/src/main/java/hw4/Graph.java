package hw4;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Collections;

/**
 * <b>Graph</b> represents an directed graph.
 * It is represented by a HashMap with the name of starting node for keys
 * and ArrayList of Edges get out from the node as values.
 *
 * Example of a directed graph containing node {A, B} and Edge {(A,B),(B,A)}:
 * {A:[Edge(A,B)], B:[Edge(B,A)]}
 */
public class Graph<T extends Comparable<T>, U extends Comparable<U>> {

    /** Holds all ArrayLists of edges mapped by the name of starting node */
    private HashMap<T, ArrayList<Edge<String,U>>> adj_lst;
    /** Holds the number of edges of the graph */
    private int edge_num;

    /**
     * @effects Constructs a new empty Graph
     */
    public Graph(){
        this.adj_lst = new HashMap<T, ArrayList<Edge<String,U>>>();
        this.edge_num = 0;
        checkRep();
    }

    /**
     * Edge Addition operation.
     *
     * @param e The other Edge to be added.
     * @effects add the Edge to the corresponding ArrayList if the key already exist,
     *          or add a new key with the new Edge in the ArrayList if not.
     * @throws IllegalArgumentException if e is null or e has attributes of null
     */
    public void addEdge(Edge<String,U> e){
        if (e == null){
            throw new IllegalArgumentException("e is null!");
        }
        if (e.getStart()==null || e.getEnd()==null || e.getWeight()==null){
            throw new IllegalArgumentException("e has null start/end !");
        }
        //if (!e.getStart().equals(e.getEnd())){
            if (adj_lst.containsKey(e.getStart()) && adj_lst.containsKey(e.getEnd())){
                adj_lst.get(e.getStart()).add(e);
            }
            this.edge_num++;
        //}
        checkRep();
    }

    /**
     * @param node the new node you want to add to the Graph
     * @effects Add the new node to the key of HashMap if the key does not already exist.
     * @throws IllegalArgumentException if node == null
     */
    public void addNode(T node){
        if (node == null){
            throw new IllegalArgumentException("node is null!");
        }
        if (!adj_lst.containsKey(node)){
            ArrayList<Edge<String,U>> arr = new ArrayList<Edge<String,U>>();
            adj_lst.put(node, arr);
        }
        checkRep();
    }

    /**
     * @return the iterator pointing to all starting Nodes that is sorted alphabetically
     */
    public Iterator<T> nodeItr(){
        ArrayList<T> keys = new ArrayList<T>();
        for (T key : adj_lst.keySet()){
            keys.add(key);
        }
        Collections.sort(keys);
        checkRep();
        return keys.iterator();
    }

    /**
     * @param children
     * @effects sort the Edges alphabetically and by weight
     * @modified children
     * @return a sorted ArrayList containing all children
     */
    public ArrayList<String> sortChildren(ArrayList<Edge<String,U>> children){
        ArrayList<String> result = new ArrayList<String>();
        /** Create a HashMap */
        HashMap<String,Edge<String,U>> compare = new HashMap<String,Edge<String,U>>();
        for (Edge<String,U> child : children){
            compare.put(child.getEnd()+child.getWeight().toString(),child);
        }
        /** Sort the keys */
        ArrayList<String> keys = new ArrayList<String>();
        for (String key : compare.keySet()){
            keys.add(key);
        }
        Collections.sort(keys);
        /** Use the sorted keys create a new sorted ArrayList */
        for (String value : keys){
            result.add(compare.get(value).getEnd()+"("+compare.get(value).getWeight()+")");
        }
        checkRep();
        /** Assign the new sorted ArrayList */
        return result;
    }

    /**
     * @param parent the parent of nodes we want to iterate
     * @requires this != null
     * @return the iterator pointing to child Nodes that is sorted alphabetically and the by weight
     * @throws IllegalArgumentException if parent == null
     */
    public Iterator<String> childrenItr(T parent){
        if (parent == null){
            throw new IllegalArgumentException("parent is null!");
        }
        if (!adj_lst.containsKey(parent)){
            return null;
        }
        ArrayList<Edge<String,U>> children = new ArrayList<>();
        for (Edge<String,U> child : adj_lst.get(parent)){
            children.add(child);
        }
        ArrayList<String> new_children = sortChildren(children);
        checkRep();
        return new_children.iterator();
    }

    // return iterator of all edges
    public Iterator<Edge<String,U>> getEdges(String start){
        ArrayList<Edge<String,U>> all_children = new ArrayList<Edge<String,U>>(adj_lst.get(start));
        return all_children.iterator();
    }

    /**
     * @throws RuntimeException if any node is null
     */
    private void checkRep() throws RuntimeException{
        if (adj_lst.containsKey(null)){
            throw new RuntimeException("Graph contains null node!");
        }
    }
}