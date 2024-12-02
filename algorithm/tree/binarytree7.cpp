/*
 * recursive reverse level order traversal
 * implementation of a O(n^2) time, auxiliary space O(h)
 * 2024.12.02 by dralee
 *
 */
#include <bits/stdc++.h>
using namespace std;

class Node {
    public:
        int data;
        Node *left, *right;

        Node(int val) {
            data = val;
            left = right = nullptr;
        }

        int height(); // root                                     // root
        void printGivenLevel(int nodeLevel, int reqLevel); // root
        void reverseLevelOrder();                          // root
};

int Node::height() {
    if (this == nullptr) {
        return 0;
    }

    int leftHeight = this->left->height();
    int rightHeight = this->right->height();
    return max(leftHeight, rightHeight) + 1;
}

void Node::printGivenLevel(int nodeLevel, int reqLevel) {
    if (this == nullptr) {
        return;
    }
    // if the required level is reached, print the node
    if (nodeLevel == reqLevel) {
        cout << this->data << " ";
        return;
    }

    // call function for left and right subtree
    if (nodeLevel < reqLevel) {
        this->left->printGivenLevel(nodeLevel + 1, reqLevel);
        this->right->printGivenLevel(nodeLevel + 1, reqLevel);
    }
}

void Node::reverseLevelOrder() {
    if (this == nullptr) {
        return;
    }

    // find the height of the tree
    int h = this->height();

    // start print from the lowest level
    for (int i = h; i >= 1; --i) {
        this->printGivenLevel(1, i);
    }
}

int main(int argc, char **argv) {
    /*
     *		     1
     *	        /  \
     *         2    3
     *        / \
     *       4   5
     */
    Node *root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->left->right = new Node(5);

    cout << "revser level order\n" << endl;
    root->reverseLevelOrder();

    return 0;
}
