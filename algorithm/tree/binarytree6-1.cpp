/*
 * recursive level order traversal in spiral form
 * implementation of a O(n) time method for spiral order traversal
 * 2024.12.02 by dralee
 *
 */
#include <bits/stdc++.h>
#include <stack>
using namespace std;

class Node {
    public:
        int data;
        Node *left, *right;

        Node(int val) {
            data = val;
            left = right = nullptr;
        }

        void printSpiral(); // root
};

// void printGivenLevel(Node *root, int level, int ltr);

void Node::printSpiral() {
    if (this == NULL) {
        return;
    }

    // create two stacks to store alternate levels
    stack<Node *> s1; // for levels to be printed from right to left
    stack<Node *> s2; // for levels to be printed from left to right

    // push first level to first stack 's1'
    s1.push(this); // # root

    // keep printing while any of the stacks has some nodes
    while (!s1.empty() || !s2.empty()) {
        // print nodes of current level from s1 and push nodes of next level to
        // s2
        while (!s1.empty()) {
            Node *temp = s1.top();
            s1.pop();
            cout << temp->data << " ";

            // note that is right is pushed before left
            if (temp->right) {
                s2.push(temp->right);
            }
            if (temp->left) {
                s2.push(temp->left);
            }
        }

        // print nodes of current level from s2 and push nodes of next level to
        // s1
        while (!s2.empty()) {
            Node *temp = s2.top();
            s2.pop();
            cout << temp->data << " ";

            // note that is left is pushed before right
            if (temp->left) {
                s1.push(temp->left);
            }
            if (temp->right) {
                s1.push(temp->right);
            }
        }
    }
}

int main(int argc, char **argv) {
    /*
     *		     1
     *	        /  \
     *            2    3
     *           / \  / \
     *          7   6 5  4
     */
    Node *root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(7);
    root->left->right = new Node(6);
    root->right->left = new Node(5);
    root->right->right = new Node(4);

    cout << "Spiral Order traversal of binary tree is \n";
    root->printSpiral();

    return 0;
}
