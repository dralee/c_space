/**
 * binary tree height
 * delete a specific element
 * 2024.11.29
 */
#include <bits/stdc++.h>
#include <cstdio>
#include <queue>
using namespace std;

class Node {
public:
  int data;
  Node *left;
  Node *right;

  Node(int val) {
    data = val;
    left = nullptr;
    right = nullptr;
  }
};

// delete the deepest node in binary tree
void deleteDeepest(Node *root, Node *dNode) {
  queue<Node *> q;
  q.push(root);

  Node *current;
  while (!q.empty()) {
    current = q.front();
    q.pop();

    // if current node is the deepest node, delete it
    if (current == dNode) {
      current = nullptr;
      delete (dNode);
      return;
    }

    // check the right child first
    if (current->right) {
      // if right child is the deepest node, delete it
      if (current->right == dNode) {
        current->right = nullptr;
        delete (dNode);
        return;
      }
      q.push(current->right);
    }

    // check the left child
    if (current->left) {
      // if left child is the deepest node, delete it
      if (current->left == dNode) {
        current->left = nullptr;
        delete (dNode);
        return;
      }
      q.push(current->left);
    }
  }
}

// delete the node with the given key
Node *deletion(Node *root, int key) {
  // if the tree is empty, return null
  if (root == nullptr) {
    return nullptr;
  }

  // if the tree has only one node
  if (root->left == nullptr && root->right == nullptr) {
    // if the root node is the key, delete it
    if (root->data == key) {
      delete (root);
      return nullptr;
    }
    return root;
  }

  queue<Node *> q;
  q.push(root);

  Node *current;
  Node *keyNode = nullptr;

  // level order traversal to find the deepest node and the key node
  while (!q.empty()) {
    current = q.front();
    q.pop();

    // if current node is the key node
    if (current->data == key) {
      keyNode = current;
    }

    if (current->left) {
      q.push(current->left);
    }

    if (current->right) {
      q.push(current->right);
    }
  }

  // if key node is found, replace its data with the deepest node
  if (keyNode != nullptr) {
    // store the data of the deepest node
    int x = current->data;

    // replace key node data with deepest node's data
    keyNode->data = x;

    // delete the deepest node
    deleteDeepest(root, current);
  }

  return root;
}

// inorder traversal of a binary tree
void inorder(Node *current) {
  if (current == nullptr) {
    return;
  }

  inorder(current->left);
  cout << current->data << ' ';
  inorder(current->right);
}

int main(int argc, char **argv) {
  // Representation of the input tree:
  //     1
  //    /  \
	//   2    3
  //  /\   / \
	// 4  5  6  7

  Node *root = new Node(1);
  root->left = new Node(2);
  root->right = new Node(3);
  root->left->left = new Node(4);
  root->left->right = new Node(5);
  root->right->left = new Node(6);
  root->right->right = new Node(7);

  printf("the tree data:\n");
  inorder(root);

  cout << "\n\ndelete the 2" << endl;
  int key = 2;
  root = deletion(root, key);

  printf("\nthe tree data:\n");
  inorder(root);

  return 0;
}
