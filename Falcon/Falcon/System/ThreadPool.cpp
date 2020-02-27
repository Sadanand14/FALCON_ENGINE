#include "ThreadPool.h"
ThreadPool* ThreadPool::mainThreadPool = nullptr;
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
	catch (...)
	{
		discard_threadPool = true;
		throw;
	}
}

void ThreadPool::ShutDown() 
{
	//if(mainThreadPool)fmemory::fdelete<ThreadPool>(mainThreadPool);
	if (mainThreadPool!= nullptr)delete mainThreadPool;
}

/**
ThreadPool Class Destructor
*/
ThreadPool::~ThreadPool()
{
	discard_threadPool = true;
	/*for (unsigned int i = 0; i < boost::thread::hardware_concurrency(); i++)
		if(worker_threads[i].joinable())worker_threads[i].join();*/
	//delete mainThreadPool;
}

ThreadPool* ThreadPool::GetThreadPool()
{
	if (mainThreadPool==nullptr)
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
		if (mtx.try_lock())
		{
			if (!workerQueue.empty())
			{
				try
				{
					job = workerQueue.front();
					workerQueue.pop();
				}
				catch (std::exception & e)
				{
					FL_ENGINE_ERROR("{0}", e.what());
				}
			}
			mtx.unlock();
		}
		else 
		{
			boost::this_thread::yield();
		}
		if (job) job();
	}
}
