#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




struct element 
{
   	int priority; // 0 for unused, the higher the number the longer time it stays
   	int address;
};

// Check if the 1D array of element struct already full or not 
// if full -> 1, not full -> 0
int already_full(int associative, struct element all[associative]){
	for (int i=0; i<associative; i++){
		if (all[i].priority==-1){
			return 0;
			break;
		}
	}
	return 1;
}

// Try to find if the target is already in the array of struct
// Return the index if hit, return -1 if miss
int find_hit(int associative, struct element all[associative], struct element target){
	for (int i=0; i<associative; i++){
		if (all[i].address == target.address){
			return i;
			break;
		}
	}
	return -1;
}

// Return the distance of the number to the nearest next one
// Return -1 if there's no next one
int find_distance(int size, int all_address[size], int target, int start_index){
	int distance = -1; // Default to no next one
	for (int i=start_index; i<size; i++){
		if (target == all_address[i]){ // Find the match one
			distance = i-start_index; // Calculate the distance between the two
			break;
		}
	}
	return distance;
}

///////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

	int all_address[1024]; // Storing all input ints
	char line[100];
	int num_address = 0; // Total count of address read
	int associative = atoi(argv[1]);
	double hit_count = 0; // Count for how many hit
	int set_count = 256/associative;
	struct element all_element[set_count][associative]; // Storing all element structs


	// Check argc
	if (argc!=4){
		fprintf(stderr, "Invalid command line input!(Wrong number)\n");
		return EXIT_FAILURE;
	}

	// Read file
	FILE *file = fopen(argv[3],"r");
	while (fgets(line, 100, file)){
		all_address[num_address] = atoi(line);
		num_address++;
	}
	fclose(file);

	// Initialize the cache with empty element which has every attributes -1 
	struct element empty;
	empty.priority = -1;
	empty.address = -1;

	for (int i=0;i<set_count;i++){
		for (int j=0;j<associative;j++){
			all_element[i][j] = empty;
		}
	}

	printf("Cache size: 256\n");
	printf("Cache associativity: %d\n",associative);
	printf("Cache sets: %d\n",set_count);

///////////////////////////////////////////////////////////////////////////////////////////
	// For LRU
	if (strncmp(argv[2],"LRU",3)==0){
		printf("Cache algorithm: LRU\n");
		// Begin to read through all address!!!!
		for (int i=0;i<num_address;i++){
			// Create temp struct
			struct element temp;
			temp.priority = 0;
			temp.address = all_address[i];
			// Find the set index
			int set = all_address[i] % set_count;

			printf("%d ",all_address[i]);
			
			// If hit
			int hit_result = find_hit(associative,all_element[set],temp);

			if (hit_result!=-1){
				printf("(hit)\n");
				hit_count++;

				// Add priority to all non -1 elements

				for (int j=0;j<associative;j++){
					if (all_element[set][j].priority!=-1){
						all_element[set][j].priority++;
					}
				}
				// Reset the element we hit to 0
				all_element[set][hit_result].priority = 0;
			}
			else{ // If miss
				printf("(miss)\n");
				// If the cache is not full
				if (already_full(associative,all_element[set])==0){
					// Find place to insert
					int index = 0;
					for (int j=0;j<associative;j++){
						if (all_element[set][j].priority==-1){
							index = j;
							break;
						}
					}
					// Increment all priority to all non -1
					for (int j=0;j<associative;j++){
						if (all_element[set][j].priority!=-1){
							all_element[set][j].priority++;
						}
					}
					// Do the insert
					all_element[set][index] = temp;
				}

				// If the cache already full
				else{
					// Find the index of highest priority to replace
					int index = 0;
					int temp_priority = all_element[set][0].priority;
					for (int j=0;j<associative;j++){
						if (all_element[set][j].priority > temp_priority){
							index = j;
							temp_priority = all_element[set][j].priority;
						}
					}
					// Increment all priority to all non -1
					for (int j=0;j<associative;j++){
						if (all_element[set][j].priority!=-1){
							all_element[set][j].priority++;
						}
					}
					// Replace the highest priority element
					all_element[set][index] = temp;
				}
			}
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////
	// For Belady
	else if (strncmp(argv[2],"Belady",6)==0){
		printf("Cache algorithm: Belady\n");
		// Begin to read through all address!!!!
		for (int i=0;i<num_address;i++){
			// Create temp struct
			struct element temp;
			temp.priority = 0;
			temp.address = all_address[i];
			// Find the set index
			int set = all_address[i] % set_count;

			printf("%d ",all_address[i]);
			
			// If hit
			int hit_result = find_hit(associative,all_element[set],temp);

			if (hit_result!=-1){
				printf("(hit)\n");
				hit_count++;
				// Add priority to all non -1 elements
				for (int j=0;j<associative;j++){
					if (all_element[set][j].priority!=-1){
						all_element[set][j].priority++;
					}
				}
				// Reset the element we hit to 0
				all_element[set][hit_result].priority = 0;
			}
			else{ // If miss
				printf("(miss)\n");
				// If the cache is not full
				if (already_full(associative,all_element[set])==0){
					// Find place to insert
					int index = 0;
					for (int j=0;j<associative;j++){
						if (all_element[set][j].priority==-1){
							index = j;
							break;
						}
					}
					// Increment all priority to all non -1
					for (int j=0;j<associative;j++){
						if (all_element[set][j].priority!=-1){
							all_element[set][j].priority++;
						}
					}
					// Do the insert
					all_element[set][index] = temp;
				}

				// If the cache already full
				else{
					// Record all distance of next show out
					int all_distance[associative];
					for (int j=0;j<associative;j++){
						all_distance[j] = find_distance(num_address, all_address, 
														all_element[set][j].address, i);
					}
					// Record how many of them exist afterward
					int later_exist = 0;
					int index = -1;
					for (int j=0;j<associative;j++){
						if (all_distance[j]!=-1){
							later_exist++;
							index = j; // Store the index of address exist later
						}
					}
					// If non of them exist later
					if (later_exist==0){
						// Replace the one with smallest address
						int min_index = 0; // store the index of the min address
						int min = all_element[set][0].address; // store the min address
						for (int j=0;j<associative;j++){
							if (all_element[set][j].address<min){
								min_index = j; // update min index
								min = all_element[set][j].address; // upadte min address
							}
						}
						// Increment all priority to all non -1
						for (int j=0;j<associative;j++){
							if (all_element[set][j].priority!=-1){
								all_element[set][j].priority++;
							}
						}
						// Replace the element at the min index
						all_element[set][min_index] = temp;
					}
					// If only one exist later
					else if (later_exist==1){
						// Replace that one (stored index)
						// Increment all priority to all non -1
						for (int j=0;j<associative;j++){
							if (all_element[set][j].priority!=-1){
								all_element[set][j].priority++;
							}
						}
						// Replace the element at index
						all_element[set][index] = temp; // index is the only place where has later exist
					}
					// If all exist later
					else{
						// Find the the one has largest distance
						int max = all_distance[0];
						int max_index = 0;
						for (int j=0;j<associative;j++){
							if (all_distance[j]>max){
								max = all_distance[j];
								max_index = j;
							}
						}
						// Increment all priority to all non -1
						for (int j=0;j<associative;j++){
							if (all_element[set][j].priority!=-1){
								all_element[set][j].priority++;
							}
						}
						// Replace it
						all_element[set][max_index] = temp;
					}
				}
			}
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////
	// Not LRU or Belady
	else{
		fprintf(stderr, "Invalid command line input!(LRU or Belady)\n");
		return EXIT_FAILURE;
	}
	printf("Cache accesses: %d\n", num_address);
	printf("Cache hits: %0.0f\n", hit_count);
	printf("Cache misses: %0.0f\n", num_address-hit_count);
	printf("Overall hit rate: %f\n", hit_count/num_address);
	//for (i=0;i<num_address;i++){printf("%d\n",all_address[i]);}
	return EXIT_SUCCESS;
};