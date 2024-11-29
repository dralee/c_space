/**
 * Level Order Traversal(Breadth First Search or BFS) of binary tree
 * 2024.11.29 by dralee
 *
 */
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

class Node{
public:
	int data;
	Node *left, *right;

	Node(int val){
		data = val;
		left = right = nullptr;
	}
};


void printCurrentLevel(Node *root, int level);
int height(Node *node);

void printLevelOrder(Node *root){
	int h = height(root);
	for(int i = 1; i <= h; ++i){
		printCurrentLevel(root, i);
	}
}

int height(Node *node){
	if(node == nullptr){
		return 0;
	}

	int leftHeight = height(node->left);
	int rightHeight = height(node->right);

	return max(leftHeight,rightHeight)+1;
}

void printCurrentLevel(Node *root, int level){
	if(root == nullptr){
		return;
	}
	if(level == 1){
		cout<<root->data<<" ";
		return;
	}
	if(level > 1){
		printCurrentLevel(root->left, level-1);
		printCurrentLevel(root->right, level-1);
	}
}

int main(int argc, char **argv){
	/*
	 *			1
	 *		   / \
	 *        2   3
	 *       / \
	 *      4   5
	 */
	Node *root = new Node(1);
	root->left = new Node(2);
	root->right = new Node(3);
	root->left->left = new Node(4);
	root->left->right = new Node(5);

	printLevelOrder(root);

	return 0;
}
