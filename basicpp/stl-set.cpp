/*
 * stl(Standard Template Library)
 * set, sorted automatically in ascending order,
 * unique, meaning equal or duplicate values are ignored
 * existing element cannot be changed, cannot be accessed by index
 * 2024.12.03 by dralee
 */
#include <functional>
#include <iostream>
#include <set>
#include <string>
using namespace std;

int main(int argc, char **argv) {
    set<string> cars = {"Volvo", "BYD", "BMW", "Ford", "Mazda"};

    for (string car : cars) {
        cout << car << endl;
    }

    set<int> nums = {1, 3, 5, 2, 7, 6, 4};
    for (int n : nums) {
        cout << n << " ";
    }
    cout << endl;

    // sort elements in set in descending order
    set<int, greater<int>> nums2 = {1, 3, 5, 2, 7, 6, 4, 2, 3, 4};
    // it is unique
    for (int n : nums2) {
        cout << n << " ";
    }
    cout << endl;

    // add element
    cars.insert("Tesla");
    cars.insert("VM");
    cars.insert("Toyota");
    cars.insert("Audi");

    // remove element
    cars.erase("Volvo");
    cars.erase("Mazda");

    cout << "after remove ---" << endl;
    for (string car : cars) {
        cout << car << endl;
    }

    cout << "size:" << cars.size() << endl;
    cout << "is empty:" << cars.empty() << endl;

    cout << "clear elements" << endl;
    cars.clear();
    cout << "size:" << cars.size() << endl;
    cout << "is empty:" << cars.empty() << endl;
    return 0;
}
