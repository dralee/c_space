/*
标准输入输出
2026.3.25 by dralee
*/
#include <iostream>

int main(){
    std::cout<<"input a number ";
    int a;
    std::cin>>a;
    std::cout<<"input the number is "<<a<<std::endl; // endl: end 打印换行同时清空缓冲区，一般换行直接使用'\n'效率更高

    return 0;
}