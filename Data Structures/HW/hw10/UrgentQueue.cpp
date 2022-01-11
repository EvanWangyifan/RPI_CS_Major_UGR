#include "Job.h"
#include "UrgentQueue.h"

// Function that swap two element in the vector
void UrgentQueue::swap(int child, int parent){
	Job* temp = urgent_queue[parent];
    urgent_queue[parent] = urgent_queue[child];
    urgent_queue[child] = temp;
    // Also update the hook
    urgent_queue[child]->urgent_hook=parent;
    urgent_queue[parent]->urgent_hook=child;
}

// Go from child upward if the parent is smaller in priority
void UrgentQueue::percolate_up(int child){
	int parent = (child-1)/2;
	while (parent >= 0){
		if (urgent_queue[child]->priority > urgent_queue[parent]->priority){
			swap(parent, child);
	        // update child and parent
	        child = parent;
	        parent = (child - 1) / 2;
		}
		else
			break;
	}
}

// Go from the node down check if the node is bigger than the bigger child
// If not swap them
void UrgentQueue::percolate_down(int node){
    int left_child = 2 * node + 1;
    int right_child, child;
    // while the is still a left node
    while (left_child < urgent_queue.size()) { 
      // choose the child to compare against
      	right_child = 2 * node + 2;
      	// Decide which child is bigger
      	if (right_child < urgent_queue.size() && 
      			urgent_queue[right_child]->priority > 
      				urgent_queue[left_child]->priority){
        	child = right_child;
    	}
      	else
        	child = left_child;
      	// check to see if we need reordering of the heap
      	if (urgent_queue[child]->priority > urgent_queue[node]->priority) { 
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

// Add a job to the urgent queue
void UrgentQueue::push(Job* job){
	urgent_queue.push_back(job);
	// Update hook and pointer
	job->urgent_hook = urgent_queue.size()-1;
	job->priority_ptr = this;
	// Make sure the new job is at the correct spot
	percolate_up(urgent_queue.size()-1);
}

// Remove the first element in the vector
Job* UrgentQueue::pop(){
	// Save the item to pop for return value
	Job* temp = urgent_queue[0];
	// Replace the first element with the last element
	urgent_queue[0] = urgent_queue[urgent_queue.size()-1];
	urgent_queue[0]->urgent_hook = 0;
	// Remove the last one
	urgent_queue.pop_back();
	// Move the new first element to correct spot
	percolate_down(0);
	// Return poped element
	return temp;
}

// Return the first element in the urgent queue
Job* UrgentQueue::top() const{
	return urgent_queue[0];
}

// Printing in format
void UrgentQueue::printHeap(std::ostream& out) const{
	out << "UQ " << urgent_queue.size() << " jobs:" << std::endl;
	for (int i=0; i<urgent_queue.size(); i++){
		out <<  "\t" << urgent_queue[i];
	}
}

// Remove the job at required position
void UrgentQueue::remove_from_index(uq_hook hook){
	// Set new value
	urgent_queue[hook] = urgent_queue[urgent_queue.size()-1];
	urgent_queue[hook]->urgent_hook = hook;
	urgent_queue.pop_back();
	// Sort everything after the hook
	percolate_down(hook);
}