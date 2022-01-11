#include "Job.h"
#include "TimeQueue.h"

// Function that swap two element in the vector
void TimeQueue::swap(int child, int parent){
	Job* temp = time_queue[parent];
    time_queue[parent] = time_queue[child];
    time_queue[child] = temp;
    // Also update the hook
    time_queue[child]->time_hook=parent;
    time_queue[parent]->time_hook=child;
}

// Go from child upward if the parent is bigger in time stamp
void TimeQueue::percolate_up(int child){
	int parent = (child-1)/2;
	while (parent >= 0){
		if (time_queue[child]->timestamp < time_queue[parent]->timestamp){
			swap(parent, child);
	        // update child and parent
	        child = parent;
	        parent = (child - 1) / 2;
		}
		else
			break;
	}
}

// Go from the node down check if the node is smaller than the smaller child
// If not swap them
void TimeQueue::percolate_down(int node){
    int left_child = 2 * node + 1;
    int right_child, child;
    // while the is still a left node
    while (left_child < time_queue.size()) { 
      // choose the child to compare against
      	right_child = 2 * node + 2;
      	// Decide which child is smaller
      	if (right_child < time_queue.size() && 
      			time_queue[right_child]->timestamp < 
      				time_queue[left_child]->timestamp){
        	child = right_child;
    	}
      	else
        	child = left_child;
      	// check to see if we need reordering of the heap
      	if (time_queue[child]->timestamp < time_queue[node]->timestamp) { 
      		// we need to swap these two elements
        	swap(node, child);
        	node = child;
        	// update the left child
        	left_child = 2 * node + 1;
      	} 
      	else
      		break; // reordering of the heap is complete
    }
}

// Add a job to the time queue
void TimeQueue::push(Job* job){
	time_queue.push_back(job);
	// Update hook and pointer
	job->time_hook = time_queue.size()-1;
	job->time_ptr = this;
	// Make sure the new job is at the correct spot
	percolate_up(time_queue.size()-1);
}

// Remove the first element in the vector
Job* TimeQueue::pop(){
	// Save the item to pop for return value
	Job* temp = time_queue[0];
	// Replace the first element with the last element
	time_queue[0] = time_queue[time_queue.size()-1];
	time_queue[0]->time_hook = 0;
	// Remove the last one
	time_queue.pop_back();
	// Move the new first element to correct spot
	percolate_down(0);
	// Return the poped element
	return temp;
}

// Return the first element in the time queue
Job* TimeQueue::top() const{
	return time_queue[0];
}

// Printing in format
void TimeQueue::printHeap(std::ostream& out) const{
	out << "TQ " << time_queue.size() << " jobs:" << std::endl;
	for (int i=0; i<time_queue.size(); i++){
		out << "\t" << time_queue[i];
	}
}

// Remove the job at required position
void TimeQueue::remove_from_index(tq_hook hook){
	// Set new value
	time_queue[hook] = time_queue[time_queue.size()-1];
	time_queue[hook]->time_hook = hook;
	time_queue.pop_back();
	// Sort everything after the hook
	percolate_down(hook);
}