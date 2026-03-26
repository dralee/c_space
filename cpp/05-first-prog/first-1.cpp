/*
简单首测
2026.3.26 by dralee
*/
#include <iostream>

int main()
{
    std::cout << "Enter an integer: ";

    int num1{}, num2{};
    std::cin >> num1;

    std::cout << "Enter another integer: ";
    std::cin >> num2;

    std::cout << num1 << " + " << num2 << " is " << num1 + num2 << ".\n";
    std::cout << num1 << " - " << num2 << " is " << num1 - num2 << ".\n";

    return 0;
}


