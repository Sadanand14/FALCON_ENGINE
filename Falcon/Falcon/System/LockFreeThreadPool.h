#ifndef LOCK_FREE_THREAD_POOL_H
#define LOCK_FREE_THREAD_POOL_H

//#include <boost/lockfree/queue.hpp>
//
//#include <boost/thread/thread.hpp>
//#include <atomic>
//#include <boost/function.hpp>
//#include <vector>
//
//typedef boost::function<void()> void_function;
//
//struct JobCallable
//{
//	/*void operator()(boost::lockfree::queue<JobCallable> jobs)
//	{
//
//	};*/
//	//void_function task;
//	//JobCallable(void_function& job) { task = job; }
//	~JobCallable() = default;
//};
//
//typedef boost::lockfree::queue<JobCallable> ThreadQueue;
//class ThreadPool
//{
//private:
//
//	static ThreadPool* mainThreadPool;
//
//	ThreadQueue workerQueue;
//	std::atomic<bool> discard_threadPool;
//	std::vector<boost::thread> worker_threads;
//	ThreadPool();
//
//public:
//
//	static ThreadPool* GetThreadPool();
//	~ThreadPool();
//
//	void execute_task();
//
//	template<typename function_type>
//	void submit(function_type func);
//
//};
//
//template<typename function_type>
//void ThreadPool::submit(function_type func)
//{
//	//workerQueue.push(JobCallable(static_cast<void_function>(func)));
//}
//

#endif // !1

