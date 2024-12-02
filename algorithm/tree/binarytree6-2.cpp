/*
 * recursive level order traversal in spiral form
 * implementation of a O(n) time method for spiral order traversal
 * use deque
 * 2024.12.02 by dralee
 *
 */
#include <bits/stdc++.h>
#include <deque>
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

    // declare a deque
    deque<Node *> dq;
    bool reverse = true;
    // push first level to first stack 's1'
    dq.push_back(this); // # root

    // keep printing while any of the stacks has some nodes
    while (!dq.empty()) {
        // save the size of the deque here itself, as in further
        // steps the size of deque will frequently
        int n = dq.size();
        // if printing left to right
        if (!reverse) {
            // iterate from left to right
            while (n > 0) {
                n--;
                // insert the child from the back of the deque left child first
                if (dq.front()->left != nullptr) {
                    dq.push_back(dq.front()->left);
                }
                if (dq.front()->right != nullptr) {
                    dq.push_back(dq.front()->right);
                }

                // print the current processed element
                cout << dq.front()->data << " ";
                dq.pop_front();
            }
            // switch reverse for next traversal
            reverse = !reverse;
        } else {
            // if printing right to left,iterate the deque in reverse order and
            // insert the children from the front
            while (n > 0) {
                // insert the child in the front of the deque right child first
                n--;
                if (dq.back()->right != nullptr) {
                    dq.push_front(dq.back()->right);
                }
                if (dq.back()->left != nullptr) {
                    dq.push_front(dq.back()->left);
                }

                // print the current processed element
                cout << dq.back()->data << " ";
                dq.pop_back();
            }
            // switch rerse for next traversal
            reverse = !reverse;
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
