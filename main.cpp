#include <iostream>
#include <thread>
#include <mutex>

// RAII синхронизация

namespace {

void __attribute__((noinline)) use(int c) { asm(""); }  

int x;
std::mutex mforx;

void race() {
    for (int i = 0; i < 1'000'000; ++i) { 
        std::lock_guard<std::mutex> lk(mforx);
        x += 1; use(x); 
    }
    for (int i = 0; i < 1'000'000; ++i) { 
        std::lock_guard<std::mutex> lk(mforx);
        x -= 1; use(x); 
    }
} 

} // namespace

int main() {
    std::thread t1{race};
    std::thread t2{race};

    t1.join();
    t2.join(); 

    std::cout << x << std::endl; 
}