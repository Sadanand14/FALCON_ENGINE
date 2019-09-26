#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <queue>
#include <thread>
#include <atomic>
#include <functional>
#include <vector>

typedef std::function<void()> void_function;
typedef std::queue<void_function> ThreadQueue;

class ThreadPool
{
private:

	static ThreadPool * mainThreadPool;

	ThreadQueue workerQueue;
	std::atomic<bool> discard_threadPool;
	std::vector<std::thread> worker_threads;

	void execute_task();
	ThreadPool();

public:

	static ThreadPool GetThreadPool();
	~ThreadPool();

	template<typename function_type>
	void submit(function_type func);

};


#endif // !1
