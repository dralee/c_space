/*
 * recursive reverse level order traversal
 * order traversal using hashmap
 * implementation of a O(n) time, auxiliary space O(n)
 * 2024.12.02 by dralee
 *
 */
#include <bits/stdc++.h>
#include <unordered_map>
#include <vector>
using namespace std;

class Node {
    public:
        int data;
        Node *left, *right;

        Node(int val) {
            data = val;
            left = right = nullptr;
        }

        void addNodesToMap(int level, unordered_map<int, vector<int>> &map);
        vector<int> reverseLevelOrder(); // root
};

void Node::addNodesToMap(int level, unordered_map<int, vector<int>> &map) {
    if (this == nullptr) {
        return;
    }

    // add the current node to the vector of nodes at its level in the hashmap
    map[level].push_back(this->data);

    // reversely traversal the left and right subtrees
    this->left->addNodesToMap(level + 1, map);
    this->right->addNodesToMap(level + 1, map);
}

vector<int> Node::reverseLevelOrder() {
    vector<int> result;

    if (this == nullptr) {
        return result;
    }

    // create an unordered_map to store the nodes at each level of the binary
    // tree
    unordered_map<int, vector<int>> map;

    // traversal the binary tree recursively and add nodes to the hashmap
    this->addNodesToMap(0, map);

    // iterate over the hashmap in reverse order of the levels and add nodes to
    // the result vector
    for (int level = map.size() - 1; level >= 0; level--) {
        vector<int> nodesAtLevel = map[level];
        for (int i = 0; i < nodesAtLevel.size(); ++i) {
            result.push_back(nodesAtLevel[i]);
        }
    }

    return result;
}

void print(vector<int> v) {
    for (int i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }
    cout << endl;
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
    vector<int> result = root->reverseLevelOrder();
    print(result);

    return 0;
}
