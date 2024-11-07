#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <queue>

// like this:
// https://github.dev/tilir/cpp-masters/blob/master/queues/unbounded_queue_demo.cc
namespace {

template <class T> T Limiter();
template <> int Limiter<int>() { return -1; }

template <class T>
class unbounded_queue {
    std::condition_variable condConc_;
    std::mutex qmut_;
    std::queue<T> q_;

    bool empty() const { return q_.empty(); }

public:
    void push(T v) {
        std::unique_lock<std::mutex> lk{qmut_};
        q_.push(std::move(v)); 
        condConc_.notify_one();
    }

    void waitNPop(T& v) {
        std::unique_lock<std::mutex> lk{qmut_};
        condConc_.wait(lk, [this] { return !empty(); });
        T task = std::move(q_.front());
        if (task == Limiter<T>()) { return; }
        v = task;
        q_.pop();
    }

    void wakeNDone() { 
        std::lock_guard<std::mutex> lk{qmut_};
        q_.push(Limiter<T>());
        condConc_.notify_all();
    }
};


} // namespase 

int main() {

    unbounded_queue<int> q;

    auto f = [&] { 
        for (;;) {
            // ...
            // работа с очередью
            if (bool alldone = true; alldone) {
                break;
            }
        }
        q.wakeNDone();
    };

    std::thread t1(f), t2(f);
    t1.join(); t2.join();
}