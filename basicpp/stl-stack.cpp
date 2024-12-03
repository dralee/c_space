/*
 * stl(Standard Template Library)
 * stack (FILO) first in last out
 * 2024.12.03 by dralee
 */
#include <iostream>
#include <stack>
#include <string>
using namespace std;

int main(int argc, char **argv) {
    stack<string> cars;

    // add element
    cars.push("Volvo");
    cars.push("BYD");
    cars.push("BMW");
    cars.push("Ford");
    cars.push("Mazda");

	cout<<"last is "<<cars.top()<<endl;

	cout<<"remove the last element"<<endl;
	cars.pop();

	cout<<"last is "<<cars.top()<<endl;
	cout<<"size is "<<cars.size()<<endl;
	cout<<"is empty: "<<cars.empty()<<endl;

    return 0;
}
