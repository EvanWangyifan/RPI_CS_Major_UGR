package hw4;

import java.util.Iterator;

public class GraphWrapper {
    private Graph<String, String> graph;

    public GraphWrapper(){
        graph = new Graph<String, String>();
    }

    public void addNode(String nodeData){
        graph.addNode(nodeData);
    }

    public void addEdge(String parentNode, String childNode, String edgeLabel){
        graph.addEdge(new Edge<String, String>(parentNode,childNode,edgeLabel));
    }

    public Iterator<String> listNodes(){ return graph.nodeItr(); }

    public Iterator<String> listChildren(String parentNode){
        return graph.childrenItr(parentNode);
    }

}