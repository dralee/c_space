/**
 * binary tree height
 * use level order traversal approach 2
 * 2024.11.29
 */
#include <bits/stdc++.h>
#include <queue>
using namespace std;

class Node {
public:
  int data;
  Node *left, *right;

  Node(int val) {
    data = val;
    left = nullptr;
    right = nullptr;
  }
};

int height(Node *root) {
  if (!root) {
    return 0;
  }

  int height = 0;

  // initializing a queue to traverse the tree level by level
  queue<Node *> q;
  q.push(root);

  while (!q.empty()) {
    int size = q.size();

    // traverse all nodes at the current level
    for (int i = 0; i < size; ++i) {
      Node *current = q.front();
      q.pop();
      if (current->left) {
        q.push(current->left);
      }
      if (current->right) {
        q.push(current->right);
      }
    }

    // increment height after traversing each level
    height++;
  }

  return height - 1;
}

int main(int argc, char **argv) {
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

  cout << "height:" << height(root);

  return 0;
}
