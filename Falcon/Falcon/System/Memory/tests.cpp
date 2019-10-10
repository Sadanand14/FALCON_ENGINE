int custom_memory_timer_test()
{

	{
		timer T1("With New Operator");
		int* ptr;
		for (int i = 0; i < 10000; i++)
		{
			ptr = new int;
			delete ptr;
		}
	}


	{
		timer T1("With Custom allocator");
		int* ptr;
		for (int i = 0; i < 10000; i++)
		{
			ptr = fmemory::fnew<int>();
			fmemory::fdelete<int>(ptr);
		}

	}
}