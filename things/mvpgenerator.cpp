#include <memory>
#include <array>
#include <iostream>
#include <utility>
#include <thread>
#include <future>
#include <coroutine>

template <class T>
struct generator {

    ///////////////////////////////////////////////////////////////////
    struct promise_type {
        using coro_handle = std::coroutine_handle<promise_type>;

        T current_value;

        auto get_return_object() {
            return coro_handle::from_promise(*this);
        }

        auto yield_value(T value) { // co_yield
            current_value = value;
            return std::suspend_always{};
        }

        auto initial_suspend() { return std::suspend_always(); }
        auto final_suspend() noexcept { return std::suspend_always(); }
        void return_void() { }
        void unhandled_exception() { std::terminate(); }         
    };
    ///////////////////////////////////////////////////////////////////

    using coro_handle = std::coroutine_handle<promise_type>;
    coro_handle handle_;

    generator(coro_handle handle) : handle_(handle) {}

    generator(generator&& r) : handle_(std::move(r.handle_)) {
        r.handle_ = nullptr;
    }

    decltype(auto) operator=(generator&& rhs) {
        handle_ = std::move(rhs.handle_);
        return *this;
    }

    generator(const generator&) = delete;
    generator& operator=(const generator&) = delete;

    ~generator() {
        handle_.destroy(); // из-за этого запрещен ctor 
    }

    bool move_next() {
        return handle_ ? (handle_.resume(), !handle_.done()) : false;
    }

    T current_value() const { return handle_.promise().current_value; }
};


namespace tst {

generator<int> natural_value() {
    // весь фрейм стека корутины между продолжениями лежит на куче.
    int num = 0;
    for(;;) {
        co_yield num++;
    }
}

} // namespace tst


int main() {
    auto c = tst::natural_value();
    
    c.move_next();
    std::cout << c.current_value() << '\n';

    c.move_next();
    std::cout << c.current_value() << '\n';

    c.move_next();
    std::cout << c.current_value() << '\n';
}

