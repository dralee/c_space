/**
 * use ofstream or fstream for << write &  >> read file
 * 2024.12.02 by dralee
 */

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char **argv){
	// create and open a text file
	ofstream myFile("hello.txt");

	myFile<<"Hello world, this is a text file, just for fun."<<endl;
	myFile<<"this is the second line!"<<endl;

	// close the file
	myFile.close();

	// read from the text file
	string myText;
	ifstream myFile2("hello.txt");

	// use a while with getline() to read file line by line
	while(getline(myFile2, myText)){
		cout<<myText<<endl;
	}

	myFile2.close();

	return 0;
}
