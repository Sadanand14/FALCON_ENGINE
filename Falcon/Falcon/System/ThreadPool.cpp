#include "ThreadPool.h"

ThreadPool* ThreadPool::mainThreadPool = NULL;

ThreadPool::ThreadPool() :discard_threadPool(false) 
{
	int const max_threads = boost::thread::hardware_concurrency();

	try
	{
		for (int i = 0; i < max_threads; i++) 
		{
			worker_threads.push_back(boost::thread(&ThreadPool::execute_task, this));
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
		void_function job;
		if (workerQueue.pop(job)) job();
	}
}

