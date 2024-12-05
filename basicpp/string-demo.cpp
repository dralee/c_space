/*
 * string library has many functions that allow you to perform tasks on strings
 * 2024.12.05 by dralee
 */
#include <iostream>
#include <string>
using namespace std;

void demo_1(){
	// returns an indexed character from a string	
	string str = "Hello World!";
	cout<<"first char is "<<str.at(0)<<endl;
	cout<<"last char is "<<str.at(str.length() - 1)<<endl;

	string chs = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	// in the string, the size() function returns the length of a string.
	cout<<"the length of string is "<<chs.size()<<" and the length is "<<chs.length()<<endl;

	// returns the maximum length of a string
	cout<<"max is is "<<chs.max_size()<<endl;

	// checks wheter a string is empty or not
	cout<<"is empty:"<<chs.empty()<<endl;

	// appends a string (or a part of a string) to another string
 	cout<<"after append is:"<<str.append(" What")<<endl;
	
	// returns a part of a string from a start index (position) and length
	cout<<"from index 2 tak 5 char:"<<str.substr(2,5)<<endl;

	// returns the index (position) of the first occurrence of a string or character
	string str2 = "hello, What is that, and What do you want?";
	cout<<"first Waht is:"<<str2.find("What")<<endl;
	cout<<"from after index 10 the What is:"<<str2.find("What", 10)<<endl;

	// returns the index (position) of the last occurrence of a string or character
	cout<<"revert first Waht is:"<<str2.rfind("What")<<endl;
	cout<<"revert from after index 10 the What is:"<<str2.rfind("What", 10)<<endl;

	// replaces a part of a string with another string
	int thatIndex = str2.find("that");
 	cout<<"after replace:"<<str2.replace(thatIndex, 4, "the thing")<<endl;

	// inserts a string at a specified index (position)
 	cout<<"after insert to that index:"<<str2.insert(thatIndex, "yes")<<endl;

	// removes characters from a string
	cout<<"after erase the that index 3 char:"<<str2.erase(thatIndex, 3)<<endl;

	// compares two strings
	cout<<"compare:"<<str2.compare("hello world")<<endl;
}

int main(int argc, char **argv){
	demo_1();

	return 0;
}
