/*
 * stl(Standard Template Library)
 * deque(stands for double-ended queue)
 * can access element by index
 * 2024.12.03 by dralee
 */
#include <deque>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char **argv) {
    deque<string> cars = {"Volv", "BYD", "BMW", "Ford", "Mazda"};

    for (string car : cars) {
        cout << car << endl;
    }

    cout << "first is " << cars[0] << endl;
    cout << "second is " << cars[1] << endl;
    cout << "first is " << cars.front() << endl;
    cout << "last is " << cars.back() << endl;
    cout << "second is " << cars.at(1) << endl;
    cout << "third is " << cars.at(2) << endl;

    // change element
    cars[0] = "Opel";
    cout << "first is " << cars.front() << endl;

    cars.at(0) = "Opel1";
    cout << "first is " << cars.at(0) << endl;

    // add element
    cars.push_front("Tesla");
    cars.push_back("VW");

    cout << "first is " << cars.front() << endl;
    cout << "last is " << cars.back() << endl;

    // remote first & last
    cars.pop_front();
    cars.pop_back();
    cout << "first is " << cars.front() << endl;
    cout << "last is " << cars.back() << endl;

    cout << "size:" << cars.size() << endl;
    cout << "is empty:" << cars.empty() << endl;

    cout << "the index loop ---" << endl;
    for (int i = 0; i < cars.size(); ++i) {
        cout << cars[i] << endl;
    }
    return 0;
}
