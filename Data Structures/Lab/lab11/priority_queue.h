#ifndef priority_queue_h_
#define priority_queue_h_

#include <iostream>
#include <vector>
#include <cassert>


template <class T>
class priority_queue {
private:
  std::vector<T> m_heap;

  void swap(int child, int parent) {
    T temp = m_heap[parent];
    m_heap[parent] = m_heap[child];
    m_heap[child] = temp;
  }
public:
  priority_queue() {}

  priority_queue( std::vector<T> const& values ){
  if (int(m_heap.size()) == 0) // empty vector
    return;
  for (int node = (m_heap.size()-1)/2; node >=0 ; --node)
    percolate_down(node);
  }

  const T& top() const 
  {
    assert( !m_heap.empty() );
    return m_heap[0]; 
  }

  void push(const T &entry) {
    m_heap.push_back(entry);
    percolate_up(m_heap.size() - 1);
  }


  void pop() {
    assert(!m_heap.empty());
    T new_root_element = m_heap[m_heap.size() - 1];
    // replace root node's value with the value of the last leaf
    m_heap[0] = new_root_element;
    // delete the last element
    m_heap.pop_back();
    // run percolate down on the new root node's value
    percolate_down(0);
  }

  void percolate_up(int child) {
    int parent = (child - 1) / 2;
    while (parent >= 0) { // while the child still has a parent
      if (m_heap[child] < m_heap[parent]) { // swap the nodes
        swap(parent, child);
        // update child and parent
        child = parent;
        parent = (child - 1) / 2;
      } else break; // the heap is all in order
    }
  }
  void percolate_down(int node) {
    int left_child = 2 * node + 1;
    int right_child, child;

    while (left_child < m_heap.size()) { // while the is still a left node
      // choose the child to compare against
      right_child = 2 * node + 2;

      if (right_child < m_heap.size() && m_heap[right_child] < m_heap[left_child])
        child = right_child;
      else
        child = left_child;

      // check to see if we need reordering of the heap
      if (m_heap[child] < m_heap[node]) { // we need to swap these two elements
        swap(node, child);
        node = child;

        // update the left child
        left_child = 2 * node + 1;

      } else break; // reordering of the heap is complete
    }
  }
  int size() { return m_heap.size(); }
  bool empty() { return m_heap.empty(); }


  //  The following three functions are used for debugging.

  //  Check to see that internally the heap property is realized.
  bool check_heap( )
  {
    return this->check_heap( this->m_heap );
  }

  //  Check an external vector to see that the heap property is realized.
  bool check_heap(const std::vector<T> &heap) {
    return this->check_heap(heap, 0);
  }

  bool check_heap(const std::vector<T> &heap, int node) {
    int left_child = 2 * node + 1;
    int right_child = 2 * node + 2;
    if (left_child >= m_heap.size() && right_child >= m_heap.size()) {
      return true;
    } else if (right_child >= m_heap.size()) {
      return check_heap(heap, left_child);
    } else if (left_child >= m_heap.size()) {
      return check_heap(heap, right_child);
    } else {
      bool left_check = check_heap(heap, left_child);
      bool right_check = check_heap(heap, right_child);
      return left_check && right_check;
    }
  }
  //  A utility to print the contents of the heap.  Use it for debugging.
  void print_heap( std::ostream & ostr ){
    for ( unsigned int i=0; i<m_heap.size(); ++i )
      ostr << i << ": " << m_heap[i] << std::endl;
  }
  
};


template <class T>
void heap_sort( std::vector<T> & v )
{
}

#endif
