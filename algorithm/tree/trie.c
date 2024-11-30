#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

struct TrieNode {
  struct TrieNode *children[ALPHABET_SIZE];
  bool isendofword;
};

typedef struct TrieNode TrieNode;

TrieNode *createNode() {
  struct TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
  node->isendofword = false;
  for (int i = 0; i < ALPHABET_SIZE; ++i) {
    node->children[i] = NULL;
  }
  return node;
}

void insert(TrieNode *root, const char *key) {
  TrieNode *current = root;
  for (int i = 0; i < strlen(key); ++i) {
    int index = key[i] - 'a';
    if (current->children[index] == NULL) {
      current->children[index] = createNode();
    }
    current = current->children[index];
  }
  current->isendofword = true;
}

bool search(TrieNode *root, const char *key) {
  TrieNode *current = root;
  for (int i = 0; i < strlen(key); ++i) {
    int index = key[i] - 'a';
    if (current->children[index] == NULL) {
      return false;
    }
    current = current->children[index];
  }
  return (current != NULL && current->isendofword);
}

bool isempty(TrieNode *root) {
  for (int i = 0; i < ALPHABET_SIZE; ++i) {
    if (root->children[i] != NULL) {
      return false;
    }
  }
  return true;
}

TrieNode *delete(TrieNode *root, const char *key, int depth) {
  if (root == NULL) {
    return NULL;
  }
  if (depth == strlen(key)) {
    if (root->isendofword) {
      root->isendofword = false;
    }
    if (isempty(root)) {
      free(root);
      root = NULL;
    }
    return root;
  }
  int index = key[depth] - 'a';
  root->children[index] = delete (root->children[index], key, depth + 1);
  if (isempty(root) && !root->isendofword) {
    free(root);
    root = NULL;
  }
  return root;
}

void deleteKey(TrieNode *root, const char *key) { delete (root, key, 0); }

int main(int argc, char **argv) {

  TrieNode *root = createNode();

  insert(root, "hello");
  insert(root, "world");

  printf("%s\n", search(root, "hello") ? "Found" : "Not Found");
  printf("%s\n", search(root, "world") ? "Found" : "Not Found");
  printf("%s\n", search(root, "dralee") ? "Found" : "Not Found");

  deleteKey(root, "hello");
  printf("%s\n", search(root, "hello") ? "Found" : "Not Found");

  return 0;
}
