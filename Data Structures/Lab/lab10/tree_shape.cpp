#include <string>
#include <iostream>
class Node{
public:
	int value;
	Node* left;
	Node* right;
};

bool shape_match(Node* a, Node* b){
	if (a==NULL && b==NULL)
		return true;
	else if (a!=NULL && b!=NULL)
		return shape_match(a->left, b->left) && shape_match(a->right, b->right);
	else
		return false;
}

Node* find_subtree_match(Node* tree, Node* pattern){
	if (!tree && !pattern) 
		return tree;
	if (!tree && pattern) 
		return NULL;
	if (shape_match(tree, pattern)) 
		return tree;
	if (find_subtree_match(tree -> left, pattern)) 
		return find_subtree_match(tree -> left, pattern);
	if (find_subtree_match(tree -> right, pattern)) 
		return find_subtree_match(tree -> right, pattern);
	return NULL;
}

int main(){
	Node* a = new Node();
	a->value = 5;
	a->left = new Node();
	a->left->value = 7;
	a->left->left = new Node();
	a->left->left->value = 6;
	a->left->right = new Node();
	a->left->right->value = 11;
	a->right = new Node();
	a->right->value = 13;
	a->right->right = new Node();
	a->right->right->value = 21;

	Node* b = new Node();
	b->value = 17;
	b->left = new Node();
	b->left->value = 9;
	b->left->left = new Node();
	b->left->left->value = 10;
	b->left->right = new Node();
	b->left->right->value = 1;
	b->right = new Node();
	b->right->value = 22;
	b->right->right = new Node();
	b->right->right->value = 8;

	Node* c = new Node();
	c->value = 15;
	c->left = new Node();
	c->left->value = 20;
	c->left->left = new Node();
	c->left->left->value = 4;
	c->left->right = new Node();
	c->left->right->value = 12;
	c->right = new Node();
	c->right->value = 14;
	c->right->left = new Node();
	c->right->left->value = 16;
	c->right->left->left = new Node();
	c->right->left->left->value = 3;
	c->right->left->right = new Node();
	c->right->left->right->value = 18;
	c->right->right = new Node();
	c->right->right->value = 2;
	c->right->right->right = new Node();
	c->right->right->right->value = 19;

	std::cout << "shape_match(a, b): " << shape_match(a, b) << std::endl;
	std::cout << "shape_match(a, c): " << shape_match(a, c) << std::endl;
	std::cout << "find_subtree_match(c, a): " << find_subtree_match(c, a)->value << std::endl;
	return 0;
}