#include "channel.h"

template<class T>
void Channel<T>::Write(T&& item) {
    std::unique_lock<std::mutex> lk(m);
    bool was_empty = q.empty();
    q.emplace(item);
    if (was_empty) {
        lk.unlock();
        cv.notify_one();
    }
}

template<class T>
void Channel<T>::Read(T& item) {
    std::unique_lock<std::mutex> lk(m);
    if (q.empty()) {
        cv.wait(lk, [this]{!q.empty();});
    }
    item = q.front();
    q.pop();
}
