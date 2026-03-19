
void foo(int)
{
}

int main()
{
    unsigned int x { 5 };
    foo(x);

    return 0;
}

/*
If the above has been set correctly, compiling the following program should generate warning C4365:
*/

/*
正确添加编译告警参数后：
$ g++ hello.cpp -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion
hello.cpp: In function ‘int main()’:
hello.cpp:9:9: warning: conversion to ‘int’ from ‘unsigned int’ may change the sign of the result [-Wsign-conversion]
    9 |     foo(x);
      |         ^
*/