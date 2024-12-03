/*
 * stl(Standard Template Library)
 * list
 * 2024.12.03 by dralee
 */
#include <iostream>
#include <list>
#include <string>
using namespace std;

int main(int argc, char **argv) {
    list<string> cars = {"Volvo", "BYD", "BMW", "Ford", "Mazda"};

    for (string car : cars) {
        cout << car << endl;
    }

    cout << "first is " << cars.front() << endl;
    cout << "last is " << cars.back() << endl;

    // change element
    cars.front() = "Opel";
    cars.back() = "Toyota";

    cout << "after change element----" << endl;
    cout << "first is " << cars.front() << endl;
    cout << "last is " << cars.back() << endl;

    // add element
    cars.push_front("Tesla");
    cars.push_back("VW");

    cout << "after add element----" << endl;
    cout << "first is " << cars.front() << endl;
    cout << "last is " << cars.back() << endl;

    cout << "size is " << cars.size() << endl;
    cout << "is empty: " << cars.empty() << endl;

    return 0;
}
