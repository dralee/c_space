/*
 * recursive reverse level order traversal
 * order traversal using stack and queue
 * implementation of a O(n) time, auxiliary space O(n)
 * 2024.12.02 by dralee
 *
 */
#include <algorithm>
#include <bits/stdc++.h>
#include <queue>
using namespace std;

class Node {
    public:
        int data;
        Node *left, *right;

        Node(int val) {
            data = val;
            left = right = nullptr;
        }

        void reverseLevelOrder(); // root
};

void Node::reverseLevelOrder() {
    if (this == nullptr) {
        return;
    }

    stack<Node *> st;
    queue<Node *> q;
    q.push(this);

    while (!q.empty()) {
		// deque node
		Node *current = q.front();
		q.pop();
		st.push(current);

		// enqueue right child
		if(current->right){
			q.push(current->right);
		}

		// enqueue left child
		if(current->left){
			q.push(current->left);
		}
    }

	// pop all items from stack one by one and print them
	while (!st.empty()) {
		Node *current = st.top();
		cout<<current->data<<" ";
		st.pop();
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
