/*
 * Morris Preorder traversal
 * implementation of a O(n) time, auxiliary space O(1)
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

        void preOrder(); // root
};

void Node::preOrder() {
    Node *root = this;
    while (root) {
        // if left child is null, print the current node data, move to right
        // child
        if (root->left == nullptr) {
            cout << root->data << " ";
            root = root->right;
        } else {
            // find inorder predecessor
            Node *current = root->left;
            while (current->right && current->right != root) {
                current = current->right;
            }

            // if the right child of inorder predecessor already points to this
            // node
            if (current->right == root) {
                current->right = nullptr;
                root = root->right;
            } else {
                // if right child doesn't point to this node,then print this
                // node and make right child point to this node
                cout << root->data << " ";
                current->right = root;
                root = root->left;
            }
        }
    }
}

int main(int argc, char **argv) {
    /*
     *		     1
     *	        /  \
     *         2    3
     *        / \  / \
     *       4   5 6  7
     */
    Node *root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->left->right = new Node(5);
    root->right->left = new Node(6);
    root->right->right = new Node(7);

    cout << "preOrder \n" << endl;
    root->preOrder();

    return 0;
}
