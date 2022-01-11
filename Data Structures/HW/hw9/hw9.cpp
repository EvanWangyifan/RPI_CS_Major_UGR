#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>
#include<cassert>

// Movie class that make all attributes public
class Movie{
public:
    std::string Title;
    std::string Year;
    std::string Runtime;
    std::vector<std::string> GenreList;
    std::vector<std::string> ActorList;
    std::vector<std::string> RoleList;
    std::string hash_key;
    // Printing function output movie in required format
    void print(std::map<std::string,std::string>& actors) const {
        std::cout << Title << std::endl;
        std::cout << Year << std::endl;
        std::cout << Runtime << std::endl;
        std::cout << GenreList.size();
        for (int i=0; i<GenreList.size(); i++){
            std::cout << ' ' << GenreList[i];
        }
        std::cout << std::endl;
        std::cout << ActorList.size();
        for (int i=0; i<ActorList.size(); i++){
            std::cout << ' ' << actors[ActorList[i]] << " (" << 
                        RoleList[i] << ')';
        }
        std::cout << std::endl;
    }
};

// Hash functor returnning the index in the vector
class HashFunc{
public:
    int operator()(std::string const &key) const{
        // This implementation comes from
        // http://www.partow.net/programming/hashfunctions/
        int hash = 31415926; // slightly changes for initial number(pi value)
        for (int i = 0; i < key.length(); i++)
            hash ^= ((hash << 5) + key[i] + (hash >> 2));
        return hash;
    }
};

// Hash_Table class that create a vector of Movie objects pointers
class Hash_Table{
private:
    std::vector<Movie*> m_table; // actual table
    HashFunc m_hash;            // hash function
    int m_size;        // number of keys
public:
    // Constructor for the table accepts the size of the table
    Hash_Table(int init_size = 100): m_table(init_size,NULL), m_size(0) {}

    // Destructor
    ~Hash_Table(){
        for (int i=0; i<m_table.size(); i++){
            if (m_table[i]==NULL){
                continue;
            }
            delete m_table[i];
        }
        m_table.clear();
    }

    void operator=(Hash_Table a){
        m_table = a.m_table;
        m_hash = a.m_hash;
        m_size = a.m_size;
    }

    // Return size
    int size() const { return m_size; }

    // Function that find all combination of informations
    // in_vec: the vector of string you want to find combination from
    // index: the index of in_vec
    // num: how many attribute need choosing
    // temp_vec: a temp vector storing temporory result for each solution
    // result: the final vector containing all possible solutions
    void all_combo(std::vector<std::string>& in_vec, int index, int num, 
        std::vector<std::string>& temp_vec, std::vector<std::string>& result){
        
        if (num == 0){   //Already find a combo
            std::string st;
            std::vector<std::string>::iterator it = temp_vec.begin();
            for (; it != temp_vec.end(); it++){
                st += *it;
            }
            result.push_back(st);    //store it in the vector
            return;
        }
        // return if reach the end
        if(index==in_vec.size()){
            return;
        }
        // Add to temp vec
        temp_vec.push_back(in_vec[index]);
        all_combo(in_vec, index+1, num - 1, temp_vec,result);
        temp_vec.pop_back();// Pop the thing you added
        // Recurse
        all_combo(in_vec, index+1, num, temp_vec, result);       
    }


    // Find function, returning the correspoinding movie object
    // entering all the attributes available
    void find(std::string title, std::string year, 
        std::string runtime, std::vector<std::string> genreList, 
        std::vector<std::string> actorList, std::vector<std::string> roleList,
        std::vector<Movie>& result){
        // Create the string key for hashing
        std::string key = "";
        key += title;
        key += year;
        key += runtime;
        for (int i=0; i<genreList.size(); i++){
            key += genreList[i];
        }
        for (int i=0; i<actorList.size(); i++){
            key += actorList[i];
        }
        for (int i=0; i<roleList.size(); i++){
            key += roleList[i];
        }
        // Find the index by key and make an copy for later use
        int index = m_hash(key);
        index = index % m_table.size();
        int original_index = index;
        int upper_limit = m_table.size();

        int num = 0;
        for(int i = 0; i < m_table.size(); i++){
            if(m_table[i] == NULL){num++;}
        }
        
        // Go through the table to find all object satisfy the requirement
        // If reach the end start over from the start until reach original start
        while (m_table[index] != NULL && index<upper_limit-1){
            if (m_table[index]->hash_key==key){
                result.push_back(*m_table[index]);
            }
            
            // Increment index to go to next spot
            index++;
            if (index==upper_limit-1){
                index = 0;
                upper_limit = original_index;
            }
        }
    }

    // Insert function
    void insert(Movie const& movie, float const occupancy){
        // Create a vector of string that constains all the attributes of movie
        std::vector<std::string> movies;
        movies.push_back(movie.Title);
        movies.push_back(movie.Year);
        movies.push_back(movie.Runtime);
        std::string temp = "";
        // Convert all string in vectors to a single string
        for (int i=0; i<movie.GenreList.size(); i++){
            temp += movie.GenreList[i];
        }
        movies.push_back(temp);
        temp = "";
        for (int i=0; i<movie.ActorList.size(); i++){
            temp += movie.ActorList[i];
        }
        movies.push_back(temp);
        temp = "";
        for (int i=0; i<movie.RoleList.size(); i++){
            temp += movie.RoleList[i];
        }
        movies.push_back(temp);

        std::vector<std::string> result;
        std::vector<std::string> temp_vec;
        // Find all combinations choosing 1-6 features from total 6 features
        for (int i=1; i<=6; i++){
            all_combo(movies,0,i,temp_vec,result);
        }
        
        // Actual Insert
        for (int i=0; i<result.size(); i++){

            // Check occupancy
            if (m_size >= occupancy * m_table.size()){
                this->resize_table(2*m_table.size()+1);
            }
            std::string key = result[i];
            int index = m_hash(key);
            index = index % m_table.size();
            // Go through the vector to find empty spot
            // otherwise go to the next spot
            while (m_table[index]!=NULL && index<m_table.size() - 1){
                index++;
            }
            // If get to the end of the vector, go to the start
            if (index==m_table.size()-1){
                index = 0;
                // Start over searching for empty spot from the start
                while (m_table[index] && index < m_table.size() - 1){
                    index++;
                }
            }

            // Adding new dynamic movie object's value to the hash table's value
            Movie* temp = new Movie();
            temp->Title = movie.Title;
            temp->Year = movie.Year;
            temp->Runtime = movie.Runtime;
            temp->GenreList = movie.GenreList;
            temp->RoleList = movie.RoleList;
            temp->ActorList = movie.ActorList;
            temp->hash_key = key;
            m_table[index] = temp;
            // Update size
            m_size++;
        }

    }

    // Resize function
    void resize_table(int new_size){
        // Make a copy of original table
        std::vector<Movie*> new_table;
        new_table = m_table;
        // Create a new one
        m_table.clear();
        for (int i=0; i<new_size; i++){
            m_table.push_back(NULL);
        }
        // Check the size
        assert(m_table.size()==new_size);
        // Copy from the original table
        for (int i=0; i<new_table.size();i++){
            // Linear Probing
            if (new_table[i]!=NULL){
                std::string key = new_table[i]->hash_key;
                int index = m_hash(key);
                index = index % m_table.size();
                while(m_table[index] != NULL){
                    index++;
                    if(index == m_table.size()){
                        index = 0;
                    }
                }
                // Reinsert movies
                m_table[index] = new_table[i];
            }
        }
    }

};


// Main function
int main(){
    // Default values / temp variables
    std::vector<Movie> all_movies;
    int table_size = 100;
    float occupancy = 0.5;
    std::string movies_file;
    std::string actors_file;
    std::string temp;
    Hash_Table table(table_size);
    // Map which has code as key and name as value
    std::map<std::string, std::string> actors;
    // Read information by cin
    while (std::cin >> temp){
        if (temp=="table_size"){
            std::cin >> table_size;
            table.resize_table(table_size);
        }
        // Create Hash Table !!
        else if (temp=="occupancy"){
            std::cin >> occupancy;
            table.resize_table(table_size);
        }
        else if (temp=="movies"){
            std::cin >> movies_file;
            // Read movies file
            std::ifstream f_movies(movies_file);
            //Print error if cannot open the file to read
            if (!f_movies.good()){
                std::cerr << "Can't open " << movies_file << " to read.\n";
                exit(1);
            }
            std::string Title = "";
            std::string Year = "";
            std::string Runtime = "";
            std::vector<std::string> GenreList;
            std::vector<std::string> ActorList;
            std::vector<std::string> RoleList;
            // Read movie information
            while (f_movies >> Title >> Year >> Runtime){
                Movie temp_movie;
                temp_movie.Title = Title;
                temp_movie.Year = Year;
                temp_movie.Runtime = Runtime;
                int num = 0;
                f_movies >> num;
                for (int i=0;i<num;i++){
                    std::string temp_str;
                    f_movies >> temp_str;
                    temp_movie.GenreList.push_back(temp_str);
                }
                f_movies >> num;
                for (int i=0;i<num;i++){
                    std::string temp_str;
                    f_movies >> temp_str;
                    temp_movie.ActorList.push_back(temp_str);
                }
                f_movies >> num;
                for (int i=0;i<num;i++){
                    std::string temp_str;
                    f_movies >> temp_str;
                    temp_movie.RoleList.push_back(temp_str);
                }
                // Insert the movie object (in 63 combos) into the hash table
                // Insert every movie into the all_movies vector
                table.insert(temp_movie,occupancy);
                all_movies.push_back(temp_movie);
            }
        }
        else if (temp=="actors"){
            std::cin >> actors_file;
            // Read actors file
            std::ifstream f_actors(actors_file);
            //Print error if cannot open the file to read
            if (!f_actors.good()){
                std::cerr << "Can't open " << actors_file << " to read.\n";
                exit(1);
            }
            std::string code;
            std::string name;
            // Add into actor map
            while (f_actors >> code >> name){
                actors[code] = name;
            }
            

        }
        else if (temp=="query"){
            // The final result vector containing all movie object
            std::vector<Movie> result;
            int num = 0;
            std::string temp1;
            std::string temp2;
            std::string temp3;
            std::string title = "";
            std::string year = "";
            std::string runtime = "";
            std::vector<std::string> genreList;
            std::vector<std::string> actorList;
            std::vector<std::string> roleList;
            // Read title, year and runtime treat ? as empty
            std::cin >> temp1 >> temp2 >> temp3;

            if (temp1!="?"){
                title = temp1;
            }
            if (temp2!="?"){
                year = temp2;
            }
            if (temp3!="?"){
                runtime = temp3;
            }
            // Fill the list vectors
            std::cin >> num;
            for (int i=0; i<num; i++){
                std::cin >> temp1;
                genreList.push_back(temp1);
            }
            std::cin >> num;
            for (int i=0; i<num; i++){
                std::cin >> temp1;
                actorList.push_back(temp1);
            }
            std::cin >> num;
            for (int i=0; i<num; i++){
                std::cin >> temp1;
                roleList.push_back(temp1);
            }
            // If no query provided
            if (title.size()==0 && year.size()==0 && runtime.size()==0 &&
                        genreList.size()==0 && actorList.size()==0 && 
                                roleList.size()==0){
                // make result to be all movies
                result = all_movies;
            }
            // Find all the movie satisfy the available attributes add to result
            table.find(title,year,runtime,genreList,actorList,roleList,result);
            if (result.size()==0){
                std::cout << "No results for query." << std::endl;
            }
            else{
                std::cout << "Printing " << result.size() <<  
                            " result(s):" << std::endl;
                for (int i=0; i<result.size(); i++){
                    result[i].print(actors);
                }
            }
        }
        // If read quit, stop reading query
        if (temp=="quit"){
            break;
        }
    }
    return 0; // Everything goes fine
}