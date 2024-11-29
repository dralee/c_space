/**
 * binary tree height
 * insert element (in level order)
 * 2024.11.29
 */
#include <bits/stdc++.h>
#include <queue>
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

Node* InsertNode(Node* root, int data){
	// if the tree is empty, new it
	if(root == nullptr){
		root = new Node(data);
		return root;
	}

	// do level order traversal until find an empty place
	// i.e. either left child or right child of some node is pointing to NULL
	queue<Node*> q;
	q.push(root);

	while (!q.empty()) {
		// a front element in queue
		Node* current = q.front();
		q.pop();

		// first check left if left is null insert node in left,
		// otherwise check for right
		if(current->left != nullptr){
			q.push(current->left);
		}else{
			current->left = new Node(data);
			return root;
		}
		
		if(current->right != nullptr){
			q.push(current->right);
		}else{
			current->right = new Node(data);
			return root;
		}
	}
	return nullptr;
}

// inorder traversal of a binary tree
void inorder(Node* current){
	if(current == nullptr){
		return;
	}

	inorder(current->left);
	cout<< current->data << ' ';
	inorder(current->right);
}

int main(int argc, char **argv){
	// Representation of the input tree:
	//     1
	//    / \
	//   2   3
	//  /   / \
	// 4   5   6

	Node *root = new Node(1);
	root->left = new Node(2);
	root->right = new Node(3);
	root->left->left = new Node(4);
	root->right->left = new Node(5);
	root->right->right = new Node(6);

	inorder(root);

	return 0;
}
