#pragma once
#include <queue>
#include <list>
#include <mutex>
#include <condition_variable>
#include <cassert>

// Generic Go-like channel
template<class T>
class Channel {
public:

    void Write(T&& item) {
        std::unique_lock<std::mutex> lk(m);
        assert(!closed);
        q.emplace(item);
        lk.unlock();
        cv.notify_one();
    }

    bool Read(T& item) {
        std::unique_lock<std::mutex> lk(m);
        while (q.empty()) {
            if (closed) {
                return false;
            }
            cv.wait(lk);
        }
        item = std::move(q.front());
        q.pop();
        return true;
    }

    void Close() {
        std::unique_lock<std::mutex> lk(m);
        closed = true;
        lk.unlock();
        cv.notify_all();
    }

private:
    std::queue<T, std::list<T>> q;
    std::mutex m;
    std::condition_variable cv;
    bool closed = false;
};
