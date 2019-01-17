#pragma once

#include <iostream>
#include <pthread.h>
#include <queue>

typedef void(*Handler_t)(int);

class Task
{
  public:

    Task(int sock,Handler_t handler):_sock(sock),_handler(handler)
    {}

    void Run()
    {
      _handler(_sock);
      close(_sock);
    }

  private:
    int _sock;
    Handler_t _handler;
};

class ThreadPool
{
  public:

    ThreadPool(int nums):_nums(nums),_idlenum(0)
    {
      pthread_mutex_init(&_lock,NULL);
      pthread_cond_init(&_cond,NULL);
    }

    static void* Routine(void* arg)
    {
      pthread_detach(pthread_self());
      ThreadPool* p = (ThreadPool*)arg;
      for(;;)
      {
        p->LockQueue();
        while(p->IsEmpty())
        {
          p->IdleThread();
        }
        Task t = p->PopTask();
        p->UnlockQueue();
        t.Run();
      }
    }

    void InitPool()
    {
      pthread_t tid;
      for(int i = 0; i < _nums; i++)
      {
        pthread_create(&tid,NULL,Routine,(void*)this);
      }
    }

    void PushTask(const Task& t)
    {
      LockQueue();
      _taskqueue.push(t);
      WeakUp();
      UnlockQueue();
    }

    Task PopTask()
    {
      Task t = _taskqueue.front();
      _taskqueue.pop();
      return t;
    }

    ~ThreadPool()
    {
      pthread_mutex_destroy(&_lock);
      pthread_cond_destroy(&_cond);
    }
  private:
    void LockQueue()
    {
      pthread_mutex_lock(&_lock);
    }

    void UnlockQueue()
    {
      pthread_mutex_unlock(&_lock);
    }

    void IdleThread()
    {
      _idlenum++;
      pthread_cond_wait(&_cond,&_lock);
      _idlenum--; 
    }

    void WeakUp()
    {
      pthread_cond_signal(&_cond);
    }

    bool IsEmpty()
    {
      return _taskqueue.empty();
    }
    
  private:
    std::queue<Task> _taskqueue;
    int _idlenum;
    int _nums;
    pthread_mutex_t _lock;
    pthread_cond_t _cond;
};
