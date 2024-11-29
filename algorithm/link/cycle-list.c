#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int data;
	struct Node *prev;
	struct Node *next;
} Node;


void init_list(Node *head){
	head->next = head;
	head->prev = head;
}

void insert_after(Node *head, Node *new_node){
	new_node->prev = head;
	new_node->next = head->next;
	head->next->prev = new_node;
	head->next = new_node;
}

void insert_tail(Node* head, Node *new_node){
	new_node->prev = head->prev;
	new_node->next = head;
	head->prev->next = new_node;
	head->prev = new_node;
}

void delete_node(Node *node){
	if(node->next != node && node->prev != node){
		node->prev->next = node->next;
		node->next->prev = node->prev;
		free(node);
	}
}

void traverse_list(Node *head){
	Node *current = head->next;
	while (current != head) {
		printf("%d ", current->data);
		current = current->next;
	}
	printf("\n");
}

int main(int argc, char **argv){
	Node head;
	init_list(&head);
	
	for(int i = 1; i <= 5; ++i){
		Node *new_node = (Node*)malloc(sizeof(Node));
		new_node->data = i;
		insert_tail(&head, new_node);
	}

	printf("链表内容：");
	traverse_list(&head);
	

	printf("删除第2个数据:\n");
	delete_node(head.next->next );

	printf("链表内容：");
	traverse_list(&head);


	while (head.next != &head) {
		delete_node(head.next);
	}

	printf("链表内容：");
	traverse_list(&head);
	
	return 0;
}	

