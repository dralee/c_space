/**
 * binary tree height
 * use level order traversal approach
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

  queue<Node *> q;
  // push the first level element along with nullptr
  q.push(root);
  q.push(nullptr);

  while (!q.empty()) {
    Node *current = q.front();
    q.pop();

    // when nullptr is encountered, increment the height
    if (current == nullptr) {
      height++;

      // if queue still has elements left, push
      // nullptr again to the queue
      if (!q.empty()) {
        q.push(nullptr);
      }
    } else {
      // if nullptr is not encountered, keep moving
      if (current->left) {
        q.push(current->left);
      }
      if (current->right) {
        q.push(current->right);
      }
    }
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
