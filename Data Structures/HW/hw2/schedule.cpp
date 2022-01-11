//File:    schedule.cpp
//Purpose: Implementation file for the Schedule class.

#include "schedule.h"

//Constructor
Schedule::Schedule(std::string aCRN, std::string aDepartment, 
	std::string aCourseCode, std::string aTitle, char aDay, 
	std::string aStartTime, std::string aEndTime, std::string aRoom){
	CRN = std::stoi(aCRN);
	department = aDepartment;
	coursecode = aCourseCode;
	title = aTitle;
	//Converting the First letter of Weekday to the full word.
	if (aDay == 'M'){
		day = "Monday";
	}
	else if (aDay == 'T'){
		day = "Tuesday";
	}
	else if (aDay == 'W'){
		day = "Wednesday";
	}
	else if (aDay == 'R'){
		day = "Thursday";
	}
	else if (aDay == 'F'){
		day = "Friday";
	}
	else{
		std::cerr << "Not in the week." << std::endl; 
	}
	starttime = aStartTime;
	endtime = aEndTime;
	room = aRoom;
}

//All the ACCESSORS
int Schedule::getCRN() const{
	return CRN;
}

std::string Schedule::getDepartment() const{
	return department;
}

std::string Schedule::getCourseCode() const{
	return coursecode;
}
	
std::string Schedule::getTitle() const{
	return title;
}
	
std::string Schedule::getDay() const{
	return day;
}
	
std::string Schedule::getStartTime() const{
	return starttime;
}
	
std::string Schedule::getEndTime() const{
	return endtime;
}
	
std::string Schedule::getRoom() const{
	return room;
}

//All the MODIFIERS
void Schedule::setCRN(int crn){
	CRN = crn;
}

void Schedule::setDepartment(std::string dep){
	department = dep;
}

void Schedule::setCourseCode(std::string code){
	coursecode = code;
}

void Schedule::setTitle(std::string tl){
	title = tl;
}

void Schedule::setDay(std::string d){
	day = d;
}

void Schedule::setStartTime(std::string start){
	starttime = start;
}

void Schedule::setEndTime(std::string end){
	endtime = end;
}

void Schedule::setRoom(std::string r){
	room = r;
}
//This function convert the AM/PM base start time into 24 hour base time 
// and return it as a 4 digit int.
int Schedule::time_24() const{
	int time_a;
	//If 12 AM, set it to 0 hour in 24 hour base time.
	if (getStartTime()[5] == 'A' && 
		getStartTime().substr(0,2) == "12"){
		time_a = 0;
	}
	//If AM, get the hour part of the time.
	else if (getStartTime()[5] == 'A'){
		time_a = std::stoi(getStartTime().substr(0,2));
	}
	//If 12 PM, set the hour part to 12 in 24 hour base time.
	else if (getStartTime()[5] == 'P' && 
			getStartTime().substr(0,2) == "12"){
		time_a = 12;
	}
	//If other PM, get the hour part and add to 12 to get the hour part in 
	// 24 base time.
	else{
		time_a = std::stoi(getStartTime().substr(0,2)) + 12;
	}
	time_a = time_a * 100 + std::stoi(getStartTime().substr(3,2));
	return time_a;
}

//This function convert the AM/PM base end time into 24 hour base time 
// and return it as a 4 digit int.
int Schedule::time_24_end() const{
	int time_a;
	//If 12 AM, set it to 0 hour in 24 hour base time.
	if (getEndTime()[5] == 'A' && 
		getEndTime().substr(0,2) == "12"){
		time_a = 0;
	}
	//If AM, get the hour part of the time.
	else if (getEndTime()[5] == 'A'){
		time_a = std::stoi(getEndTime().substr(0,2));
	}
	//If 12 PM, set the hour part to 12 in 24 hour base time.
	else if (getEndTime()[5] == 'P' && 
			getEndTime().substr(0,2) == "12"){
		time_a = 12;
	}
	//If other PM, get the hour part and add to 12 to get the hour part in 
	// 24 base time.
	else{
		time_a = std::stoi(getEndTime().substr(0,2)) + 12;
	}
	//Convert the hour and minute into 4 digit integer for comparison.
	time_a = time_a * 100 + std::stoi(getEndTime().substr(3,2));
	return time_a;
}

//This function convert the string course code into 6 digit int and return it.
int Schedule::coursecode_int() const{
	int code_int;
	code_int = std::stoi(getCourseCode().substr(0,4)+getCourseCode().substr(5,2));
	return code_int;
}
//Return true if the objects were sorted correctly by command request for each 
//room by the order: day of week, start time, course code. 
bool course_sort (const Schedule& a, const Schedule& b){
	return (sort_by_day(a) < sort_by_day(b))|| 
	(a.getRoom() == b.getRoom() && sort_by_day(a) == sort_by_day(b) && 
		a.time_24() < b.time_24()) || 
	(a.getRoom() == b.getRoom() && sort_by_day(a) == sort_by_day(b) && 
		a.time_24() == b.time_24() && a.coursecode_int() < b.coursecode_int());
}
//Return true if the objects were sorted correctly by room name.
bool room_sort (const std::vector<Schedule>& a, const std::vector<Schedule>& b){
	return a[0].getRoom() < b[0].getRoom();
}

//Renturn true if the objects were sorted correctly by department command request.
//The order is: course code, week day, and start time.
bool department_sort(const Schedule& a, const Schedule& b){
	return (a.coursecode_int() < b.coursecode_int() ||
			(a.coursecode_int() == b.coursecode_int() && 
				sort_by_day(a) < sort_by_day(b)) ||
			(a.coursecode_int() == b.coursecode_int() && 
				sort_by_day(a) == sort_by_day(b) && a.time_24() < b.time_24()));
}

//Renturn true if the objects were sorted correctly by custom command request.
//The order is: week day, start time, and end time, course code.
bool custom_sort(const Schedule& a, const Schedule& b){
	return (sort_by_day(a) < sort_by_day(b) ||
			(sort_by_day(a) == sort_by_day(b) && a.time_24() < b.time_24()) ||
			(sort_by_day(a) == sort_by_day(b) && a.time_24() == b.time_24() 
				&& a.time_24_end() < b.time_24_end()) ||
			(sort_by_day(a) == sort_by_day(b) && a.time_24() == b.time_24() 
				&& a.time_24_end() == b.time_24_end() 
				&& a.coursecode_int() < b.coursecode_int()));
}

//This function return the position of week day for comparison in sort algorithm.
int sort_by_day(const Schedule& a){
	std::string day_order = "MondayTuesdayWednesdayThursdayFriday";
	return day_order.find(a.getDay());
}

//Print each schedule object following the format of command room requires.
void Schedule::print_room(std::ofstream &o_str, int max_Title, int max_Day){
	o_str << std::setw(6) << std::left << department << std::setw(11) 
	<< coursecode << std::setw(max_Title+2) << title << std::setw(max_Day+2) 
	<< day << std::setw(12)<< starttime << std::setw(8) << endtime << std::endl;
}

//Print each schedule object following the format of command dept requires.
void Schedule::print_dept(std::ofstream &o_str, int max_Title, int max_Day){
	o_str << std::setw(11) << std::left << coursecode << std::setw(max_Title+2) 
	<< title << std::setw(max_Day+2) << day << std::setw(12) << starttime 
	<< std::setw(8) << endtime<< std::endl;
}
//Print each schedule object following the format of command custom requires.
void Schedule::print_custom(std::ofstream &o_str, int max_Title, int max_Day){
	o_str << std::setw(max_Day+2) << std::left << day << std::setw(12) 
	<< starttime << std::setw(10) << endtime << std::setw(11) << coursecode 
	<< std::setw(max_Title+2) << title << std::endl;
}
