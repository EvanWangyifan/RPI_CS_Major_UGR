package hw4;

/**
 * <b>Edge</b> represents an Edge of directed graph.
 * It includes the name of starting node (String),
 * the name of ending node (String), and the weight
 * of the edge (String).
 */
public class Edge<T, U> {

    private T start;
    private T end;
    private U weight;

    /**
     * @param s The name of the start node
     * @param e The name of the end node
     * @effects Construct a new Edge with null weight
     */
    public Edge(T s, T e){
        this.start = s;
        this.end = e;
        this.weight = null;
        checkRep();
    }

    /**
     * @param s The name of the start node
     * @param e The name of the end node
     * @param w The weight of the edge
     * @effects Construct a new Edge
     */
    public Edge(T s, T e, U w){
        this.start = s;
        this.end = e;
        this.weight = w;
        checkRep();
    }

    /**
     * @return the name of start node
     */
    public T getStart(){ return this.start; }

    /**
     * @return the name of end node
     */
    public T getEnd(){ return this.end; }

    /**
     * @return the weight of the edge
     */
    public U getWeight(){ return this.weight; }

    /**
     * @param w the new weight you want to set for the edge
     * @effects set the weight of the edge to the new weight
     */
    public void setWeight(U w){
        if (w == null){
            throw new IllegalArgumentException("w is null!");
        }
        this.weight = w;
    }

    @Override
    public boolean equals(Object o)
    {
        if(o == this)
            return true;
        if(!(o instanceof Edge<?, ?>))
            return false;

        Edge<?, ?> e = (Edge<?, ?>) o;
        return this.getWeight().equals(e.getWeight()) && this.getStart().equals(e.getStart()) && this.getEnd().equals(e.getEnd());
    }

    @Override
    public int hashCode()
    {
        return getStart().hashCode() + getEnd().hashCode() + getWeight().hashCode();
    }

    /**
     * Checks that the representation invariant holds (if any).
     **/
    public void checkRep() throws RuntimeException{
        if (this.start == null){
            throw new RuntimeException("start is null!");
        }
        if (this.end == null){
            throw new RuntimeException("end is null!");
        }
    }
}
