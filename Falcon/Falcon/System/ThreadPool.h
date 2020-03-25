#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>
#include <Types.h>
#include <vector>

//#include <EntityManager.h>
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

	boost::mutex mtx;
	ThreadQueue workerQueue;
	GLFWwindow* m_window = nullptr;
	static unsigned int count;
	std::atomic<bool> discard_threadPool;
	static ThreadPool* mainThreadPool;
	std::vector<boost::thread> worker_threads;
	//std::vector<boost::thread,fmemory::STLAllocator<boost::thread>> worker_threads;
	~ThreadPool();
	ThreadPool(GLFWwindow* window);
public:

	static ThreadPool* GetThreadPool();
	static ThreadPool* GetThreadPool(GLFWwindow* window);
	static void ShutDown();


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
