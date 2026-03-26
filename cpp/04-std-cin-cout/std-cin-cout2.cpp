/*
标准输入输出
2026.3.25 by dralee
*/
#include <iostream>

int main(){
    std::cout<<"input two number ";
    int a,b;
    std::cin>>a>>b;
    
    std::cout<<"input the number a is "<<a<<" b is "<<b<<'\n'; // endl: end 打印换行同时清空缓冲区，一般换行直接使用'\n'效率更高

    return 0;
/*
eg 1:
input two number 12
32
input the number a is 12 b is 32

eg 2:
input two number 12 33
input the number a is 12 b is 33

eg 3:
input two number 12\n
input the number a is 12 b is 0

eg 4:
input two number 12\n 33
input the number a is 12 b is 0


*/
}