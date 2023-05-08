#include"ThreadPool.h"

ThreadPool::ThreadPool(int size):stop(false){
    for(int i = 0; i < size; ++i){
        threads.emplace_back(std::thread([this](){
            while(true){
                std::function<void()> task;
                {//在这个{}作用域内对std::mutex加锁， 出了作用域会自动解锁，不需要调用unlock()
                    std::unique_lock<std::mutex> lock(tasks_mutex);
                    cv.wait(lock, [this](){ //等待条件变量，条件为任务队列不为空或线程池停止
                        return stop || !tasks.empty();
                    });
                    if(stop && tasks.empty()) return; //任务队列为空并且线程池停止，退出线程
                    task = tasks.front();
                    tasks.pop();
                }
                task(); //执行任务 允许任务被打断
            }
        }));
    }
}

ThreadPool::~ThreadPool(){
    {
        std::unique_lock<std::mutex> lock(tasks_mutex);
        stop = true;
    }

    cv.notify_all();
    for(std::thread &th : threads){
        if(th.joinable())
            th.join();
    }

}

