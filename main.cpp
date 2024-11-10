#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>


std::atomic<resource*> res;
std::mutex mut;
resource* getRes() {...}  


void atomic_dcl() {
    if (!res) {
        std::lock_guard<std::mutex> lk{mut};
        if (!res) res = getRes();
    }
}



int main() {
    int x = 1;
    std::move(x);
}