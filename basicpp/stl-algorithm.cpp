/*
 * stl(Standard Template Library)
 * algorithm, sort(), find()
 * library has many functions that allow you to modify ranges of data from data
 * structures. 2024.12.03 by dralee 2024.12.06 by dralee others algorithm
 */
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void basic_alg() {
    vector<string> cars = {"Volvo", "BYD", "BMW", "Ford", "Mazda"};

    sort(cars.begin(), cars.end());
    for (string car : cars) {
        cout << car << endl;
    }

    // reverse order
    cout << "reverse order ===" << endl;
    sort(cars.rbegin(), cars.rend());
    for (string car : cars) {
        cout << car << endl;
    }

    cout << "find BYD:";
    auto it = find(cars.begin(), cars.end(), "BYD");
    cout << *it << endl;

    vector<int> nums = {1, 3, 7, 2, 9, 5};
    sort(nums.begin(), nums.end());
    auto it2 = upper_bound(nums.begin(), nums.end(), 5);
    cout << "the first value greater than 5 is " << *it2 << endl;
    auto it3 = lower_bound(nums.begin(), nums.end(), 4);
    cout << "the first value at or above 4 is " << *it3 << endl;

    it2 = min_element(nums.begin(), nums.end());
    cout << "the smallest number is " << *it2 << endl;

    it2 = max_element(nums.begin(), nums.end());
    cout << "the largest number is " << *it2 << endl;

    cout << "copy ..." << endl;
    vector<int> num2(6);
    copy(nums.begin(), nums.end(), num2.begin());
    cout << "the copied size is " << num2.size() << endl;

    vector<int> num3(6);
    cout << "fill the vector with 6:" << endl;
    fill(num3.begin(), num3.end(), 6);
    for (int i = 0; i < num3.size(); ++i) {
        cout << num3[i] << " ";
    }
    cout << endl;
}

bool less_than_10(int value) { return value < 10; }
bool greater_than_5(int value) { return value > 5; }
void add_one(int &value) { value++; }
bool greater_than_10(int value) { return value > 10; }
bool has_o(string item) { return item.find('o') != string::npos; }

void advance_alg() {
    cout << "an iterator pointing to the first element of the first adjacent "
            "pair in a data range."
         << endl;
    vector<string> cars = {"Volvo", "BYD", "BYD", "BMW", "Ford", "Mazda"};
    auto it = adjacent_find(cars.begin(), cars.end());
    if (it != cars.end()) {
        cout << *it << " appears twice in a row" << endl;
    } else {
        cout << "There is no adjacent elements" << endl;
    }

    cout << "all of====" << endl;
    vector<int> num = {1, 5, 3, 7, 9, 2};
    if (all_of(num.begin(), num.end(), less_than_10)) {
        cout << "All of the numbers are less than 10" << endl;
    } else {
        cout << "Some numbers are not less than 10" << endl;
    }

    if (any_of(num.begin(), num.end(), greater_than_5)) {
        cout << "Found a value greater than 5" << endl;
    } else {
        cout << "No value are greater than 5" << endl;
    }

    cout << "binary search 5 ===" << endl;
    sort(num.begin(), num.end());
    if (binary_search(num.begin(), num.end(), 5)) {
        cout << "The number 5 was found!" << endl;
    } else {
        cout << "The number 5 was not Found" << endl;
    }

    cout << "count if ***" << endl;
    int amount = count_if(num.begin(), num.end(), greater_than_5);
    cout << "There are " << amount << " values greater than 5" << endl;

    cout << "find first of 2,3,9:" << endl;
    vector<int> search = {2, 3, 9};
    auto it2 =
        find_first_of(num.begin(), num.end(), search.begin(), search.end());
    if (it2 != num.end()) {
        cout << "the number " << *it2 << " was found" << endl;
    } else {
        cout << "none of the values were found." << endl;
    }

    cout << "find if ***" << endl;
    auto it3 = find_if(num.begin(), num.end(), greater_than_5);
    if (it3 != num.end()) {
        cout << "the number " << *it3 << " is greater than 5" << endl;
    } else {
        cout << "no numbers greater than 5 were found" << endl;
    }

    cout << "find if not ***" << endl;
    auto it4 = find_if_not(num.begin(), num.end(), greater_than_5);
    if (it4 != num.end()) {
        cout << "the number " << *it4 << " is not greater than 5" << endl;
    } else {
        cout << "all numbers are greater than 5" << endl;
    }

    cout << "for each:" << endl;
    for_each(num.begin(), num.end(), add_one);
    for (int n : num) {
        cout << n << " ";
    }
    cout << endl;

    cout << "includes ..." << endl;
    sort(num.begin(), num.end());
    if (includes(num.begin(), num.end(), search.begin(), search.end())) {
        cout << "found" << endl;
    } else {
        cout << "not found" << endl;
    }

    cout << "is permutation:" << endl;
    vector<int> other = {9, 7, 5, 3, 2, 1};
    if (is_permutation(num.begin(), num.end(), other.begin())) {
        cout << "permutation found" << endl;
    } else {
        cout << "no permutation found" << endl;
    }

    cout << "is sorted:";
    if (is_sorted(num.begin(), num.end())) {
        cout << "sorted" << endl;
    } else {
        cout << "not sorted" << endl;
    }
}

void advance_alg2() {
    vector<int> num = {1, 7, 3, 5, 9, 2};
    auto it = is_sorted_until(num.begin(), num.end());
    int pos = it - num.begin();
    cout << "the first " << pos << " items are in ascending order." << endl;

    vector<int> num1 = {1, 4, 9, 16};
    vector<int> num2 = {2, 4, 6, 8};
    vector<int> merged(8);
    merge(num1.begin(), num1.end(), num2.begin(), num2.end(), merged.begin());
    cout << "merge the vector:";
    for (int i : merged) {
        cout << i << " ";
    }
    cout << endl;

    cout << "none of:";
    if (none_of(num.begin(), num.end(), greater_than_10)) {
        cout << "none of the numbers are greater than 10" << endl;
    } else {
        cout << "some numbers are greater than 10" << endl;
    }

    cout << "random shuffle:";
    random_shuffle(num.begin(), num.end());
    for (int n : num) {
        cout << n << " ";
    }
    cout << endl;

    vector<string> cars = {"Volvo", "BYD", "BMW", "Ford", "Mazda"};
    cout << "replace:";
    replace(cars.begin(), cars.end(), (string) "Ford", (string) "Toyota");
    for (string car : cars) {
        cout << car << " ";
    }
    cout << endl;

    cout << "replace copy:";
    vector<string> newcars(6);
    replace_copy(cars.begin(), cars.end(), newcars.begin(), (string) "BMW",
                 (string) "Audi");
    for (string car : cars) {
        cout << car << " ";
    }
    cout << endl;

    cout << "replace copy if:";
    vector<string> newcars2(6);
    replace_copy_if(cars.begin(), cars.end(), newcars2.begin(), has_o,
                    (string) "Audi");
    for (string car : cars) {
        cout << car << " ";
    }
    cout << endl;

    cout << "replace if:";
    replace_if(cars.begin(), cars.end(), has_o, (string) "Audi");
    for (string car : cars) {
        cout << car << " ";
    }
    cout << endl;

    cout << "reverse:";
    reverse(num.begin(), num.end());
    for (int n : num) {
        cout << n << " ";
    }
    cout << endl;

    vector<int> num4(6);
    reverse_copy(num.begin(), num.end(), num4.begin());
    for (int n : num4) {
        cout << n << " ";
    }
    cout << endl;

    cout << "search:";
    vector<int> target = {3, 5, 9};
    /*	auto its = search(num.begin(), num.end(), target.begin(), target.end(),
       target.end() != num.end()); if(its != num.end()){ cout<<"target was
       found"<<endl; }else{ cout<<"target was not found"<<endl;
        }
    */
    int a = 10, b = 25;
    cout << "before: a=" << a << ",b=" << b << endl;
    swap(a, b);
    cout << "after: a=" << a << ",b=" << b << endl;

    cout << "swap range:";
    vector<char> values = {'1', '2', '3', '4', 'w', 'x', 'y', 'z'};
    swap_ranges(values.begin() + 1, values.begin() + 3, values.begin() + 5);
    for (char c : values) {
        cout << c << " ";
    }
    cout << endl;
}

void other_alg1() {
    vector<string> cars = {"Volvo", "BYD", "BMW", "BYD", "Ford", "Mazda"};
    int amount = count(cars.begin(), cars.end(), "BYD");
    cout << "BYD appears " << amount << " times" << endl;

    vector<int> numbers = {1, 7, 3, 5, 9, 2};
    vector<int> target = {3, 5, 9};

    if (search(numbers.begin(), numbers.end(), target.begin(), target.end()) !=
        numbers.end()) {
        cout << "Target was found";
    } else {
        cout << "The target was not found";
    }
}

int main(int argc, char **argv) {
    basic_alg();

    advance_alg();

    advance_alg2();

    other_alg1();

    return 0;
}
