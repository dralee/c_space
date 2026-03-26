/*
标准输入输出
std::cin 有输入缓冲区，如果输入能抽取到对应变量，则抽取，不能抽取的则会存缓冲区待一下一次输入并赋予下一个变量
std::cout 有输出缓冲区
2026.3.25 by dralee
*/
#include <iostream>

int main(){
    std::cout<<"input two number ";
    int a{};
    std::cin>>a;
    int b{};
    std::cin>>b;
    
    std::cout<<"input the number a is "<<a<<" b is "<<b<<'\n'; // endl: end 打印换行同时清空缓冲区，一般换行直接使用'\n'效率更高

    return 0;
/*
eg 1:
input two number 1 2
input the number a is 1 b is 2

eg 2:
input two number 32
53
input the number a is 32 b is 53

eg 3:
input two number 12\n  
input the number a is 12 b is 0

eg 4:
input two number 23\n 32
input the number a is 23 b is 0

eg 5:
input two number 12\n5
input the number a is 12 b is 0

*/
}