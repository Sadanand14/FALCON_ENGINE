#include "ThreadPool.h"

/**
*ThreadPool Constructor which helps Initialize worker threads	
*/
ThreadPool::ThreadPool() :discard_threadPool(false) 
{
	int const max_threads = std::thread::hardware_concurrency();

	try
	{
		for (int i = 0; i < max_threads; i++) 
		{
			worker_threads.push_back(std::thread(&ThreadPool::execute_task, this));
		}
	}
	catch(...)
	{
		discard_threadPool = true;
		throw;
	}
}

/**
ThreadPool Class Destructor	
*/
ThreadPool::~ThreadPool() 
{
	discard_threadPool = true;
}

/**
*ThreadPoll Executer.
*/
//TODO-> Create a que based execution system based on an event system
void ThreadPool::execute_task()
{
	while (!discard_threadPool)
	{
		if (!workerQueue.empty())
		{
			void_function task = workerQueue.front();
			workerQueue.pop();
			task();
		}
		else
		{
			std::this_thread::yield();
		}
	}
}

/**
* Direct Method for passing in functions to be executed by the thread pool.
*
*@param[in] Any type of function pointer(including lambdas).
*/
template<typename function_type>
void ThreadPool::submit(function_type func) 
{
	workerQueue.push(void_function(func));
}