#include <string>

class Time{

public:
	Time();
	Time(int hour, int minute, int second);

	//ACCESSOR
	int getHour() const;
	int getMinute() const;
	int getSecond() const;

	void setHour(int hour);
	void setMinute(int minute);
	void setSecond(int second);
	void PrintAMPM() const;


private:
	int h;
	int m;
	int s;

};

bool IsEarlierThan(const Time& t1, const Time& t2);