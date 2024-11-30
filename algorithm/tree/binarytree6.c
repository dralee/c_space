/*
 * binary tree
 * recursive level order traversal in spiral
 * 2024.11.29
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Node {
  int data;
  struct Node *left, *right;
};

void printGivenLevel(struct Node *root, int level, int ltr);
int height(struct Node *node);
struct Node *newNode(int data);

void printSpiral(struct Node *root) {
  int h = height(root);
  int i;

  // ltr -> left to right, if this variable is set
  // the the given level is traversed from left to right
  bool ltr = false;
  for (i = 1; i <= h; ++i) {
    printGivenLevel(root, i, ltr);

    // revert ltr to traverse next level in opposite order
    ltr = !ltr;
  }
}

// pint node at a given level
void printGivenLevel(struct Node *root, int level, int ltr) {
  if (root == NULL) {
    return;
  }

  if (level == 1) {
    printf("%d ", root->data);
    return;
  }
  if (level > 1) {
    if (ltr) {
      printGivenLevel(root->left, level - 1, ltr);
      printGivenLevel(root->right, level - 1, ltr);
    } else {
      printGivenLevel(root->right, level - 1, ltr);
      printGivenLevel(root->left, level - 1, ltr);
    }
  }
}

// compute the "height" of a tree, the number of nodes along the longest path
// from the root node down to the farthest leaf node
int height(struct Node *node) {
  if (node == NULL) {
    return 0;
  }

  int leftHeight = height(node->left);
  int rightHeight = height(node->right);

  return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
}

// help that allocates a new node with the given data and NULL left and right
// pointers
struct Node *newNode(int data) {
  struct Node *node = (struct Node *)malloc(sizeof(struct Node));
  node->data = data;
  node->left = NULL;
  node->right = NULL;
  return node;
}

int main(int argc, char **argv) {
  /*
   *				1
   *			   / \
   *            2   3
   *           / \  / \
   *          7   6 5  4
   */
  struct Node *root = newNode(1);
  root->left = newNode(2);
  root->right = newNode(3);
  root->left->left = newNode(7);
  root->left->right = newNode(6);
  root->right->left = newNode(5);
  root->right->right = newNode(4);

  printSpiral(root);

  return 0;
}
