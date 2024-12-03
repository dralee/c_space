/*
 * stl(Standard Template Library)
 * map, accessiable by keys(not index),and each key is unique
 * automatically sorted in ascending order by their keys
 * 2024.12.03 by dralee
 */
#include <functional>
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main(int argc, char **argv) {
    map<string, int> people = {{"John", 32}, {"Adele", 45}, {"Bo", 29}};

    cout << "John is " << people["John"] << endl;
    cout << "Adele is " << people["Adele"] << endl;

    cout << "Adele is " << people.at("Adele") << endl;
    cout << "Bo is " << people.at("Bo") << endl;

    // change element
    people["John"] = 50;
    cout << "John is " << people["John"] << endl;

    // add element
    people["Jenny"] = 22;
    people["Liam"] = 24;
    // people["Kasper"] = 20;
    //	people["Anja"] = 30;

    people.insert({"Kasper", 20});
    people.insert({"Anja", 30});

    people.insert({"Jim", 18});
    people.insert({"Jim", 28});

    cout << "size:" << people.size() << endl;
    cout << "John is there: " << people.count("John") << endl;

	cout<<"loop element ----"<<endl;
    for (auto person : people) {
        cout << person.first << " is " << person.second << endl;
    }

    // remove element
    people.erase("John");

    cout << "size:" << people.size() << endl;
    cout << "is empty:" << people.empty() << endl;

    people.clear();
    cout << "size:" << people.size() << endl;
    cout << "is empty:" << people.empty() << endl;

    map<string, int, greater<string>> people2 = {
        {"John", 32}, {"Adele", 45}, {"Bo", 29}};
	cout<<"loop element ----"<<endl;
    for (auto person : people2) {
        cout << person.first << " is " << person.second << endl;
    }

    return 0;
}
