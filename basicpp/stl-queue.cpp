/*
 * stl(Standard Template Library)
 * queue, (FIFO) first in first out
 * 2024.12.03 by dralee
 */
#include <iostream>
#include <queue>
#include <string>
using namespace std;

int main(int argc, char **argv) {
    queue<string> cars;

    // add element
    cars.push("Volvo");
    cars.push("BYD");
    cars.push("BMW");
    cars.push("Ford");
    cars.push("Mazda");

    cout << "first is " << cars.front() << endl;
    cout << "last is " << cars.back() << endl;

    // change element
    cars.front() = "Tesla";
    cars.back() = "VM";
    cout << "first is " << cars.front() << endl;
    cout << "last is " << cars.back() << endl;

    cout << "remove the first" << endl;
    cars.pop();
    cout << "first is " << cars.front() << endl;

    cout << "size:" << cars.size() << endl;
    cout << "is empty:" << cars.empty() << endl;

    return 0;
}
