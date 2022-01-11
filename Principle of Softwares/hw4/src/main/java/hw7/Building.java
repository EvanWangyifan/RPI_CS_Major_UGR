package hw7;

/**
 * Building represents an immutable RPI building.
 * b.name() is the String name of the building, empty if it is an intersection.
 * b.id() is the id of the building.
 * b.x() is the x-coordinate of the building.
 * b.y() is the y-coordinate of the building.
 * b.name() != null
 */
 public class Building {
    /* AF:
     * A Building b is an  building or intersection in RPI.
     * if b.name() is an empty string, b is an intersection.
     * b has id, and its x and y coordinates.
     */
    private String name;
    private int id;
    private int x;
    private int y;

    /**
     * @param name String name of the building
     * @param id id of the building
     * @param x x-coordinate of the building
     * @param y y-coordinate of the building
     * @requires b.name() != null
     */
    public Building(String name, int id, int x, int y){
        this.name = name;
        this.id = id;
        this.x = x;
        this.y = y;
    }

    // return the name of the building
    public String name(){
        return this.name;
    }

    // return the id of the building
    public int id(){
        return this.id;
    }

    // return the x of the building
    public int x(){
        return this.x;
    }

    // return the y of the building
    public int y(){
        return this.y;
    }
}
