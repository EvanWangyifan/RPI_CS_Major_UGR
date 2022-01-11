//
// NOTE: YOUR FINAL SUBMITTED VERSION SHOULD ONLY CONTAIN 
//       CHANGES TO MoreTests()
//

#include <iostream>
#include <cmath>
#include <cassert>
#include <list>

#include "unrolled.h"

// ===================================================================
// This function compares an unrolled linked list to an STL list.  It
// returns true iff the lists are the same size, and contain the same
// elements in the same order.
template <class T>
bool SAME(UnrolledLL<T>& a, std::list<T> &b) { 
  if (a.size() != b.size()) return false;
  typename UnrolledLL<T>::iterator a_iter = a.begin();
  typename std::list<T>::iterator b_iter = b.begin();
  while (a_iter != a.end() && b_iter != b.end()) {
    if (*a_iter != *b_iter) return false;
    a_iter++;
    b_iter++;
  }
  assert (a_iter == a.end() && b_iter == b.end()); 
  return true;
}


// ===================================================================
void BasicTests();
void MoreTests();

int main() {
  BasicTests();
  MoreTests();
  std::cout << "Finished MoreTests()" << std::endl;
}


// ===================================================================
// NOTE: Your internal data structure may be different (& thus print
// differently), depending on how you implement your internal member
// functions.  That's ok!
void BasicTests() {

  // make two matching list of integers, one using an unrolled list,
  // one using an STL list.  They should stay the "SAME" throughout
  // these tests.
  UnrolledLL<int> a;
  std::list<int> b;
  for (int i = 10; i < 30; ++i) {
    a.push_back(i);
    b.push_back(i);
  }  

  // iterate through the integers and print them out
  std::cout << "the integers from 10->29" << std::endl;
  for (UnrolledLL<int>::iterator itr = a.begin(); itr != a.end(); itr++) {
    std::cout << " " << *itr;
  }
  std::cout << std::endl << std::endl;
  assert (SAME(a,b));

  // use the output operator to print the underlying representation
  std::cout << "initial" << std::endl;
  a.print(std::cout);
  std::cout << std::endl;

  // testing some basic functions in the class
  std::cout << "some editing with pop & push" << std::endl;
  assert (a.size() == 20);
  assert (a.front() == 10);
  assert (a.back() == 29);
  a.pop_front(); 
  b.pop_front();
  assert (a.size() == 19);
  assert (a.front() == 11);
  a.pop_back();
  b.pop_back();
  assert (a.size() == 18);
  assert (a.back() == 28);
  a.print(std::cout);
  std::cout << std::endl;
  assert (SAME(a,b));

  // more editing
  std::cout << "more editing with pop & push" << std::endl;
  a.pop_front();
  a.pop_front();
  a.pop_front();
  a.pop_front();
  a.pop_front();
  b.pop_front();
  b.pop_front();
  b.pop_front();
  b.pop_front();
  b.pop_front();
  a.print(std::cout);
  a.pop_back();
  b.pop_back();
  a.print(std::cout);
  assert (a.size() == 12);
  assert (a.front() == 16);
  assert (a.back() == 27);
  a.push_front(90);
  a.push_front(91);
  a.push_front(92);
  a.push_front(93);
  b.push_front(90);
  b.push_front(91);
  b.push_front(92);
  b.push_front(93);
  a.print(std::cout);
  std::cout << std::endl;
  assert (a.size() == 16);
  assert (a.front() == 93);
  assert (SAME(a,b));

  // erase the multiples of 3
  std::cout <<"erase the multiples of 3" << std::endl;
  UnrolledLL<int>::iterator a_iter = a.begin();
  while (a_iter != a.end()) {
    if (*a_iter % 3 == 0) {
      a_iter = a.erase(a_iter);
    } else {
      a_iter++;
    }
  }
  std::list<int>::iterator b_iter = b.begin();
  while (b_iter != b.end()) {
    if (*b_iter % 3 == 0) {
      b_iter = b.erase(b_iter);
    } else {
      b_iter++;
    }
  }
  a.print(std::cout);
  std::cout << std::endl;
  assert (a.size() == 10);
  assert (SAME(a,b));

  // inserting elements
  std::cout << "do some inserts" << std::endl;
  // insert some stuff
  for (UnrolledLL<int>::iterator itr = a.begin(); itr != a.end(); itr++) {
    if (*itr == 92 || *itr == 16 || *itr == 19 || *itr == 26) {
      itr = a.insert(itr,77);
      itr++;
    } 
  }
  for (std::list<int>::iterator itr = b.begin(); itr != b.end(); itr++) {
    if (*itr == 92 || *itr == 16 || *itr == 19 || *itr == 26) {
      itr = b.insert(itr,77);
      itr++;
    } 
  }
  a.print(std::cout);
  std::cout << std::endl;
  assert (a.size() == 14);
  assert (SAME(a,b));
  
  // overflowing an insert
  std::cout << "insert that overflows the node" << std::endl;
  for (UnrolledLL<int>::iterator itr = a.begin(); itr != a.end(); itr++) {
    if (*itr == 17) {
      itr = a.insert(itr,88);
      itr++;
    } 
  }
  for (std::list<int>::iterator itr = b.begin(); itr != b.end(); itr++) {
    if (*itr == 17) {
      itr = b.insert(itr,88);
      itr++;
    } 
  }
  a.print(std::cout);
  std::cout << std::endl;
  assert (a.size() == 15);
  assert (SAME(a,b));

  // more erasing
  std::cout << "erasing that removes a node" << std::endl;
  a_iter = a.begin();
  while (a_iter != a.end()) {
    if (*a_iter == 77 || *a_iter == 16 || *a_iter == 88) {
      a_iter = a.erase(a_iter);
    } else {
      a_iter++;
    }
  }
  b_iter = b.begin();
  while (b_iter != b.end()) {
    if (*b_iter == 77 || *b_iter == 16 || *b_iter == 88) {
      b_iter = b.erase(b_iter);
    } else {
      b_iter++;
    }
  }
  a.print(std::cout);
  std::cout << std::endl;
  assert (a.size() == 9);
  assert (SAME(a,b));

}


// ===================================================================
void MoreTests() {

  //
  //
  // ADD YOUR OWN TEST CASES HERE
  // ADD COMMENTS TO EXPLAIN THE PURPOSE OF THE TEST CASES
  // TEST CASES SHOULD FINISH AND NOT CRASH. OUTPUT IS HELPFUL AS WELL.
  //
  //

  // make two matching list of integers, one using an unrolled list,
  // one using an STL list.  They should stay the "SAME" throughout
  // these tests.
  UnrolledLL<char> c;
  std::list<char> d;
  for (char i = 'A'; i <= 'Z'; ++i) {
    c.push_back(i);
    d.push_back(i);
  }  

  // iterate through the integers and print them out
  std::cout << "the integers from A->Z" << std::endl;
  for (UnrolledLL<char>::iterator itr = c.begin(); itr != c.end(); itr++) {
    std::cout << " " << *itr;
  }
  std::cout << std::endl << std::endl;
  assert (SAME(c,d));

  // use the output operator to print the underlying representation
  std::cout << "initial" << std::endl;
  c.print(std::cout);
  std::cout << std::endl;

  // testing some basic functions in the class
  std::cout << "some editing with pop & push" << std::endl;
  assert (c.size() == 26);
  assert (c.front() == 'A');
  assert (c.back() == 'Z');
  c.pop_front(); 
  d.pop_front();
  assert (c.size() == 25);
  assert (c.front() == 'B');
  c.pop_back();
  d.pop_back();
  assert (c.size() == 24);
  assert (c.back() == 'Y');
  c.print(std::cout);
  std::cout << std::endl;
  assert (SAME(c,d));

  // more editing
  std::cout << "more editing with pop & push" << std::endl;
  c.pop_front();
  c.pop_front();
  c.pop_front();
  c.pop_front();
  c.pop_front();
  d.pop_front();
  d.pop_front();
  d.pop_front();
  d.pop_front();
  d.pop_front();
  c.print(std::cout);
  c.pop_back();
  d.pop_back();
  c.print(std::cout);
  assert (c.size() == 18);
  assert (c.front() == 'G');
  assert (c.back() == 'X');
  c.push_front('a');
  c.push_front('b');
  c.push_front('c');
  c.push_front('d');
  d.push_front('a');
  d.push_front('b');
  d.push_front('c');
  d.push_front('d');
  c.print(std::cout);
  std::cout << std::endl;
  assert (c.size() == 22);
  assert (c.front() == 'd');
  assert (SAME(c,d));

  // erase the multiples of 3
  std::cout <<"erase char 'O', 'I', and 'U' " << std::endl;
  UnrolledLL<char>::iterator c_iter = c.begin();
  while (c_iter != c.end()) {
    if (*c_iter == 'O' || *c_iter == 'I' || *c_iter == 'U') {
      c_iter = c.erase(c_iter);
    } else {
      c_iter++;
    }
  }
  std::list<char>::iterator d_iter = d.begin();
  while (d_iter != d.end()) {
    if (*d_iter == 'O' || *d_iter == 'I' || *d_iter == 'U') {
      d_iter = d.erase(d_iter);
    } else {
      d_iter++;
    }
  }
  c.print(std::cout);
  std::cout << std::endl;
  assert (c.size() == 19);
  assert (SAME(c,d));
  
  // inserting elements
  std::cout << "do some inserts" << std::endl;
  // insert some stuff
  for (UnrolledLL<char>::iterator itr = c.begin(); itr != c.end(); itr++) {
    if (*itr == 'J' || *itr == 'P' || *itr == 'V' || *itr == 'b') {
      itr = c.insert(itr,'x');
      itr++;
    } 
  }
  for (std::list<char>::iterator itr = d.begin(); itr != d.end(); itr++) {
    if (*itr == 'J' || *itr == 'P' || *itr == 'V' || *itr == 'b') {
      itr = d.insert(itr,'x');
      itr++;
    } 
  }
  c.print(std::cout);
  std::cout << std::endl;
  assert (c.size() == 23);
  assert (SAME(c,d));
  
  // overflowing an insert
  std::cout << "insert that overflows the node" << std::endl;
  for (UnrolledLL<char>::iterator itr = c.begin(); itr != c.end(); itr++) {
    if (*itr == 'G') {
      itr = c.insert(itr,'y');
      itr++;
    } 
  }
  for (std::list<char>::iterator itr = d.begin(); itr != d.end(); itr++) {
    if (*itr == 'G') {
      itr = d.insert(itr,'y');
      itr++;
    } 
  }
  c.print(std::cout);
  std::cout << std::endl;
  assert (c.size() == 24);
  assert (SAME(c,d));

  // more erasing
  std::cout << "erasing that removes a node" << std::endl;
  c_iter = c.begin();
  while (c_iter != c.end()) {
    if (*c_iter == 'y' || *c_iter == 'd' || *c_iter == 'a') {
      c_iter = c.erase(c_iter);
    } else {
      c_iter++;
    }
  }
  d_iter = d.begin();
  while (d_iter != d.end()) {
    if (*d_iter == 'y' || *d_iter == 'd' || *d_iter == 'a') {
      d_iter = d.erase(d_iter);
    } else {
      d_iter++;
    }
  }
  c.print(std::cout);
  std::cout << std::endl;
  assert (c.size() == 21);
  assert (SAME(c,d));



}
