/**
 * Level Order Traversal(Breadth First Search or BFS) of binary tree
 * 2024.11.29 by dralee
 * use queue for level order Traversal
 */
#include <bits/stdc++.h>
#include <queue>
using namespace std;

class Node {
public:
	int data;
	Node *left, *right;

	Node(int val){
		data = val;
		left = right = nullptr;
	}
};

void printLevelOrder(Node *root){
	if(root == nullptr){
		return;
	}

	queue<Node*> q;
	q.push(root);

	while (!q.empty()) {
		// print front of queue and remove it from queue
		Node *node = q.front();
		cout<<node->data<<" ";
		q.pop();

		// enqueue left child
		if(node->left != nullptr){
			q.push(node->left);
		}

		// enqueue right child
		if(node->right != nullptr){
			q.push(node->right);
		}
	}
}

int main(int argc, char **argv){
	/*
	 *		1
	 *	   / \
	 *    2   3
	 *   / \
	 *  4   5
	 */
	Node *root = new Node(1);
	root->left = new Node(2);
	root->right = new Node(3);
	root->left->left = new Node(4);
	root->left->right = new Node(5);

	printLevelOrder(root);

	return 0;
}
