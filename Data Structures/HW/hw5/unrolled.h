#ifndef UnrolledLL_h_
#define UnrolledLL_h_
// A simplified implementation of the STL list container class,
// including the iterator, but not the const_iterators.  Three
// separate classes are defined: a Node class, an iterator class, and
// the actual list class.  The underlying list is doubly-linked, but
// there is no dummy head node and the list is not circular.
#include <iostream>
#include <cassert>

// Maximum size of the array
const int NUM_ELEMENTS_PER_NODE = 6;
// -----------------------------------------------------------------
// NODE CLASS
template <class T>
class Node {
public:
  // Default empty node 
  Node() : next_(NULL), prev_(NULL), num_elements_(0) {}
  // Copy existing node (passing how many elements are already in the node)
  Node(const T new_elements[NUM_ELEMENTS_PER_NODE], int num) {
    for (int i=0;i<num;i++){
      elements_[i]=new_elements[i];
    }
    num_elements_=num;
    next_=NULL;
    prev_=NULL;
  }
  // Printing elements in a single node
  void print(std::ostream& a){
    a << "node:[" << num_elements_ << "] ";
    for (int i=0;i<num_elements_;i++){
      a << elements_[i] << ' ';
    }
    a << std::endl;
  }

  // REPRESENTATION
  int num_elements_; // number of elements in the array(<=6)
  T elements_[NUM_ELEMENTS_PER_NODE]; // array that contains all elements
  Node<T>* next_;
  Node<T>* prev_;
};

// A "forward declaration" of this class is needed
template <class T> class UnrolledLL;

// -----------------------------------------------------------------
// LIST ITERATOR
template <class T>
class list_iterator {
public:
  // default constructor, copy constructor, assignment operator, & destructor
  list_iterator(Node<T>* p=NULL, int offset=0) : ptr_(p), offset_(offset) {}
  // NOTE: the implicit compiler definitions of the copy constructor,
  // assignment operator, and destructor are correct for this class

  // dereferencing operator gives access to the value at the array in the node
  T& operator*()  { return ptr_->elements_[offset_];  }

  // increment & decrement operators
  list_iterator<T>& operator++() { // pre-increment, e.g., ++iter
    // When the iterator was pointing to the last element in the array
    // point to the first element in the next node's array
    if (offset_==ptr_->num_elements_ - 1){
      ptr_ = ptr_->next_;
      offset_=0;
    }
    // Normal cases
    else{
      offset_++;
    }
    return *this;
  }
  list_iterator<T> operator++(int) { // post-increment, e.g., iter++
    list_iterator<T> temp(*this);
    // When the iterator was pointing to the last element in the array
    // point to the first element in the next node's array
    if (offset_==ptr_->num_elements_ - 1){
      ptr_ = ptr_->next_;
      offset_=0;
    }
    // Normal cases
    else{
      offset_++;
    }
    return temp;
  }
  list_iterator<T>& operator--() { // pre-decrement, e.g., --iter
    // When the iterator was pointing to the first element in the array
    // point to the last element in the previous node's array
    if (offset_==0){
      ptr_ = ptr_->prev_;
      offset_=ptr_->num_elements_;
    }
    // Normal cases
    else{
      offset_--;
    }
    return *this;
  }
  list_iterator<T> operator--(int) { // post-decrement, e.g., iter--
    list_iterator<T> temp(*this);
    // When the iterator was pointing to the first element in the array
    // point to the last element in the previous node's array
    if (offset_==0){
      ptr_ = ptr_->prev_;
      offset_=ptr_->num_elements_;
    }
    // Normal cases
    else{
      offset_--;
    }
    return temp;
  }
  // the UnrolledLL class needs access to the private ptr_ member variable
  friend class UnrolledLL<T>;

  // Comparions operators are straightforward
  // The iterators will be identical only if when they are pointing to the same
  // Node and same offset value
  bool operator==(const list_iterator<T>& r) const {
    return ptr_ == r.ptr_ && offset_ == r.offset_; }
  // The iterator will be different if they are pointing at different node or
  // different offset value
  bool operator!=(const list_iterator<T>& r) const {
    return ptr_ != r.ptr_ || offset_ != r.offset_; }


private:
  // REPRESENTATION
  Node<T>* ptr_;    // ptr to node in the list
  int offset_; // off set index showing the position in the array
};

// -----------------------------------------------------------------
// LIST CLASS DECLARATION
// Note that it explicitly maintains the size of the list.
template <class T>
class UnrolledLL {
public:
  // default constructor, copy constructor, assignment operator, & destructor
  UnrolledLL() : head_(NULL), tail_(NULL), size_(0) {}
  // Copy from existing list
  UnrolledLL(const UnrolledLL<T>& old) { copy_list(old); }
  UnrolledLL& operator= (const UnrolledLL<T>& old);
  // Destructor
  ~UnrolledLL() { destroy_list(); }

  typedef list_iterator<T> iterator;

  // simple accessors & modifiers
  unsigned int size() const {
    int result = 0;
    Node<T>* temp = head_;
    // Get the number of element from every single node and add them up
    while (temp){
      result += temp->num_elements_;
      temp=temp->next_;
    }
    return result;
  }
  bool empty() const { return head_ == NULL; }
  void clear() { destroy_list(); }

  // read/write access to contents
  const T& front() const { return head_->elements_[0];  }
  T& front() { return head_->elements_[0]; }
  const T& back() const { return tail_->elements_[tail_->num_elements_ - 1]; }
  T& back() { return tail_->elements_[tail_->num_elements_-1]; }
  void print(std::ostream& a);

  // modify the linked list structure
  void push_front(const T& v);
  void pop_front();
  void push_back(const T& v);
  void pop_back();

  iterator erase(iterator itr);
  iterator insert(iterator itr, const T& v);
  iterator begin() { return iterator(head_, 0); }
  iterator end() { return iterator(NULL, 0); }

private:
  // private helper functions
  void copy_list(const UnrolledLL<T>& old);
  void destroy_list();

  //REPRESENTATION
  Node<T>* head_;
  Node<T>* tail_;
  unsigned int size_;
};

// -----------------------------------------------------------------
// LIST CLASS IMPLEMENTATION
template <class T>
UnrolledLL<T>& UnrolledLL<T>::operator= (const UnrolledLL<T>& old) {
  // check for self-assignment
  if (&old != this) {
    destroy_list();
    copy_list(old);
  }
  return *this;
}

// print function for debugging
template <class T>
void UnrolledLL<T>::print(std::ostream& a){
  Node<T>* ptr;
  ptr = head_;
  a << "UnrolledLL, size: " << size() << std::endl;
  // Print all nodes
  while(ptr != NULL){
    ptr->print(a);
    ptr=ptr->next_;
  }

}

// push element v into the front of list
template <class T>
void UnrolledLL<T>::push_front(const T& v) {
  // Create new node containing the v value in the heap for later use
  T new_elements[NUM_ELEMENTS_PER_NODE];
  new_elements[0] = v;
  Node<T>* newp = new Node<T>(new_elements,1);
  // special case: initially empty list
  if (!head_) {
    // Both head and tail point to the new node
    head_ = tail_ = newp;
    ++size_; // Increment size
  } 
  // normal case: at least one node already
  else {
    // if the node we want to push fron is already full
    // put the new node we created earlier in front of the original node
    if (head_->num_elements_==NUM_ELEMENTS_PER_NODE){
      // Make head points to the new node and new node points to the old head
      newp->next_=head_;
      head_->prev_=newp;
      head_=newp;
      ++size_; // Increment size
    }
    // The node we want to push is not full
    else{
      // Save the original node to old for reference to later copying
      T old[NUM_ELEMENTS_PER_NODE];
      for (int i=0; i<NUM_ELEMENTS_PER_NODE;i++){
        old[i]=head_->elements_[i];
      }
      // Put the pushed element in the first spot
      head_->elements_[0]=v;
      // Copy the rest of the original node(which should be one element later)
      for (int i=1; i<NUM_ELEMENTS_PER_NODE;i++){
        head_->elements_[i]=old[i-1];
      }
      head_->num_elements_++; // Not adding node so just increment the element
      delete newp; // Didn't use the node we created earlier, so delete it
    }
  }
}

// Delete the first element from the list
template <class T>
void UnrolledLL<T>::pop_front() {
// for the list that is not empty
  if (head_){
    // if the node is empty after pop, delete the node directly
    if (head_->num_elements_-1==0){
      // Make head pointing to the second node
      Node<T>* temp = head_;
      head_=head_->next_;
      delete temp;
      size_--; 
    }
    // if not empty after pop, delete the first element in the array
    else{
      // Copying one element later in the original array
      for (int i=1; i<head_->num_elements_; i++){
        head_->elements_[i-1]=head_->elements_[i];
      }
      head_->num_elements_--;
    }
  }
}

// Add element v in the back of the list
template <class T>
void UnrolledLL<T>::push_back(const T& v) {
  // Create array with element v in the heap for later use
  T new_elements[NUM_ELEMENTS_PER_NODE];
  new_elements[0] = v;
  Node<T>* newp = new Node<T>(new_elements, 1);
  // special case: initially empty list
  if (!tail_) {
    // Both head and tail points to the only node
    head_ = tail_ = newp;
    ++size_;
  } 
  // normal case: at least one node already
  else {
    // If the last node is already full
    if (tail_->num_elements_==NUM_ELEMENTS_PER_NODE){
      // Connect the new node to the old head and make it the new head
      newp->prev_ = tail_;
      tail_->next_ = newp;
      tail_ = newp;
      ++size_;
    }
    // If not full
    else{
      // Simply make the last element of the array v 
      tail_->elements_[tail_->num_elements_]=v;
      tail_->num_elements_++; // Since not adding new node just increment num
      delete newp; // Not using the new node, delete it
    }
  }
  
}

// Delete the last element in the list
template <class T>
void UnrolledLL<T>::pop_back() {
  // for the list that is not empty
  if (tail_){
    // if the node is empty after pop, delete the node directly
    if (tail_->num_elements_-1==0){
      // Make the previous element the tail and pointing to NULL
      Node<T>* temp = tail_;
      tail_->prev_->next_ = NULL;
      tail_ = tail_ -> prev_;
      delete temp;// Delete the original tail
      size_--;
    }
    // if not empty after pop, delete the last element in the array
    else{
      tail_->num_elements_--;// Didn't change node number just element in array
    }
  }
}

// do these lists look the same (length & contents)?
template <class T>
bool operator== (UnrolledLL<T>& left, UnrolledLL<T>& right) {
  if (left.size() != right.size()) return false;
  typename UnrolledLL<T>::iterator left_itr = left.begin();
  typename UnrolledLL<T>::iterator right_itr = right.begin();
  // walk over both lists, looking for a mismatched value
  while (left_itr != left.end()) {
    if (*left_itr != *right_itr) return false;
    left_itr++; right_itr++;
  }
  return true;
}

template <class T>
bool operator!= (UnrolledLL<T>& left, UnrolledLL<T>& right){ 
  return !(left==right); 
}

// Delete element the iterator points to
template <class T>
typename UnrolledLL<T>::iterator UnrolledLL<T>::erase(iterator itr) {
  // Make sure we are not erasing from a empty list
  assert (size_ > 0);
  // The iterator we returns
  iterator temp_itr=itr;
  iterator result(temp_itr);
  // One node left in the list.
  if (itr.ptr_->num_elements_==1){
    result++; // Increment the returning iterator
    // If there is just one node
    if(head_ == tail_){
      // Delete the node directly and make head and tail point to NULL
      delete itr.ptr_;
      head_ = NULL;
      tail_ = NULL;
    }
    // If the iterator points to head
    else if (itr.ptr_ == head_ ) {
      // Make head points to the second node, make second node's prev NULL
      // Delete the orginal head
      Node<T>* temp = itr.ptr_;
      head_ = head_->next_;
      head_->prev_ = NULL;
      delete temp;
    }
    // If the iterator points to tail
    else if(itr.ptr_ == tail_){
      // Make tail points to the second last node, 
      // make second last node points to NULL, delete the original tail
      Node<T>* temp = itr.ptr_;
      tail_=itr.ptr_->prev_;
      itr.ptr_->prev_->next_=NULL;
      delete temp;
    }
    // Normal cases with just one element in the node
    else{
      // Make the previous and next node points to each other & delete the node
      Node<T>* temp = itr.ptr_;
      itr.ptr_->prev_->next_ = itr.ptr_->next_;
      itr.ptr_->next_->prev_ = itr.ptr_->prev_;
      delete temp;
    }
    --size_; // Number of node decrease by one
    return result;
  }
  // More than one element in the node that iterator points to
  else{
    // Flag suggesting if already reach the element to erase (easier to copy)
    bool found = false;
    // Go through the how array
    for(int i=0; i < itr.ptr_->num_elements_ ; i++){
      // Find the element to erase
      if (i == itr.offset_){
        found = true;
        // If the iterator points to the last element in the array
        // Point the iterator to the next element
        if (itr.offset_==itr.ptr_->num_elements_-1){
          result++;
        }
        // Not copying the element to erase, continue
        continue;
      }
      // After pass the element to erase
      // copy the one element later to the array
      else if (found){
        itr.ptr_->elements_[i-1]=itr.ptr_->elements_[i];
      }
    }  
  }
  // Number of element in the node decrement after erase
  itr.ptr_->num_elements_--;
  return result;
}

// Insert the given element to the position of given iterator
template <class T>
typename UnrolledLL<T>::iterator UnrolledLL<T>::insert(iterator itr,const T& v){
  // Save the original position of the iterator
  iterator result(itr);
  // If the node is already full
  if (itr.ptr_->num_elements_ == NUM_ELEMENTS_PER_NODE){
    // Create a new node and connect it next to the orgininal node
    Node<T>* p = new Node<T>;
    p->prev_=itr.ptr_;
    p->next_=itr.ptr_->next_;
    itr.ptr_->next_=p;
    p->next_->prev_ = p;
    size_++;
    // Move all element after the inserted element to the new node
    for (int i = itr.offset_ , j = 0; i < NUM_ELEMENTS_PER_NODE ; i++,j++){
      p->elements_[j] = itr.ptr_->elements_[i];
      p->num_elements_++;
    }
    // Change the size of the original node and add the insert element
    itr.ptr_->num_elements_ = itr.offset_+1;
    *itr = v;
  }
  // If the node is not already full
  else{
    // Copy all element one earlier after the insert position
    // (backward so the elements we copy is original)
    for (int i = itr.ptr_->num_elements_ ; i > itr.offset_ ; i--){
      itr.ptr_->elements_[i] = itr.ptr_->elements_[i-1];
    }
    // Insert the element and change the number of element of the array
    *itr = v;
    itr.ptr_->num_elements_++;
  }
  return result;
}


template <class T>
void UnrolledLL<T>::copy_list(const UnrolledLL<T>& old) {
  size_ = old.size_;
  // Handle the special case of an empty list.
  if (size_ == 0) {
    head_ = tail_ = 0;
    return;
  }
  // Create a new head node.
  head_ = new Node<T>(old.head_->value_);
  // tail_ will point to the last node created and therefore will move
  // down the new list as it is built
  tail_ = head_;
  // old_p will point to the next node to be copied in the old list
  Node<T>* old_p = old.head_->next_;
  // copy the remainder of the old list, one node at a time
  while (old_p) {
    tail_->next_ = new Node<T>(old_p->value_);
    tail_->next_->prev_ = tail_;
    tail_ = tail_->next_;
    old_p = old_p->next_;
  }
}

// Destructor
template <class T>
void UnrolledLL<T>::destroy_list() {
  // Keep deleting until the list is empty
  while (head_){
    Node<T>* nodeToDelete = head_;
    head_ = head_ -> next_;
    delete nodeToDelete;
    size_--;
  }
}

#endif
