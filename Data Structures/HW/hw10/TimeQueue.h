#ifndef __TIMEQ_H_
#define __TIMEQ_H_
//You can change/add to these #includes
#include <ctime>
#include <vector>
#include <string>
#include <iostream>


typedef int tq_hook; //Used to point to a location in the heap, fill in the "???"

//Forward declaration of Job class, do not remove this line, do not change.
class Job;

//Promise to declare stream output operator for internal vector heap representation
std::ostream& operator<<(std::ostream& out, const std::vector<Job*>& heap);

class TimeQueue{

//////////FILL THIS IN///////////
public:
	void push(Job* job);
	Job* pop();
	Job* top() const;
	void swap(int child, int parent);
	void printHeap(std::ostream& out) const;
	void percolate_up(int child);
	void percolate_down(int node);
	void remove_from_index(int hook);
private:
	std::vector<Job*> time_queue;
};

#endif
