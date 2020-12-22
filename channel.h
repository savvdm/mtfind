#pragma once
#include <queue>
#include <list>
#include <mutex>
#include <condition_variable>

template<class T>
class Channel {
public:
    void Write(T&&);
    void Read(T&);
    void Close();
private:
    std::queue<T, std::list<T>> q;
    std::mutex m;
    std::condition_variable cv;
};
