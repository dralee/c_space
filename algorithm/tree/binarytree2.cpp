/**
 * binary tree height
 * use depth-first search (DFS) approach
 * 2024.11.29
 */
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

class Node{
public:
	int data;
	Node *left;
	Node *right;

	Node(int val){
		data = val;
		left = nullptr;
		right = nullptr;
	}
};

int height(Node *root){
	if(root == nullptr){
		return -1;
	}

	int leftHeight = height(root->left);
	int rightHeight = height(root->right);

	return max(leftHeight, rightHeight) + 1;
}


int main(int argc, char **argv){
	// Representation of the input tree:
	//     1
	//    / \
	//   2   3
	//  / \
	// 4   5

	Node *root = new Node(1);
	root->left = new Node(2);
	root->right = new Node(3);
	root->left->left = new Node(4);
	root->left->right = new Node(5);

	cout<<"tree height:"<<height(root);

	return 0;
}
