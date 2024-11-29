/**
 * binary tree height, by c
 * tree using depth-first search (DFS) approach
 * 2024.11.29
 */
#include <stdio.h>
#include <stdlib.h>

struct Node{
	int data;
	struct Node *left;
	struct Node *right;
};

int height(struct Node *root){
	if(root == NULL){
		return -1;
	}

	int leftHeight = height(root->left);
	int rightHeight = height(root->right);

	return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

struct Node *createNode(int val){
	struct Node *node = (struct Node*)malloc(sizeof(struct Node));
	node->data = val;
	node->left = NULL;
	node->right = NULL;
	return node;	
}

int main(int argc, char **argv){

    // Representation of the input tree:
    //     1
    //    / \
    //   2   3
    //  / \
    // 4   5
	struct Node *root = createNode(1);
	root->left = createNode(2);
	root->right = createNode(3);
	root->left->left = createNode(4);
	root->left->right = createNode(5);

       	printf("height: %d\n", height(root));

	return 0;
}
