#include <iostream>
#include <string>


// Note: It's ok that all the member variables are public for this
// tiny class.  We'll build up to a more robust and complete linked
// list implementation in lecture 11.
template <class T>
class Node {
public:
  T value;
  Node<T> *ptr;
};



template <class T>
void print(Node<T> *data, const std::string &label) {
  std::cout << label;
  Node<T> *tmp = data;
  while (tmp != NULL) {
    std::cout << " " << tmp->value;
    tmp = tmp->ptr;
  }
  std::cout << std::endl;
}



template <class T>
void reverse(Node<T>* &input) {

  // FILL IN THIS FUNCTION
  Node<T> *p = input->ptr;
  Node<T> *p1 = input->ptr->ptr;
  Node<T> *ori = input;
  while(p1 != NULL){
    p->ptr = input;
    input = p;
    p = p1;
    p1 = p1->ptr;
  }
  p->ptr = input;
  input = p;
  ori->ptr = p1;

}





int main() {

  // manually create a linked list of notes with 4 elements
  Node<int>* my_list = new Node<int>; 
  my_list->value = 1; 
  my_list->ptr = new Node<int>;
  my_list->ptr->value = 2;
  my_list->ptr->ptr = new Node<int>;
  my_list->ptr->ptr->value = 3;
  my_list->ptr->ptr->ptr = new Node<int>;
  my_list->ptr->ptr->ptr->value = 4;
  my_list->ptr->ptr->ptr->ptr = NULL;

  
  print(my_list,"my_list before");
  reverse(my_list);
  print(my_list,"my_list after ");

  Node<std::string>* my_list2 = new Node<std::string>; 
  my_list2->value = "aaa"; 
  my_list2->ptr = new Node<std::string>;
  my_list2->ptr->value = "bbb";
  my_list2->ptr->ptr = new Node<std::string>;
  my_list2->ptr->ptr->value = "ccc";
  my_list2->ptr->ptr->ptr = new Node<std::string>;
  my_list2->ptr->ptr->ptr->value = "ddd";
  my_list2->ptr->ptr->ptr->ptr = NULL;

  
  print(my_list2,"my_list before");
  reverse(my_list2);
  print(my_list2,"my_list after ");

  // Note: We are not deleting any of the Nodes we created...  so this
  // program has memory leaks!  More on this in lecture 11.

}

// ===========================================================================
