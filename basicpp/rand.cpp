/*
 * rand for random num
 * rand a number between 0 and 100
 * 2024.12.04 by dralee
 */
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    int rn = rand() % 101;
    cout << "rand num: " << rn << endl;

    srand(time(0));
    rn = rand() % 101;
    cout << "rand num: " << rn << endl;

    return 0;
}
