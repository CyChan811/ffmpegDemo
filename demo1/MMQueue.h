#pragma once

#include <queue>
#include <mutex>

using namespace std;

template<class T>
class MMQueue
{
public:
    MMQueue()
    {

    }

    ~MMQueue()
    {
        mut.lock();
        while(Size() > 0)
        {
            que.pop();
        }
        mut.unlock();
    }

    int Push(T* t)
    {
        mut.lock();

        que.push(t);
        mut.unlock();
        return 0;
    }

    int Pop(T** t)
    {
        mut.lock();

        int size = Size();
        if(size > 0)
        {
            *t = que.front();
            que.pop();
            mut.unlock();
            return 0;
        }
        mut.unlock();
        return -1;
    }

    int Size()
    {
        return que.size();
    }

private:
    queue<T*> que;
    mutex mut;
};