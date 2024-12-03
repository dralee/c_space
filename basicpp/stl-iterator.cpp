/*
 * stl(Standard Template Library)
 * iterator, for vector, set, list, deque, map not support for stack and queue
 * begin() returns an iterator that points to the first element of the data
 * structure. end() returns an iterator that points to one position after the
 * last element. 2024.12.03 by dralee
 */
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

void vector_it() {
    cout << "vector +++++++++++++++++++++" << endl;
    vector<string> cars = {"Volvo", "BYD", "BMW", "Ford", "Mazda"};

    // create a vector iterator
    vector<string>::iterator it;

    for (it = cars.begin(); it != cars.end(); ++it) {
        cout << *it << endl;
    }

    it = cars.begin();
    cout << "first is " << *it << endl;
    it = cars.end() - 1;
    cout << "last is " << *it << endl;

    cout << "modify first----" << endl;
    it = cars.begin();
    *it = "Tesla";
    cout << "first is " << cars.front() << endl;

    cout << "loop by auto key --- " << endl;
    for (auto it = cars.begin(); it != cars.end(); ++it) {
        cout << *it << endl;
    }

    // loop through vector elements
    cout << "remove BWM" << endl;
    for (auto it = cars.begin(); it != cars.end();) {
        if (*it == "BMW") {
            it = cars.erase(it); // remove BWM element
        } else {
            it++;
        }
    }

    for (const string &car : cars) {
        cout << car << endl;
    }

    cout << "reverse print---" << endl;
    for (auto it = cars.rbegin(); it != cars.rend(); ++it) {
        cout << *it << endl;
    }
    cout << endl;
}

void list_it() {
    list<string> cars = {"Volvo", "BYD", "BMW", "Ford", "Mazda"};
    cout << "list ++++++++++++++++++++++" << endl;
    for (auto it = cars.begin(); it != cars.end(); ++it) {
        cout << *it << endl;
    }

    cout << endl;
}

void deque_it() {
    cout << "deque ****************" << endl;
    deque<string> cars = {"Volvo", "BYD", "BMW", "Ford", "Mazda"};

    for (auto it = cars.begin(); it != cars.end(); ++it) {
        cout << *it << endl;
    }
    cout << endl;
}

void set_it() {
    cout << "set **********************" << endl;
    set<string> cars = {"Volvo", "BYD", "BMW", "Ford", "Mazda"};
    for (auto it = cars.begin(); it != cars.end(); ++it) {
        cout << *it << endl;
    }
    cout << endl;
}

void map_it() {
    cout << "map *****************" << endl;
    map<string, int> people = {{"Jhon", 32}, {"Adele", 45}, {"Bo", 29}};
    for (auto it = people.begin(); it != people.end(); ++it) {
        cout << it->first << " is " << it->second << endl;
    }
    cout << endl;
}

int main(int argc, char **argv) {

    vector_it();

    list_it();

    deque_it();

    set_it();

    map_it();

    return 0;
}
