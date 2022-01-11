#include "time.h"
#include <iostream>

Time::Time(){
	h = 0;
	m = 0;
	s = 0;
}

Time::Time(int hour, int minute, int second){
	h = hour;
	m = minute;
	s = second;
}
int Time::getHour() const{
	return h;
}
int Time::getMinute() const{
	return m;
}
int Time::getSecond() const{
	return s;
}
void Time::setHour(int hour){
	h = hour;
}
void Time::setMinute(int minute){
	m = minute;
}
void Time::setSecond(int second){
	s = second;
}
void Time::PrintAMPM() const{
	std::string ampm = " am";
	int temp_h = h;
	if (h == 12){
		ampm = " pm";
	}
	else if (h == 0){
		temp_h = 12;
		ampm = " am";
	}
	if (h > 12){
		temp_h -= 12;
		ampm = " pm";
	}
	std::cout << temp_h << ':' << m << ':' << s << ampm << std::endl;
}

bool IsEarlierThan(const Time& t1, const Time& t2){
	return ((t1.getHour() < t2.getHour()) ||
			(t1.getHour() == t2.getHour() && t1.getMinute() < t2.getMinute()) ||
			(t1.getHour() == t2.getHour() && t1.getMinute() == t2.getMinute() && t1.getSecond() < t2.getSecond()));
}
