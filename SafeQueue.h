#ifndef SAFE_QUEUE
#define SAFE_QUEUE
#include <queue>
#include <mutex>
#include <condition_variable>
using namespace std;

template <class T> class SafeQueue {
public:
    SafeQueue() : q() , m() , c() {}
    ~SafeQueue() {} 
    void enqueue(T t) {
		lock_guard<std::mutex> lock(m);
		q.push(t);
		c.notify_one();
    }

    T dequeue(void) {
		unique_lock<std::mutex> lock(m);
		while(q.empty()) { c.wait(lock); }
		T val = q.front(); q.pop();
		return val;
    }
private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
};
#endif
