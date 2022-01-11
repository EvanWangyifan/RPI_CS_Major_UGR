package hw7;
import java.io.*;
import java.util.*;

public class CampusParser {
    /**
     * @param: fileNode
     * @param: fileEdge
     * @param: allNodes HashMap to store <id,building>
     * @param: allEdges HashMap to store <id,HashSet<id>>
     * @param: allIds HashMap to store <name,id>
     * @effects: fill all containers
     * @throws: IOException if file cannot be read of file not a CSV file
     */
    public static void readData(String fileNode, String fileEdge, HashMap<Integer, Building> allNodes,
                                HashMap<Integer, HashSet<Integer>> allEdges, HashMap<String, Integer> allIDs) throws IOException{
        // read node file
        BufferedReader reader = new BufferedReader(new FileReader(fileNode));
        String line = null;
        String[] temp = null;
        while ((line = reader.readLine()) != null) {
            temp = line.split(",");
            String name = temp[0];
            int id = Integer.parseInt(temp[1]);
            int x = Integer.parseInt(temp[2]);
            int y = Integer.parseInt(temp[3]);
            Building b = new Building(name,id,x,y);
            allNodes.put(id,b);
            if (!temp[0].isEmpty()){
                allIDs.put(name,id);
            }
        }
        reader.close();
        // read edge file
        reader = new BufferedReader(new FileReader(fileEdge));
        line = null;
        temp = null;
        while ((line = reader.readLine()) != null) {
            temp = line.split(",");
            int start = Integer.parseInt(temp[0]);
            int end = Integer.parseInt(temp[1]);
            if (allEdges.containsKey(start)){
                allEdges.get(start).add(end);
            }
            else{
                HashSet<Integer> arr = new HashSet<Integer>();
                arr.add(end);
                allEdges.put(start,arr);
            }
        }
        reader.close();
    }
}
