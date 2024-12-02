/*
 * try and throw
 * 2024.12.02 by drale
 */
#include <iostream>
using namespace std;

void normal(){
	try{
		int age = 12;
		if(age >= 18){
			cout<<"Access granted - you are old enough.";
		}else{
			throw (age);
		}
	}catch(int num){
		cout<<"Access denied - You must be at least 18 year old."<<endl;
		cout<<"Age is: "<<num<<endl;
	}
}

void anyType(){
	try{
		int age = 12;
		if(age >= 18){
			cout<<"Access granted - you are old enough.";
		}else{
			throw 505;
		}
	}catch(...){
		cout<<"Access denied - You must be at least 18 year old."<<endl;
	}
}

int main(int argc, char **argv){
	cout<<"normal..."<<endl;
	normal();
	cout<<"any type of exceptions..."<<endl;
	anyType();

	return 0;
}
