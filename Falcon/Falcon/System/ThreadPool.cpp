#include "ThreadPool.h"
ThreadPool* ThreadPool::mainThreadPool = NULL;

/**
*ThreadPool Constructor which helps Initialize worker threads	
*/
ThreadPool::ThreadPool() :discard_threadPool(false) 
{
	int const max_threads = 5;// boost::thread::hardware_concurrency();

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

/**
ThreadPool Class Destructor	
*/
ThreadPool::~ThreadPool() 
{
	discard_threadPool = true;
}

ThreadPool* ThreadPool::GetThreadPool() 
{
	if (!mainThreadPool) 
	{
		mainThreadPool = new ThreadPool();
		//mainThreadPool = fmemory::fnew<ThreadPool>();
	}

	return mainThreadPool;
}

/**
*ThreadPoll Executer.
*/
void ThreadPool::execute_task()
{
	while (!discard_threadPool)
	{
		void_function job = NULL;
		mtx.lock();
		if (!workerQueue.empty()) 
		{
			job = workerQueue.front();
			workerQueue.pop();
		}
		mtx.unlock();
		if (job) job();
	}
}
