#include "MMThread.h"

int MMThread::Start()
{
	if(t == nullptr)
	{
		stopFlag = 0;
		t = new thread(&MMThread::run, this);
	}
	// t.detach();

	return 0;
}

int MMThread::Stop()
{
	if(t != nullptr)
	{
		stopFlag = 1;
		t->join();
		delete t;
		t = nullptr;
	}
	return 0;
}