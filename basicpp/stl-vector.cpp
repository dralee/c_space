/*
 * stl(Standard Template Library)
 * vector,dynamic array
 * 2024.12.03 by dralee
 */
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void vector_test() {
    vector<string> cars = {"Volvo", "BYD", "BMW", "Ford", "Mazda"};

    for (string car : cars) {
        cout << car << endl;
    }

    // get the first element
    cout << cars[0] << " & " << cars[1] << endl;

    // front, back
    cout << "first is " << cars.front() << endl;
    cout << "last is " << cars.back() << endl;

    // at
    cout << "second is " << cars.at(1) << endl;
    cout << "third is " << cars.at(2) << endl;

    // change element
    cars[0] = "Opel";
    cout << "first is " << cars.front() << endl;

    // use at to change
    cars.at(0) = "Opel2";
    cout << "first is " << cars.at(0) << endl;

    // add element
    cars.push_back("Tesla");
    cars.push_back("VW");
    cars.push_back("Mitsublishi");
    cars.push_back("Mini");

    cout << "size:" << cars.size() << " last is " << cars.back() << endl;

    // remove element
    cars.pop_back();
    cout << "the last is " << cars.back() << " and now the size is "
         << cars.size() << endl;

    // is empty
    cout << "is empty:" << cars.empty() << endl;

    // by index loop
    cout << "list by index --" << endl;
    for (int i = 0; i < cars.size(); ++i) {
        cout << cars[i] << endl;
    }
}

int main(int argc, char **argv) {
    vector_test();

    return 0;
}
