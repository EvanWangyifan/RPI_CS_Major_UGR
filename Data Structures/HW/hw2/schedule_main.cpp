//Program: schedule_main.cpp
//Purpose: Use of Schedule class.

#include "schedule.h"

//This function take a vector containing all the Schedule objects
//It returns a big vector contains multiple smaller vectors for each room
std::vector< std::vector<Schedule> > 
split_by_room(const std::vector<Schedule> schedule_all){
	std::vector< std::vector<Schedule> > result;
	//Enter the vector containing all the object.
	for (int i = 0; i < schedule_all.size(); i++){
		bool already_in = false;//If the name of room is found.
		int position = 0;//The temp position of the object we are finding.
		//Find out if there is existing small vector for the same room name.
		for (int j = 0; j < result.size(); j++){
			if (schedule_all[i].getRoom() == result[j][0].getRoom()){
				already_in = true;//The room is found.
				position = j;//Temp position
			}
		}
		//If there is small vector for the room name, add the object into it.
		if (already_in == true){
			result[position].push_back(schedule_all[i]);
		}
		//If it is not already in, create a new one and add the object in it,
		// and push back it to the output one.
		else {
			std::vector<Schedule> new_one;
			new_one.push_back(schedule_all[i]);
			result.push_back(new_one);
		}
	}
	return result;
}

//This function take a vector containing all the Schedule objects
//It returns a big vector contains multiple smaller vectors for each department
std::vector< std::vector<Schedule> > 
split_by_dept(const std::vector<Schedule> schedule_all){
	std::vector< std::vector<Schedule> > result;
	//Enter the vector containing all the object.
	for (int i = 0; i < schedule_all.size(); i++){
		bool already_in = false;//If the name of dept is found.
		int position = 0;//The temp position of the object we are finding.
		//Find out if there is existing small vector for the same dept name.
		for (int j = 0; j < result.size(); j++){
			if (schedule_all[i].getDepartment() == result[j][0].getDepartment()){
				already_in = true;//The room is found.
				position = j;//Temp position
			}
		}
		//If there is small vector for the dept name, add the object into it.
		if (already_in == true){
			result[position].push_back(schedule_all[i]);
		}
		//If it is not already in, create a new one and add the object in it,
		// and push back it to the output one.
		else {
			std::vector<Schedule> new_one;
			new_one.push_back(schedule_all[i]);
			result.push_back(new_one);
		}
	}
	return result;
}

//The function print the header lines for dept command
void print_dept_header(std::ofstream &out_str, int max_title, int max_day, 
						std::string deptname){
	out_str << "Dept" << " " << deptname << std::endl;
	out_str << "Coursenum  " << "Class Title" 
	<< std::string(max_title+2-11, ' ') << "Day" 
	<< std::string(max_day+2-3, ' ') << "Start Time  " << "End Time" 
	<< std::endl;
	out_str << std::string(9, '-') << "  " << std::string(max_title, '-') 
	<< "  " << std::string(max_day, '-') << "  " << std::string(10, '-') 
	<< "  " << std::string(8, '-') << std::endl;
}

//The function print the header lines for room command
void print_room_header(std::ofstream &out_str, int max_title, int max_day, 
						std::string roomname){
	out_str << "Room" << " " << roomname << std::endl;
	out_str << "Dept  " << "Coursenum  " << "Class Title" 
	<< std::string(max_title+2-11, ' ') << "Day" 
	<< std::string(max_day+2-3, ' ') << "Start Time  " << "End Time" 
	<< std::endl;
	out_str << std::string(4, '-') << "  " << std::string(9, '-') << "  "
	<< std::string(max_title, '-') << "  " << std::string(max_day, '-') 
	<< "  " << std::string(10, '-') << "  " << std::string(8, '-') 
	<< std::endl;
}

//The function print the header lines for custom command
void print_custom_header(std::ofstream &out_str, int max_title, int max_day){
	out_str << "Sort by week day / start time / end time / coursenum" 
	<< std::endl;
	out_str << "Day" << std::string(max_day+2-3, ' ') << "Start Time  " 
	<< "End Time  "<< "Coursenum  " << "Class Title" 
	<< std::string(max_title+2-11, ' ') << std::endl;
	out_str << std::string(max_day, '-') << "  " << std::string(10, '-') 
	<< "  " << std::string(8, '-') << "  " << std::string(9, '-') << "  " 
	<< std::string(max_title-2, '-')  << std::endl;
}

//Check the number of command line argument
int main(int argc, char* argv[]){
  //Report error if the length of command line is wrong.
  if (argc != 4 && argc != 5){
  	std::cerr << "Invalid Command" << std::endl;
  	exit(1);
  }
  //Read the file
  std::ifstream in_str(argv[1]);
  //Print error if cannot open the file to read
  if (!in_str.good()){
    std::cerr << "Can't open " << argv[1] << " to read.\n";
    exit(1);
  }

  std::vector<Schedule> schedule;
  std::string aCRN;
  std::string aDepartment;
  std::string aCourseCode;
  std::string aTitle;
  std::string aDay;
  std::string aStartTime;
  std::string aEndTime;
  std::string aRoom;
  //Read each line of the file put the information to a object.
  while (in_str >> aCRN >> aDepartment >> aCourseCode >> aTitle >> aDay >> 
  		aStartTime >> aEndTime >> aRoom){
  	//Put each line of the information to a object.
    for (int i = 0; i < aDay.size(); i++){
    	Schedule temp = Schedule(aCRN, aDepartment, aCourseCode, 
    							aTitle, aDay[i], aStartTime, aEndTime, aRoom);
    	//Put the object into the vector schedule.
    	schedule.push_back(temp);
    }
  }

  //Open the out file
  std::ofstream out_str(argv[2]);
  //Print error if connot open the file to write
  if (!out_str.good()) {
    std::cerr << "Can't open " << argv[2] << " to write.\n";
    exit(1);
  }

  //Find the vector that has the room name we are finding 
  // and store it in to_print.
  std::vector<Schedule> to_print;
  int max_title = 0;
  int max_day = 0;
  //Split the vector by room and dept.
  std::vector< std::vector<Schedule> > 
  schedule_split_room = split_by_room(schedule);
  std::vector< std::vector<Schedule> > 
  schedule_split_dept = split_by_dept(schedule);
  //If the vector is empty, which means the file is empty.
  // Output the file with "No data available."
  if (schedule.size() == 0){
  	out_str << "No data available." << std::endl;
  }
  //Normal file reading
  else{
  	  //If we have five command.
	  if (argc == 5){
	  	bool room_found = false;//Flag if the room in the command line is found.
		if ((std::string)argv[3] == "room"){
			//Find the small vector for the room we are finding.
			for (int i = 0; i < schedule_split_room.size(); i++){
				if (schedule_split_room[i][0].getRoom() == argv[4]){
					to_print = schedule_split_room[i];
					room_found = true;//Find the room in the command.
					break;//Time saving.
				}
			}
			//If we can't find the room command provides, print emptyfile output.
			if (room_found == false){
				out_str << "No data available." << std::endl;
			}
		}
		if ((std::string)argv[3] == "dept"){
			bool dept_found = false;//Flag if the dept provided is found.
			//Find the small vector for the dept we are finding.
			for (int i = 0; i < schedule_split_dept.size(); i++){
				if (schedule_split_dept[i][0].getDepartment() == argv[4]){
					to_print = schedule_split_dept[i];
					dept_found = true;//Find the dept in the command.
					break;//Time saving.
				}
			}
			//If we can't find the dept command provides, print emptyfile output.
			if (dept_found == false){
				out_str << "No data available." << std::endl;
			}
		}
		//For each small vector, find out the maximum length for title and day.
		//The minimum length (head words).
		max_title = 11;
	  	max_day = 3;
		for (int i = 0; i < to_print.size(); i++){
			if (to_print[i].getTitle().size() > max_title){
			max_title = to_print[i].getTitle().size();
			}
			if (to_print[i].getDay().size() > max_day){
			max_day = to_print[i].getDay().size();
			}
		}
		if ((std::string)argv[3] == "room"){
			//Sort the to_print vector by course sort(since only one room).
			std::sort(to_print.begin(), to_print.end(), course_sort);
			//Print the header lines using the max width we calculate.
			print_room_header(out_str, max_title, max_day, argv[4]);
			//Print out each object.
			for (int i = 0; i < to_print.size(); i++){
				to_print[i].print_room(out_str, max_title, max_day);
			}
			//Print the entries number.
			out_str << to_print.size() << " entries" << std::endl << std::endl;
		}
		else if ((std::string)argv[3] == "dept"){
			//Sort the to_print vector by dept sort.
			std::sort(to_print.begin(), to_print.end(), department_sort);
			//Print the header lines using the max width we calculate.
			print_dept_header(out_str, max_title, max_day, argv[4]);
			//Print out each object.
			for (int i = 0; i < to_print.size(); i++){
				to_print[i].print_dept(out_str, max_title, max_day);
			}
			//Print the entries number.
			out_str << to_print.size() << " entries" << std::endl << std::endl;
		}
		//If the fourth command is not room or dept, report command error.
		else{
			std::cerr << "Invalid Command" << std::endl;
		}
	  }
	  //If we have 4 command and the last command is room (print all rooms).
	  else if (argc == 4 && (std::string)argv[3] == "room"){
	  	//Sort the splited vector by room name first.
	  	std::sort(schedule_split_room.begin(), schedule_split_room.end(), 
	  			room_sort);
	  	for (int i = 0; i < schedule_split_room.size(); i++){
	  		//The minimum length (head words).
	  		max_title = 11;
	  		max_day = 3;
	  		//Calculate the max length of title and day lines.
	  		for (int j = 0; j < schedule_split_room[i].size(); j++){
				if (schedule_split_room[i][j].getTitle().size() > max_title){
				max_title = schedule_split_room[i][j].getTitle().size();
				}
				if (schedule_split_room[i][j].getDay().size() > max_day){
				max_day = schedule_split_room[i][j].getDay().size();
				}
	  		}
	  		//Sort every small vectors by course.
	  		std::sort(schedule_split_room[i].begin(), 
	  				schedule_split_room[i].end(), course_sort);
	  		//Print the header lines for room command.
			print_room_header(out_str, max_title, max_day, 
								schedule_split_room[i][0].getRoom());
			//Print each object.
			for (int j = 0; j < schedule_split_room[i].size(); j++){
				schedule_split_room[i][j].print_room(out_str, max_title, max_day);
			}
			//Print the entries number.
			out_str << schedule_split_room[i].size() << " entries" 
			<< std::endl << std::endl;
	  	}
	  }
	  //If we have 4 command and the last command is custom
	  else if (argc == 4 && (std::string)argv[3] == "custom"){
	  	//Sort the vector by custom requirement.
	  	//Which is: Daytime/Starttime/Endtime/Coursecode.
	  	std::sort(schedule.begin(), schedule.end(), custom_sort);
	  	//Find the maximum length of title and day.
	  	max_title = 11;
	  	max_day = 3;
		for (int i = 0; i < schedule.size(); i++){
			if (schedule[i].getTitle().size() > max_title){
			max_title = schedule[i].getTitle().size();
			}
			if (schedule[i].getDay().size() > max_day){
			max_day = schedule[i].getDay().size();
			}
		}
		//Print the header lines.
		print_custom_header(out_str, max_title, max_day);
		//Print out each object by order.
		for (int i = 0; i < schedule.size(); i++){
				schedule[i].print_custom(out_str, max_title, max_day);
			}
		//Print the number of entries.
		out_str << schedule.size() << " entries" << std::endl << std::endl;
	  }
	  //If the command is not room, roomname/room/dept, or deptname, report error.
	  else{
	 	std::cerr << "Invalid Command" << std::endl;
	  }
  }
  return 0;//Everything goes fine.
}