/*
 * binary tree use the array for store
 * make children: 2n+1 & 2n+2
 * root: 2n
 * 2024.12.29 by dralee
 */
#include <bits/stdc++.h>
#include <iostream>
using namespace std;

char tree[10];
int root(char key){
	if(tree[0] != '\0'){
		cout<<"Tree already had root";		
	}else{
		tree[0] = key;
	}
	return 0;
}

int set_left(char key, int parent){
	if(tree[parent] == '\0') {
		cout<<"\nCan't set child at "
		<<(parent * 2) + 1
		<<", not parent found";
	}else{
		tree[(parent*2)+1] = key;
	}
	return 0;
}

int set_right(char key, int parent){
	if(tree[parent] == '\0') {
		cout<<"\nCan't set child at "
		<<(parent * 2) + 2
		<<", not parent found";
	}else{
		tree[(parent*2)+2] = key;
	}
	return 0;
}

int print_tree(){
	cout<<"\n";
	for(int i = 0; i < 10; ++i){
		if(tree[i] != '\0'){
			cout<<tree[i];
		}else{
			cout<<"-";
		}
	}
	return 0;
}

int main(){
	root('A');
	set_left('B', 0);
	set_right('C', 0);
	set_left('D', 1);
	set_right('E', 1);
	set_right('F', 2);

	print_tree();

	return 0;

}

