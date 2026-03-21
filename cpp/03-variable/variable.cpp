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

    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    std::cout << d << std::endl;
    std::cout << e << std::endl;
    return 0;
}