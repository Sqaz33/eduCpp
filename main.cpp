#include <iostream>
#include <thread>
#include <signal.h>
#include <mutex>
#include <condition_variable>
#include <ranges>
#include <utility>
#include <vector>
#include <algorithm>
#include <functional>



// namespace {

// std::mutex gmut;
// int counter = 0;
// std::condition_variable data_cond;


// void proc() {
//     int c;
//     std::unique_lock<std::mutex> lk{gmut};
//     data_cond.wait(lk, [] { return counter > 0; }); 
//     // при пробуждении и наступлении условия, 
//     // лочить lk
//     c = counter;
// }


// void preporation() {
//     std::lock_guard<std::mutex> lk{gmut}; 
//     ++counter;
//     data_cond.notify_one(); // оповещает другой поток.
// }

// } // namespace

// int x = 0;
// std::mutex xmut;


int main() {
    std::vector<std::pair<int, int>> v = { {0, 1}, {0, 4}, {0, 2}, {0, 3}, {0, 0} };

    // std::sort(v.begin(), v.end(),
    //      [](const auto& a, const auto& b) { return a.second < b.second; });

    std::ranges::sort(v, {}, std::mem_fn(&std::pair<int, int>::second));


    for (const auto& [f, s] : v) {
        std::cout << f << ' ' << s << '\n';
    }

}