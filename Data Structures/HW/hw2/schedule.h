//File:    schedule.h
//Purpose: Header file with declaration of the Schedule class, 
//			including member functions and member variables

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

class Schedule{
public:
	Schedule(std::string aCRN, std::string aDepartment, std::string aCourseCode, 
				std::string aTitle, char aDay, std::string aStartTime, 
				std::string aEndTime, std::string aRoom);

	//ACCESSORS
	int getCRN() const;
	std::string getDepartment() const;
	std::string getCourseCode() const;
	std::string getTitle() const;
	std::string getDay() const;
	std::string getStartTime() const;
	std::string getEndTime() const;
	std::string getRoom() const;

	//MODIFIERS
	void setCRN(int aCRN);
	void setDepartment(std::string aDepartment);
	void setCourseCode(std::string aCourseCode);
	void setTitle(std::string aTitle);
	void setDay(std::string aDay);
	void setStartTime(std::string aStartTime);
	void setEndTime(std::string aEndTime);
	void setRoom(std::string aRoom);

	//Other member functions
	int time_24() const;
	int time_24_end() const;
	int coursecode_int() const;
	void print_room(std::ofstream &o_str, int max_Title, int max_Day);
	void print_dept(std::ofstream &o_str, int max_Title, int max_Day);
	void print_custom(std::ofstream &o_str, int max_Title, int max_Day);


private:
	//REPRESENTATION(member variable)
	int CRN;
	std::string department; 
	std::string coursecode;
	std::string title; 
	std::string day;
	std::string starttime; 
	std::string endtime;
	std::string room;
};

//Other functions
bool course_sort (const Schedule& a, const Schedule& b);
bool room_sort(const std::vector<Schedule>& a, const std::vector<Schedule>& b);
bool custom_sort(const Schedule& a, const Schedule& b);
bool department_sort(const Schedule& a, const Schedule& b);
int sort_by_day(const Schedule& a);



