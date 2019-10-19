#include "ThreadPool.h"
ThreadPool* ThreadPool::mainThreadPool = NULL;

/**
*ThreadPool Constructor which helps Initialize worker threads	
*/
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
*ThreadPool Executer.
*/
void ThreadPool::execute_task()
{
	void_function job = NULL;
	while (!discard_threadPool)
	{
		mtx.lock();
		if (!workerQueue.empty()) 
		{
			try 
			{
				job = workerQueue.front();
				workerQueue.pop();
			}
			catch(std::exception &e)
			{
				FL_ENGINE_ERROR("{0}", e.what());
			}
		}
		mtx.unlock();
		if (job) job();
	}
}
