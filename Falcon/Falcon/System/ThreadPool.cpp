#include "ThreadPool.h"
ThreadPool* ThreadPool::mainThreadPool = NULL;
unsigned int ThreadPool::count = 0;
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
	if (--count == 0) 
	{
		if (mainThreadPool != nullptr)
		{
			discard_threadPool = true;
			delete mainThreadPool;
		}
	}
}

ThreadPool* ThreadPool::GetThreadPool() 
{
	if (!mainThreadPool) 
	{
		mainThreadPool = new ThreadPool();
		//mainThreadPool = fmemory::fnew<ThreadPool>();
	}
	count++;
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
		job = NULL;
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
