/**
 * auto and other advanced features
 * 2025.8.29 by dralee
 * need the c++20, eg: g++ -std=c++20
 */
#include <iostream>
#include <vector>
using namespace std;

void cpp20();
void init();
void typdefName();
void nullAndEnum();

int main(){
    auto x = 10;
    auto y = 20.5f;

    cout<<"x:"<<x<<endl;
    cout<<"y:"<<y<<endl;

    cpp20();

    init();

    typdefName();
    nullAndEnum();

    return 0;
}

/**
 * constinit only can be build in c++20
 */
void cpp20(){
    constexpr int p1 = 2; // 编译期已知
    constinit static int p2 = 3; // 全局静态初始化 ‘constinit’ can only be applied to a variable with static or thread storage duration

    cout<<"p1:"<<p1<<endl;
    cout<<"p2:"<<p2<<endl;
}

void init(){
    std::vector<int> v1 = {1,2,3};
    std::vector<int> v2{1,2,3};
    std::vector<int> v3(3);
    int a = {10};

    cout<<"a="<<a<<endl;    
}

template<typename T>
using Vec = std::vector<T>;
using uint = unsigned int;
void typdefName(){
    uint n = 100;
    Vec<int> v1 = {1,2,3};
    Vec<int> v2{1,2};
    Vec<int> v3(3);

    cout<<"n="<<n<<endl;
    cout<<"v1.size()="<<v1.size()<<endl;
    cout<<"v2.size()="<<v2.size()<<endl;
    cout<<"v3.size()="<<v3.size()<<endl;
}

void nullAndEnum(){
    int *p = nullptr;
    enum class Color { red, green, blue };
    Color c = Color::red;
    cout<<"p="<<p<<endl;
    
}