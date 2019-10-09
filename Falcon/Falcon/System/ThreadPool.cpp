#include "ThreadPool.h"

ThreadPool* ThreadPool::mainThreadPool = NULL;

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

ThreadPool::~ThreadPool() 
{
	discard_threadPool = true;
}

void ThreadPool::ExecutePool() 
{

}

ThreadPool* ThreadPool::GetThreadPool() 
{
	if (!mainThreadPool) 
	{
		mainThreadPool = new ThreadPool();
	}

	return mainThreadPool;
}

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

