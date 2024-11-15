#include <memory>
#include <array>
#include <iostream>
#include <utility>
#include <thread>
#include <future>
#include <coroutine>

// return object
struct resumable {

    ///////////////////////////////////////////////////////////////////
    struct promise_type {
        using coro_handle = std::coroutine_handle<promise_type>;

        // из этого делается resume
        auto get_return_object() {
            return coro_handle::from_promise(*this);
        }

        /*
            Эти методы initial_suspend() и final_suspend() используются в C++ корутинах и определяют
            поведение корутины при её запуске и завершении.

            initial_suspend() — определяет, будет ли корутина сразу приостановлена после вызова. Если он
            возвращает std::suspend_always(), корутина начнет выполнение, но сразу приостановится. Чтобы
            корутина продолжила выполнение, нужно вызвать resume().

            final_suspend() — вызывается после завершения тела корутины, перед тем как корутина будет
            действительно завершена. Возвращение std::suspend_always() заставляет корутину приостановиться на
            этом этапе, ожидая, пока ее явно не возобновят или не уничтожат.
        */
        auto initial_suspend() { return std::suspend_always(); }
        auto final_suspend() noexcept { return std::suspend_always(); }
        void return_void() { }
        void unhandled_exception() { std::terminate(); }         
    };
    ///////////////////////////////////////////////////////////////////

    using coro_handle = std::coroutine_handle<promise_type>;
    coro_handle handle_;
    
    resumable(coro_handle handle) : handle_(handle) {}
    bool resume() {
        if (!handle_.done()) handle_.resume();
        return !handle_.done();
    }

    resumable(resumable&& r) : handle_(std::move(r.handle_)) {
        r.handle_ = nullptr;
    }

    decltype(auto) operator=(resumable&& rhs) {
        handle_ = std::move(rhs.handle_);
        return *this;
    }

    resumable(const resumable&) = delete;
    resumable& operator=(const resumable&) = delete;

    ~resumable() {
        handle_.destroy(); // из-за этого запрещен ctor 
    }
};


namespace tst {



resumable cot() {
    int g = 0;
    std::cout << g << '\n';
    co_await std::suspend_always();

    ++g;
    std::cout << g << '\n';
    co_await std::suspend_always();

    ++g;
    std::cout << g << '\n';
}


} // namespace tst


int main() {
    auto c = tst::cot();
    c.resume();
    c.resume();
    c.resume();
}