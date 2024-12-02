/*
 * recursive level order traversal in spiral form
 * 2024.12.02 by dralee
 *
 */
#include <bits/stdc++.h>
using namespace std;

class Node {
public:
	int data;
	Node *left, *right;

	Node(int val){
		data = val;
		left = right = nullptr;
	}

	void printSpiral(); // root
	void printGivenLevel(int level, int ltr); // node
	int height(); // node
};

//void printGivenLevel(Node *root, int level, int ltr);

void Node::printSpiral(){
	int h = this->height();
	int i;

	// ltr: left to right, if this variable is set
	// then the given level is traverse from left to right
	bool ltr = false;
	for(i = 1; i <= h; ++i){
		this->printGivenLevel(i, ltr);

		// revert ltr to traverse next level in opposite order
		ltr = !ltr;
	}
}

void Node::printGivenLevel(int level, int ltr){
	if(this == NULL){
		return;
	}

	if(level == 1){
		cout<<this->data<<" ";
		return;
	}
	if(level > 1){
		if(ltr){
			this->left->printGivenLevel(level-1, ltr);
			this->right->printGivenLevel(level-1, ltr);
		}else{
			this->right->printGivenLevel(level-1, ltr);
			this->left->printGivenLevel(level-1, ltr);
		}
	}
}

int Node::height(){
	if(this == NULL){
		return 0;
	}
	int leftHeight = this->left->height();
	int rightHeight = this->right->height();

	return leftHeight > rightHeight ? leftHeight+1 : rightHeight + 1;
}


int main(int argc, char **argv){
	/*
	 *				1
	 *			   /  \
	 *            2    3
	 *           / \  / \
	 *          7   6 5  4
	 */
	Node *root = new Node(1);
	root->left = new Node(2);
	root->right = new Node(3);
	root->left->left = new Node(7);
	root->left->right = new Node(6);
	root->right->left = new Node(5);
	root->right->right = new Node(4);

	root->printSpiral();
	
	return 0;
}
