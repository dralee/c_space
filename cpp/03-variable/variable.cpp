/*
变量
2026.3.21 by dralee
*/
#include <iostream>

int main() {
    int a;                 // 定义变量，而不赋值 default-initialization (no initializer)

    int b = 5;              // 定义变量并赋值 5 copy-initialization (initial value after equals sign)
    int c(6);               // 定义变量并赋值，小括号方式 6 direct-initialization (initial value in parenthesis)
    int d{7};               // 定义变量并赋值，大括号方式直接列表初始化 7 direct-list-initialization (initial value in braces)
    int e = {};             // 定义变量并赋值，大括号方式默认值 0 value-initialization (empty braces)

    //int w1{4.5};          // compile error: list-init does not allow narrowing conversion
    int w2(4.5);            // compiles: w3 direct-initialized to value 4
    int w3 = 4.5;           // compiles: w2 copy-initialized to value 4, copy-assignment allows narrowing conversion of 4.5 to 4

    // multiple variables defined
    int m1 = 5, m2 = 6;     // copy-initialization
    int m3(7), m4(8);       // direct-initialization
    int m5{9}, m6{10};      // direct-list-initialization
    int m7 {}, m8 {};       // value-initialization
    [[maybe_unused]] double pi {3.14159}; // Don't complain if pi is unused

    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    std::cout << d << std::endl;
    std::cout << e << std::endl;
    return 0;
}