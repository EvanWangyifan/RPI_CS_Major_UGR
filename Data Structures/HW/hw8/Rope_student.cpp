/*
Write your Rope implementation in this file. 
You do not have to keep any of the code that is already in this file.
Functions that are required that you have not implemented have an assert 0
meaning they will always crash. This should help you make sure you implement
everything.
*/

#include <cassert>
#include "Rope.h"

//Should advance to the next Node using in-order traversal
//It can point at any Node, not just leaves
rope_iterator& rope_iterator::operator++(){
	// find the leftmost child of the right node
	if (ptr_->right!=NULL){
		ptr_ = ptr_->right;
		while (ptr_->left!=NULL){
			ptr_ = ptr_->left;
		}
	}
	else{
		// go upwards along right branches...  stop after the first left
		while (ptr_->parent!=NULL && ptr_->parent->right == ptr_){
			ptr_ = ptr_->parent;
		}
		ptr_ = ptr_->parent;
	}
	return *this;
}

//Point to the first Node for in-order traversal
rope_iterator Rope::begin() const{
	if (!root){
		return rope_iterator(NULL);
	}
	Node* p = root;
	// find the leftmost node
	while (p->left){
		p = p->left;
	}
	return rope_iterator(p);
}

// Default constructor
Rope::Rope(){
	root = NULL;
	size_ = 0;
}

//Should make the root = p and adjust any member variables
//This should not make a copy of the rope with root at p,
//it should just "steal" the Node*
Rope::Rope(Node* p){
	root = p;
	int count_size = p->weight;
	Node* temp = p;
	// Find all weight along the righthand side which equals to size
	while (temp->right){
		temp = temp->right;
		count_size += temp->weight;
	}
	size_ = count_size;
}

// Destructor helper function
void Rope::destroy_rope(Node* p){
	if (!p){
		 p = NULL;
		 size_ = 0;
		 return;
	}
	destroy_rope(p->left);
	Node* temp = p->right;
	delete p;
	destroy_rope(temp);
}

// Destructor
Rope::~Rope(){
	destroy_rope(root);
}

// helper function for rope copying
Node* Rope::copy_rope(Node* old_root, Node* parent_node) {
    if (!old_root){
      return NULL;
    }
    Node* new_root = new Node();
    new_root->value=old_root->value;
    new_root->weight=old_root->weight;
    new_root->left=copy_rope(old_root->left, new_root);
    new_root->right=copy_rope(old_root->right, new_root);
    new_root->parent = parent_node;
    return new_root;
}

// Rope copyier
Rope::Rope(const Rope& r){
	size_ = r.size();
	root = copy_rope(r.root, NULL);
}

// Rope assignment
Rope& Rope::operator= (const Rope& r){
	// if they are not the same make a new copy and assign it
	if (&r != this) {
      this->destroy_rope(root);
      root = this->copy_rope(r.root, NULL);
      size_ = r.size_;
    }
    return *this;
}

//MUST BE ITERATIVE
//Get a single character at index i
bool Rope::index(int i, char& c) const{
	int index = i;
	Node* temp = root;
	// If the index is out of range
	if (i >= size_){
		return false;
	}
	// while we don't hit the leaf
	while (!is_leaf(temp)){
		// If the index smaller than weight, go left
		if (index < temp->weight){
			temp = temp->left;
		}
		// If the index larger than weight
		// go right and minus the weight from index
		else{
			index -= temp->weight;
			temp = temp->right;
		}
	}
	// Access the string
	c = temp->value[index];
	return true;
}

//Add the other rope (r)'s string to the end of my string
void Rope::concat(const Rope& r){
	Node* new_root = new Node;
	Node* new_right_root = copy_rope(r.root, NULL);
	// Set original rope as lefthand child
	new_root->left = root;
	// Set parent for the original rope
	root->parent = new_root;
	// update the weight as the size of original rope since it's on the left
	new_root->weight = size_;
	new_root->right = new_right_root;
	// Set parent for the new rope
	new_right_root->parent = new_root;
	// Set new root as the root for the new rope
	root = new_root;
	size_ += r.size();
}

//Get a substring from index i to index j.
//Includes both the characters at index i and at index j.
//String can be one character if i and j match
//Returns true if a string was returned, and false otherwise
//Function should be written with efficient running time.
bool Rope::report(int i, int j, std::string& s) const{
	int num = i;
	// Check if out of range
	if (i<0 || i>=size_ || j<0 || j>=size_ || i>j)
		return false;
	s = "";
	// Find every char in the string from i to j
	for (; num<=j; num++){
		char temp;
		index(num, temp);
		s += temp;
	}
	if (s.size() > 0)
		return true;
	return false;
}

// Helper function to check if we want to split at i we will need to cut node
// Similar to index function
void Rope::cut(int i, Node*& p){
	int index = i;
	Node* temp = root;
	// while we don't hit the leaf
	while (!is_leaf(temp)){
		// If the index smaller than weight, go left
		if (index < temp->weight){
			temp = temp->left;
		}
		// If the index larger than weight
		// go right and minus the weight from index
		else{
			index -= temp->weight;
			temp = temp->right;
		}
	}
	// After hitting the leaf
	// If it is the first char of the string 
	// set the p point to the node
	if (index == 0){
		p = temp;
	}
	// Not first char (need cut)
	else{
		// Set right sub node
		Node* right_new_node = new Node;
		right_new_node->value = temp->value.
								substr(index,temp->value.size()-index);
		right_new_node->weight = temp->value.size()-index;
		
		// Set the left sub node
		Node* left_new_node = new Node;
		left_new_node->value = temp->value.substr(0,index);
		left_new_node->weight = index;

		// Set the original node to two sub node
		temp->left = left_new_node;
		temp->right = right_new_node;
		temp->weight = index;
		left_new_node->parent = temp;
		right_new_node->parent = temp;

		// Set the p to the right sub node
		p = right_new_node;
	}
}

//The first i characters should stay in the current rope, while a new
//Rope (rhs) should contain the remaining size_-i characters.
//A valid split should always result in two ropes of non-zero length.
//If the split would not be valid, this rope should not be changed,
//and the rhs rope should be an empty rope.
//The return value should be this rope (the "left") part of the string
//This function should move the nodes to the rhs instead of making new copies.
Rope& Rope::split(int i, Rope& rhs){
	// 
 	if (i>size_ || i<0){
 		return *this;
 	}
	// If split at the start
	if (i==0){
		// set rhs to the whole rope and return the lhs to NULL
		rhs = *this;
		size_ = 0;
		Node* temp = new Node();
		root = temp;
		return *this;
	}
	// If split at the end
	if (i==size_){
		// set rhs to NULL and return the lhs to the whole rope
		rhs = NULL;
		return *this;
	}
	Node* p;// pointer to the node to start split
	Node* previous; // pointer to the previous node to start split node
	cut(i, p);
	rhs = Rope();
	// Find precious
	previous = p->parent;
	while (!is_leaf(previous)){
		if (previous->right && previous->right!=p){
			previous = previous->right;
		}
		else{
			previous = previous->left;
		}
	}
	// Find the right_sub
	// Go to find the next right node to concat
	Node* next_node = previous;
	
	// Go through the rope to find the next right sub rope
	// Need to manipulate before stop so using while true and break
	while (true){
		// If there is next right and not him self (there is next right node)
		if (next_node->parent!=NULL &&
					next_node->parent->right && 
						next_node->parent->right!=next_node){
			// Create new temp rope for concate later
			Rope temp = Rope();
			Node* temp_root = next_node->parent->right;
			// Count the size of the new rope (while not the leaf)
			Node* ptr = next_node->parent->right;
			int count_size = ptr->weight;
			// Go down the right until hit leaf
			while (ptr->right){
				ptr = ptr->right;
				count_size += ptr->weight;
			}
			temp.root = temp_root;
			temp.size_ = count_size;
			
			// Update weight of corresponding node after disconnect temp
			Node* find_weight_to_change = next_node->parent->right;
			// For all the node has the disconnected node on its left sub tree
			while (find_weight_to_change->parent && find_weight_to_change){
				// Change the weight for these nodes
				if (find_weight_to_change->parent->left==find_weight_to_change){
					find_weight_to_change->parent->weight -= temp.size_;
				}
				find_weight_to_change = find_weight_to_change->parent;
			}
			
			// Disconnect temp from the original rope
			next_node->parent->right->parent = NULL;
			next_node->parent->right = NULL;
			// If adding the first rope to the rhs 
			// (can't use concat because it's the first one)
			if (temp.root==p){
				rhs = temp;
			}
			// Normal Concat
			else{
				rhs.concat(temp);
			}	
		}
		// Stop when hitting the root
		if (next_node==root){
			break;
		}
		next_node = next_node->parent;
	}
	// Update the size of original rope (the one the function returns)
	size_ -= rhs.size();
	// Return the left Rppe
	return *this;
}
