#pragma once
#include<functional>
#include<vector>
#include<queue>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<future>

class ThreadPool
{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasks_mutex;
    std::condition_variable cv; //条件变量
    bool stop;
public:
    ThreadPool(int size = std::thread::hardware_concurrency());
    ~ThreadPool();

    template<class F, class... Args>
    auto add(F&& func, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type> ;
};

template<class F, class... Args>
auto ThreadPool::add(F&& f, Args&&... args)-> std::future<typename std::result_of<F(Args...)>::type> {
    typedef typename std::result_of<F(Args...)>::type return_type;

    auto task = std::make_shared<std::packaged_task<return_type()>> (
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
    
    std::future<return_type> res = task->get_future(); //使用期约

    {//在这个{}作用域内对std::mutex加锁， 出了作用域会自动解锁，不需要调用unlock()
        std::unique_lock<std::mutex> lock(tasks_mutex);
        if(stop) 
            throw std::runtime_error("ThreadPool already stop, can't add task anymore!\n");
        tasks.emplace([task](){ (*task)(); });
    }
    cv.notify_one(); //通知一次 唤醒线程
    return res;
}
