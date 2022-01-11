package hw7;
import java.io.IOException;
import java.util.*;
import hw4.*;
import hw6.*;

public class CampusModel {
    private Graph<String, Double> graph; // Storing id as node
    private HashMap<Integer, Building> allBuildings;
    private HashMap<String, Integer> allIDs;

    public CampusModel(){}

    /**
     * @param fileNode csv files that contains all node information
     * @param fileEdge csv files that contains all edge information
     * @effects Create the graph based on the data from two files and store two map of building and id for searching
     */
    public void createGraph(String fileNode, String fileEdge){
        // initialization
        HashMap<Integer, Building> id_building = new HashMap<Integer, Building>();
        HashMap<Integer, HashSet<Integer>> edges = new HashMap<Integer, HashSet<Integer>>();
        HashMap<String, Integer> name_id = new HashMap<String, Integer>();
        try{
            CampusParser.readData(fileNode, fileEdge, id_building, edges, name_id);
        } catch (IOException e) {
            throw new IllegalArgumentException("Unable to parse files");
        }
        this.graph = new Graph<String, Double>();
        this.allBuildings = id_building;
        this.allIDs = name_id;
        // add all nodes to the graph
        for (int node: id_building.keySet()){
            graph.addNode(Integer.toString(node));
        }
        // add all edges to the graph
        for (int start: edges.keySet()){
            for (int end: edges.get(start)){
                Building b1 = id_building.get(start);
                Building b2 = id_building.get(end);
                double weight = Math.sqrt((b1.x()-b2.x())*(b1.x()-b2.x())+(b1.y()-b2.y())*(b1.y()-b2.y()));
                Edge<String,Double> e1 = new Edge<String,Double>(Integer.toString(start),Integer.toString(end),weight);
                Edge<String,Double> e2 = new Edge<String,Double>(Integer.toString(end),Integer.toString(start),weight);
                this.graph.addEdge(e1);
                this.graph.addEdge(e2);
            }
        }
    }

    /**
     * @param start starting node (can be either id or name)
     * @param end destination node (can be either id or name)
     * @return the formatted output string of the shortest path
     */
    public String findPath(String start, String end){
        // local variables
        String building1 = "";
        String building2 = "";
        int id1 = -1;
        int id2 = -1;
        StringBuffer output = new StringBuffer();
        boolean NotExist = false;
        // check start
        try{ // is int
            id1 = Integer.parseInt(start);
            if (allBuildings.containsKey(id1)){
                if (allBuildings.get(id1).name().isEmpty()){
                    NotExist = true;
                    output.append("Unknown building: ["+start+"]\n");
                }
                building1 = allBuildings.get(id1).name();
            }
            else{
                NotExist = true;
                output.append("Unknown building: ["+start+"]\n");
            }
        }
        catch(Exception e){ // is string
            building1 = start;
            if (allIDs.containsKey(start)){
                id1 = allIDs.get(start);
            }
            else{
                NotExist = true;
                output.append("Unknown building: ["+start+"]\n");
            }
        }
        // check end
        try{ // is int
            id2 = Integer.parseInt(end);
            if (allBuildings.containsKey(id2)){
                if (allBuildings.get(id2).name().isEmpty()){
                    NotExist = true;
                    if (!end.equals(start)) {
                        output.append("Unknown building: ["+end+"]\n");
                    }
                }
                building2 = allBuildings.get(id2).name();
            }
            else{
                NotExist = true;
                if (!end.equals(start)) {
                    output.append("Unknown building: ["+end+"]\n");
                }
            }
        }
        catch(Exception e){ // is string
            building2 = end;
            if (allIDs.containsKey(end)){
                id2 = allIDs.get(end);
            }
            else{
                NotExist = true;
                if (!end.equals(start)) {
                    output.append("Unknown building: ["+end+"]\n");
                }
            }
        }
        // Something not found
        if (NotExist){ return output.toString(); }
        else{// Everything goes fine, do MarvelPath2
            MarvelPaths2 path = new MarvelPaths2();
            LinkedList<AbstractMap.SimpleEntry<String, Double>> result = new LinkedList<AbstractMap.SimpleEntry<String, Double>>();
            ArrayList<Integer> havePath = new ArrayList<Integer>();
            path.findPath(String.valueOf(id1), String.valueOf(id2), result, havePath, graph);
            if (havePath.size()!=0){ // Do have a path
                output.append(direction(building1,building2,result));
            }
            else{ // No path exist
                if (start.equals(end)) {
                    output.append(String.format("Path from %s to %s:\nTotal distance: 0.000 pixel units.\n", start, end));
                }
                else {
                    output.append(String.format("There is no path from %s to %s.\n",allBuildings.get(id1).name(), allBuildings.get(id2).name()));
                }
            }
            return output.toString();
        }
    }

    /**
     * @param start String value of the first building's id
     * @param end String value of the second building's id
     * @param path all Path calculated
     * @return the step by step formatted string direction instructions
     */
    public String direction(String start, String end, LinkedList<AbstractMap.SimpleEntry<String, Double>> path){
        Iterator<AbstractMap.SimpleEntry<String, Double>> itr = path.iterator();
        AbstractMap.SimpleEntry<String, Double> temp = itr.next();
        Building parent = allBuildings.get(Integer.parseInt(temp.getKey()));
        Building child;
        double weight = temp.getValue();
        StringBuffer output = new StringBuffer();
        output.append(String.format("Path from %s to %s:\n", start, end));
        while(itr.hasNext()){
            // next iteration
            temp = itr.next();
            child = allBuildings.get(Integer.parseInt(temp.getKey()));
            String direction = "North";
            // Calculate the directions
            Double distance = temp.getValue() - weight;
            double x_axis = child.x() - parent.x();
            double y_axis = child.y() - parent.y();
            double degree = Math.toDegrees(Math.asin(x_axis/distance));
            // get degree into 360 from 12 clock in clockwise
            if (degree > 0){
                if (y_axis>0){
                    degree = 180 - degree;
                }
            }
            else{
                if (y_axis>0){
                    degree = 180 - degree;
                }
                else{
                    degree = 360 + degree;
                }
            }
            // determine the direction
            if(degree < 22.5) {
                direction = "North";
            }
            else if(degree < 67.5) {
                direction = "NorthEast";
            }
            else if(degree < 112.5) {
                direction = "East";
            }
            else if(degree < 157.5) {
                direction = "SouthEast";
            }
            else if(degree < 202.5) {
                direction = "South";
            }
            else if(degree < 247.5) {
                direction = "SouthWest";
            }
            else if(degree < 292.5) {
                direction = "West";
            }
            else if(degree < 337.5) {
                direction = "NorthWest";
            }

            // output
            if (child.name().isEmpty()){ // Intersection
                output.append(String.format("\tWalk %s to (Intersection %s)\n", direction, temp.getKey()));
            }
            else{ // Actual Building
                output.append(String.format("\tWalk %s to (%s)\n", direction, child.name()));
            }
            weight = temp.getValue();
            parent = child;
        }
        output.append(String.format("Total distance: %.3f pixel units.\n", weight));
        return output.toString();
    }

    /**
     * @return a formatted "name,id" output of all buildings
     */
    public String ListAllBuildings(){
        ArrayList<String> names = new ArrayList<String>();
        for (String name: allIDs.keySet()){
            names.add(name+","+Integer.toString(allIDs.get(name)));
        }
        Collections.sort(names);
        StringBuffer output = new StringBuffer();
        for (String out: names){
            output.append(out+"\n");
        }
        return output.toString();
    }
}
