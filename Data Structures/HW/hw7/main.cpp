#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <cmath> 
#include <vector>
#define earthRadiusKm 6371.0
#ifndef M_PI
#define M_PI 3.14
#endif

//TODO: You must fill in all ?????? with the correct types.
typedef long ID_TYPE; //Type for user IDs
typedef std::vector<double> COORD_TYPE; //Type for a coordinate (latitude and longitude)
typedef std::map<long,std::vector<long> > ADJ_TYPE; //Adjacency Lists type
typedef std::map<long, std::vector<double> > GEO_TYPE; //Positional "dictionary"

//Function forward declarations. DO NOT CHANGE these. 
double deg2rad(double deg);
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d);
void loadConnections(ADJ_TYPE& adj_lists, std::ifstream& loadfile);
void loadLocations(GEO_TYPE& locations, std::ifstream& loadfile);
void printAllUsersWithinDistance(const GEO_TYPE& locations, std::ofstream& outfile,
                                 const ID_TYPE& start_id, double max_distance);
void printDegreesHistogram(const ADJ_TYPE& adj_lists, std::ofstream& outfile);
void printDegreesOfAll(const ADJ_TYPE& adj_lists, std::ofstream& outfile);
void printFriendsWithDegree(const ADJ_TYPE& adj_lists, std::ofstream& outfile,
                            const ID_TYPE& start_id, unsigned int degree);
void printFriendsWithinDistance(const ADJ_TYPE& adj_lists, const GEO_TYPE& locations, 
                                std::ofstream& outfile, const ID_TYPE& start_id, 
                                double max_distance);
void printUsersWithinIDRange(std::ofstream& outfile, ADJ_TYPE::const_iterator begin, 
                             ADJ_TYPE::const_iterator end, 
                             ADJ_TYPE::const_iterator start_it, unsigned int offset);


// DO NOT CHANGE THIS FUNCTION
int main(int argc, char** argv){
    ADJ_TYPE adj_lists;
    GEO_TYPE locations;

    if(argc != 3){
        std::cout << "Correct usage is " << argv[0] 
                  << " [commands file] [output file]" << std::endl;
        return -1;
    }

    std::ifstream commands(argv[1]);
    if(!commands){
        std::cerr << "Problem opening " << argv[1] << " for reading!" 
                  << std::endl;
        return -1;
    }

    std::ofstream outfile(argv[2]);
    if(!outfile){
        std::cerr << "Problem opening " << argv[2] << " for writing!" 
                  << std::endl;
        return -1;
    }

    std::string token; //Read the next command
    while(commands >> token){
        if(token == "load-connections"){
            std::string filename;
            commands >> filename;

            std::ifstream loadfile(filename.c_str());
            if(!loadfile){
                std::cerr << "Problem opening " << filename 
                          << " for reading connections." << std::endl;
                return -1;
            }

            loadConnections(adj_lists, loadfile);
        }
        else if(token == "load-locations"){
            std::string filename;
            commands >> filename;

            std::ifstream loadfile(filename.c_str());
            if(!loadfile){
                std::cerr << "Problem opening " << filename 
                          << " for reading locations." << std::endl;
                return -1;
            }

            loadLocations(locations, loadfile);
        }
        else if(token == "print-degrees"){
            printDegreesOfAll(adj_lists, outfile);
        }
        else if(token == "print-degree-histogram"){
            printDegreesHistogram(adj_lists, outfile);
        }
        else if(token == "all-users-within-distance"){
            ID_TYPE start_id;
            double max_distance; //In kilometers
            commands >> start_id >> max_distance;

            printAllUsersWithinDistance(locations, outfile, start_id, 
                                        max_distance);
        }
        else if(token == "friends-within-distance"){
            ID_TYPE start_id;
            double max_distance; //In kilometers
            commands >> start_id >> max_distance;

            printFriendsWithinDistance(adj_lists, locations, outfile, 
                                       start_id, max_distance);
        }
        else if(token == "friends-with-degree"){
            ID_TYPE start_id,degree;
            commands >> start_id >> degree;

            printFriendsWithDegree(adj_lists, outfile, start_id, degree);
        }
        else if(token == "nodes-within-ID-range"){
            ID_TYPE start_id,offset;
            commands >> start_id >> offset;

            printUsersWithinIDRange(outfile, adj_lists.begin(), adj_lists.end(),
                                    adj_lists.find(start_id), offset);
        }
        else{
            std::cerr << "Unknown token \"" << token << "\"" << std::endl;
            return -1;
        }
    }

    return 0;
}




// This function converts decimal degrees to radians
// From https://stackoverflow.com/a/10205532
// DO NOT CHANGE THIS FUNCTION
double deg2rad(double deg) {
  return (deg * M_PI / 180);
}

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * Taken from https://stackoverflow.com/a/10205532
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 * DO NOT CHANGE THIS FUNCTION
 */
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
  double lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r)/2);
  v = sin((lon2r - lon1r)/2);
  return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

//////////////////TODO: IMPLEMENT ALL FUNCTIONS BELOW THIS POINT////////////////////

/**
 * Loads a list of connections in the format "a b" meaning b is a friend of a
 * into the adjacency lists data structure. a and b are IDs.
 * @param adj_lists Adjacency lists structure
 * @param loadfile File to read from
 */
void loadConnections(ADJ_TYPE& adj_lists, std::ifstream& loadfile){
	// This will create a map with <user,vector of friends>
	ID_TYPE id1;
	ID_TYPE id2;
	// read the input file line by line
 	while (loadfile >> id1 >> id2){
 		// find if id1 is already in the map
 		// if already in, push_back id2 in it
 		// otherwise create a new pair
 		// Make id1 the key and all friends of id1 the value
 		adj_lists[id1].push_back(id2);
 	}
}

/**
 * Loads a list of locations in the format "id latitude longitude"
 * @param locations Location lookup table
 * @param loadfile File to read from
 */
void loadLocations(GEO_TYPE& locations, std::ifstream& loadfile){
	// This will create a map with <user, <latitude, longtitude> >
	ID_TYPE id;
	double lat;
	double lon;
	// read input file
	while (loadfile >> id >> lat >> lon){
		// Create new pair in the map
		// id is the key and lat & lon in the vector as values
		// Make id the key and vector with latitude and longtitude the value
		locations[id].push_back(lat);
		locations[id].push_back(lon);
	}
}

/**
 * Prints all users within a certain distance of a particular user.
 * Sorted from shortest distance to user to longest distance to user.
 * Rounds down to the next lowest km if the difference is a decimal
 * @param locations Location lookup table
 * @param outfile File to write output to
 * @param start_id User we are using as our "origin" (i.e. 0 distance)
 * @param max_distance Maximum distance from start_id that a printed user can be
 */
void printAllUsersWithinDistance(const GEO_TYPE& locations, 
	std::ofstream& outfile, const ID_TYPE& start_id, double max_distance){
	// store all distance as key and user within the limit as value
	std::map<int,std::vector<long> > result;
	// See if the user in the list
	GEO_TYPE::const_iterator itr_start = locations.find(start_id);
	// If user in the list
	if (itr_start!=locations.end()){
		GEO_TYPE::const_iterator itr=locations.begin();
		// Find all the location for user's friends
		for (; itr!=locations.end(); itr++){
			// calculate the distance
			double distance = distanceEarth(itr->second[0],itr->second[1],
				itr_start->second[0],itr_start->second[1]);
			// If distance within limit
			if (distance <= max_distance && itr_start->first!=itr->first){
				// put all distance and user in the range to the all_possible
				result[(int)distance].push_back(itr->first);
			}
		}
		// If no friend in the limit
		if (result.size()==0){
			outfile << "There are no users within " << max_distance
					<< " km of user " << start_id<<std::endl;
		}
		else{
			outfile << "User IDs within " << max_distance << " km of user "
					<< start_id << ':' << std::endl;
			// Print the info in the map which is sorted automatically
			std::map<int,std::vector<long> >::
			iterator print_itr = result.begin();
			for (; print_itr!=result.end(); print_itr++){
				outfile << ' ' << print_itr->first << " km:";
				for (int i=0; i<print_itr->second.size(); i++){
					outfile << ' ' << print_itr->second[i];
				}
				outfile << std::endl;
			}
		}
	}
	// If start_id not in locations
	else{
		outfile << "User ID " << start_id << " does not have a recorded location." 
				<< std::endl;
	}
}

/**
 * Prints a sorted list of degrees (smallest to largest) along with how many users
 * have that degree. Only prints for degree >=1.
 * @param adj_lists Adjacency lists structure
 * @param outfile File to write output to
 */
void printDegreesHistogram(const ADJ_TYPE& adj_lists, std::ofstream& outfile){
	// map to store number of degree and how many user have that degree
	std::map<int,long> result;
	ADJ_TYPE::const_iterator itr = adj_lists.begin();
	// For all users
	for (; itr!=adj_lists.end(); itr++){
		// while the user has at least 1 degree
		if (itr->second.size()>0){
			// add to the number of that degree
			result[itr->second.size()]++;
		}
	}
	// print in output file
	outfile << "Histogram for " << adj_lists.size() << " users:" << std::endl;
	std::map<int,long>::iterator print_itr = result.begin();
	for (; print_itr!=result.end(); print_itr++){
		outfile << " Degree " << print_itr->first << ": " << print_itr->second
				<< std::endl;
	}
}

/**
 * Prints a sorted list of user IDs (smallest to largest) along with the degree for
 * each user. Only prints for degree >=1.
 * @param adj_lists Adjacency lists structure
 * @param outfile File to write output to
 */
void printDegreesOfAll(const ADJ_TYPE& adj_lists, std::ofstream& outfile){
	ADJ_TYPE::const_iterator itr = adj_lists.begin();
	outfile << "Degrees for " <<  adj_lists.size() << " users:" << std::endl;
	// For all users if the user has more than 0 friends print the degree
	for (; itr!=adj_lists.end(); itr++){
		if (itr->second.size()>=1){
			outfile << ' ' << itr->first << ": Degree " << itr->second.size()
					<< std::endl;
		}
	}
}

/**
 * Prints all friends of a particular user who have a particular degree.
 * Sorted by user ID (from smallest to largest).
 * @param adj_lists Adjacency lists structure
 * @param outfile File to write output to
 * @param start_id User whose friends we are looking at
 * @param degree The degree of friends we want to print. Will be >=1.
 */
void printFriendsWithDegree(const ADJ_TYPE& adj_lists, std::ofstream& outfile,
                            const ID_TYPE& start_id, unsigned int degree){
	std::vector<long> result;
	ADJ_TYPE::const_iterator itr_start = adj_lists.find(start_id);
	if (itr_start!=adj_lists.end()){
		// For all the user's friends
		for (int i=0; i<itr_start->second.size(); i++){
			ADJ_TYPE::const_iterator itr_friends = 
			adj_lists.find(itr_start->second[i]);
			// If friend in the list
			if (itr_friends!=adj_lists.end()){
				// If the friend has the same degree
				if (itr_friends->second.size()==degree){
					// Save the friend's ID
					result.push_back(itr_friends->first);
				}
			}
		}
		// sort the vector by user ID
		std::sort(result.begin(), result.end());
		// Output
		outfile << "User " << start_id << " has " << result.size() 
				<< " friend(s) with degree " << degree;
		if (result.size()>0){
			outfile << ':';
		}
		for (int i=0; i<result.size(); i++){
			outfile << ' ' << result[i];
		}
		outfile << std::endl;
	}
	// If cannot find start_id in the list
	else{
		outfile << "There is no user with friends and ID " << start_id 
				<< std::endl;
	}
}

/**
 * Prints friends of a particular user within a certain distance of that user.
 * Sorted from shortest distance to user to longest distance to user.
 * @param adj_lists Adjacency lists structure
 * @param locations Location lookup table
 * @param outfile File to write output to
 * @param start_id User we are using as our "origin" (i.e. 0 distance)
 * @param max_distance Maximum distance from start_id that a printed user can be
 */
void printFriendsWithinDistance(const ADJ_TYPE& adj_lists, const GEO_TYPE& locations, 
                                std::ofstream& outfile, const ID_TYPE& start_id, 
                                double max_distance){
	// If start_id not in the adj list
	ADJ_TYPE::const_iterator itr_friends = adj_lists.find(start_id);
	if (itr_friends==adj_lists.end()){
		outfile << "There is no user with friends and ID " << start_id 
				<< std::endl;
		return;
	}
	// If start_id not in the location list
	GEO_TYPE::const_iterator itr_start_loc = locations.find(start_id);
	if (itr_start_loc==locations.end()){
		outfile << "User ID " << start_id 
				<< " does not have a recorded location." << std::endl;
		return;
	}
	// If start_id in the list
	if (itr_friends!=adj_lists.end()){
		// map store distance and id
		std::map<double,long> result;
		// vector for all the friends of start_id
		std::vector<long> friends(itr_friends->second);
		std::vector<double> location;
		// For all friends of the user
		for (int i=0; i<friends.size(); i++){
			// find the location of friends of user
			GEO_TYPE::const_iterator itr_location = locations.find(friends[i]);
			std::vector<double> friend_location = itr_location->second;
			// if the distance is smaller than limit
			double distance = distanceEarth(friend_location[0],friend_location[1],
				itr_start_loc->second[0],itr_start_loc->second[1]);
			if (distance <= max_distance){
				// make distance the key and ID the value
				result[distance]= friends[i];
			}	
		}
		// If no friend in limit
		if (result.size()==0){
			outfile << "There are no friends within "<< max_distance 
					<< " km of user " << start_id << std::endl;
			return;
		}
		else{
			// Normal output
			std::map<double,long>::iterator print_itr = result.begin();
			outfile << "Friends within " << max_distance << " km of user "
					<< start_id << ':' << std::endl;
			for (; print_itr!=result.end(); print_itr++){
				outfile << ' ' << print_itr->first << " km: " << print_itr->second
						<< std::endl;
			}
		}
	}
}

/**
 * Prints users with an ID that is "close" to a particular user's ID.
 * Sorted from smallest to largest user ID.
 * Only prints for degree >=1.
 *
 * If there are M IDs that should be printed this function should only use
 * ADJ_TYPE::const_iterator::operator++ / ADJ_TYPE::const_iterator::operator-- O(M) times
 *
 * @param outfile File to write output to
 * @param begin The .begin() iterator for the adjacency lists structure
 * @param end The .end() iterator for the adjacency lists structure
 * @param start_it Iterator for user we are using as our baseline (i.e. 0 diifference)
 *        It is possible that the start_it was not found in the data.
 * @param offset Maximum absolute difference from start_id that a printed user's
 *               ID can have
 */
void printUsersWithinIDRange(std::ofstream& outfile, ADJ_TYPE::const_iterator begin, 
                             ADJ_TYPE::const_iterator end, 
                             ADJ_TYPE::const_iterator start_it, unsigned int offset){
	// if start_it not in the list
	if (start_it==end){
		outfile << "There is no user with the requested ID" << std::endl;
	}
	else{
		// vector store all id satisfy the condition
		std::vector<long> result;
		ADJ_TYPE::const_iterator itr = begin;
		for (; itr!=end; itr++){
			// If absolute value is smaller than offset and it's not himself
			if (std::abs((itr->first)-(start_it->first))<=offset 
								&& itr->first!=start_it->first){
				// record the id
				result.push_back(itr->first);
			}
		}
		// If no user satisfy the condition
		if (result.size()==0){
			outfile << "There are no users with an ID within +/-" << offset << " of "
					<< start_it->first << std::endl;
		}
		else{
			// sort by id
			std::sort(result.begin(), result.end());
			// Normal output
			outfile << "Users with an ID within +/-" << offset << " of " 
					<< start_it->first << ':';
			for (int i=0; i<result.size(); i++){
				outfile << ' ' << result[i];
			}
			outfile << std::endl;
		}
	}
}
