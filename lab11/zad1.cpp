#include <iostream>
#include <thread>
#include <mutex>
#include <future>

// void fun(){
//     std::cout << "Wywołano funkcję fun()" << std::endl;
// }

// void fun2(int a){
//     std::cout << "Wywołano funkcję fun2(" << a << ")" << std::endl;
// }

// class MyF{
// public:
//     void operator() (int a);
// };

// void MyF::operator()(int a){
//     std::cout << "Wywołano funktor z argumentem " << a << std::endl;
// }

class Counter{
    int value = 0;
    std::mutex m;
public:
    void incv();
    int get() {return value;}
};

void Counter::incv(){
    // m.lock(); 
    std::scoped_lock lock(m);
    ++value; 
    // m.unlock();
}

int main(){
    // std::thread t1(fun);
    // std::thread t2(fun2,1);

    // MyF f;
    // std::thread t3(f, 2);

    // int a = 5;
    // int b = 3;
    // std::thread t4([a,b](){std::cout << "Lambda: "<< a << "*" << b << "=" << a*b << std::endl;});

    // std::thread t5([](int a, int b){std::cout << "Lambda z arg: "<< a << "*" << b << "=" << a*b << std::endl;},a,b);

    // int w;
    // std::thread t6([&w](int a, int b){std::cout << "Lambda z ref: "<< a << "*" << b << "=" << a*b << std::endl; w=a*b;},a,b);

    // t1.join();
    // t2.join();
    // t3.join();
    // t4.join();
    // t5.join();
    // t6.join();

    // std::cout << "w = " << w << std::endl;

    Counter c;

    std::thread t7([&c](int n){for(int i=0; i<n; ++i) c.incv();},100000);
    std::thread t8([&c](int n){for(int i=0; i<n; ++i) c.incv();},100000);

    t7.join();
    t8.join();

    std::cout << "Licznik: " << c.get() << std::endl;


    std::future<int> result;

    result = std::async(std::launch::async, [](int a, int b){return a*b;},3,4);

    std::cout << "Async: " << result.get() << std::endl;
    
    return 0;
}
