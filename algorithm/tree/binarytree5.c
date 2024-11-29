/**
 * Level Order Traversal(Breadth First Search or BFS) of binary tree
 * 2024.11.29 by dralee
 *
 */
#include <stdio.h>
#include <stdlib.h>

struct Node {
	int data;
	struct Node *left, *right;
};

void printCurrentLevel(struct Node *root, int level);
int height(struct Node *node);

void printLevelOrder(struct Node *root){
	int h = height(root);
	int i;
	for(i = 1; i <= h; ++i){
		printCurrentLevel(root, i);
	}	
}

void printCurrentLevel(struct Node *root, int level){
	if(root == NULL){
		return;
	}

	if(level == 1){
		printf("%d ", root->data);
		return;
	}

	if(level > 1){
		printCurrentLevel(root->left, level-1);
		printCurrentLevel(root->right, level-1);
	}
}

// compute the "height" of a tree
int height(struct Node *node){
	if(node == NULL){
		return 0;
	}

	int leftHeight = height(node->left);
	int rightHeight = height(node->right);

	return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
}

struct Node* createNode(int data){
	struct Node *node = (struct Node*)malloc(sizeof(struct Node));
	node->data = data;
	node->left = NULL;
	node->right = NULL;

	return node;	
}

int main(int argc, char **argv){
	/*
	 *      1
	 *     / \
	 *    2   3
	 *   / \
	 *  4   5 
	 */
	struct Node *root = createNode(1);
	root->left = createNode(2);
	root->right = createNode(3);
	root->left->left = createNode(4);
	root->left->right = createNode(5);

	printLevelOrder(root);

	return 0;
}
