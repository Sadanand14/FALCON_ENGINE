#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <boost/thread/thread.hpp>
#include <atomic>
#include <boost/function.hpp>
#include <vector>
#include <boost/thread/mutex.hpp>
#include <queue>

typedef boost::function<void()> void_function;
typedef std::queue<void_function> ThreadQueue;

class ThreadPool
{
private:
	ThreadQueue workerQueue;
	boost::mutex mtx;
	std::atomic<bool> discard_threadPool;
	std::vector<boost::thread> worker_threads;
	
	static ThreadPool * mainThreadPool;
	ThreadPool();

public:

	static ThreadPool* GetThreadPool();
	~ThreadPool();

	void execute_task();

	template<typename function_type>
	void submit(function_type func);

};

template<typename function_type>
void ThreadPool::submit(function_type func)
{
	workerQueue.push(static_cast<void_function>(func));
}


#endif // !1
