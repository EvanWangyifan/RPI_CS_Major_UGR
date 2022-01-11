package hw7;
import java.util.*;
/**
 * the Initializer of CampusPath.
 * allows input command through System.in and print results
 * Commands: b, r, q, m
 */
public class CampusPaths{
    /**
     * Initialize the CampusPath
     * get the input from user and output result
     */
    public static void main(String[] args){
        // Initialization
        CampusModel m = new CampusModel();
        m.createGraph("data/RPI_map_data_Nodes.csv", "data/RPI_map_data_Edges.csv");
        //read System.in
        Scanner reader = new Scanner(System.in);
        while(reader.hasNext()){
            String input = reader.nextLine();
            if (input.equals("b")){
                System.out.print(m.ListAllBuildings());
            }
            else if (input.equals("r")){
                System.out.print("First building id/name, followed by Enter: ");
                String start = reader.nextLine();
                System.out.print("Second building id/name, followed by Enter: ");
                String end = reader.nextLine();
                System.out.print(m.findPath(start, end));
            }
            else if (input.equals("q")){
                reader.close();
                return;
            }
            else if (input.equals("m")){
                System.out.print("commands available:\n");
                System.out.print("b: list all buildings in (name,id) format in lexicographic order.\n");
                System.out.print("r: input two building and get a shortest path.\n");
                System.out.print("q: quit the program.\n");
                System.out.print("m: list the menu.\n");
            }
            else{
                System.out.print("Unknown option\n");
            }
        }
        reader.close();
    }
}