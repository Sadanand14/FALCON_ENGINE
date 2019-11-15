#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <boost/thread/thread.hpp>
#include <atomic>
#include <boost/function.hpp>
#include <vector>
#include <boost/thread/mutex.hpp>
#include <queue>
#include <mutex>
#include "Memory/fmemory.h"

typedef boost::function<void()> void_function;
typedef std::queue<void_function> ThreadQueue;

/**
* Class Definition for a job system.
*/
class ThreadPool
{
private:
	static unsigned int count;
	ThreadQueue workerQueue;
	boost::mutex mtx;
	std::atomic<bool> discard_threadPool;
	static ThreadPool * mainThreadPool;
	ThreadPool();
	std::vector<boost::thread,fmemory::STLAllocator<boost::thread>> worker_threads;

public:

	static ThreadPool* GetThreadPool();
	~ThreadPool();

	void execute_task();

	template<typename function_type>
	void submit(function_type func);

	template<typename function_type, typename ...Args>
	void submit(function_type func, Args ... as);
};

/**
* Direct Method for passing in functions to be executed by the thread pool.
*
*@param[in] Any type of function pointer(including lambdas).
*/
template<typename function_type>
void ThreadPool::submit(function_type func)
{
	mtx.lock();
	workerQueue.push(static_cast<void_function>(func));
	mtx.unlock();
}


template<typename function_type, typename ...Args>
void ThreadPool::submit(function_type func, Args ... as)
{
	auto function = [func, as...]() {func(as...); };

	mtx.lock();
	workerQueue.push(function);
	mtx.unlock();
}

#endif // !1
