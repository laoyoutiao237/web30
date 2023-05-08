#include"src/ThreadPool.h"
#include<iostream>
#include<string>

void print(int a, double b, const char * c, std::string d){
    // std::cout << a << ' ' << b << ' ' << c << ' ' << d << std::endl;
    printf("%d %f %s %s\n", a, b, c, d.c_str());
}

void test(){
    //std::cout << "test" << std::endl;
    printf("test\n");
}

void test1(){
    //std::cout << "other test" << std::endl;
    printf("other try\n");
}

int main(){
    ThreadPool *pool = new ThreadPool();
    std::function<void()> f = std::bind(print, 1, 3.1415, "hello, xy", std::string("world!"));
    
    for(int i = 0 ; i < 20; ++i){
        pool->add(f);
        pool->add(test);
        pool->add(f);
        pool->add(test1);

    }

    
    delete pool;
    return 0;
}