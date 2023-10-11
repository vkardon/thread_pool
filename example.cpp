//
// example.cpp
//
#include <iostream>
#include "ThreadPool.hpp"

static std::mutex sLogMutex;

auto fptr1 = [](int count)
{
    // Do something here...
    usleep((random() % 5) * 1000); // Add a random 0-4 ms delay
    std::unique_lock<std::mutex> lock(sLogMutex);
    std::cout << "[tid=" << ThreadPool::GetThreadId() << "] count=" << count << std::endl;
};

auto fptr2 = [](int count, const std::string& str)
{
    // Do something here...
    usleep((random() % 5) * 1000); // Add a random 0-4 ms delay
    std::unique_lock<std::mutex> lock(sLogMutex);
    std::cout << "[tid=" << ThreadPool::GetThreadId() << "] count=" << count
            << ", str='" << str << "'"<< std::endl;
};

int main()
{
    ThreadPool tpool;
    tpool.Create(4); // 4 threads

    // Test thread pool using fptr1
    for(int i = 0; i < 20; i++)
    {
        tpool.Post(fptr1, i);
    }
    tpool.Wait();
    std::cout << ">>> " << __func__ << ": End Of TestThreadPool part 1" << std::endl;
    sleep(1);

    // Test thread pool using fptr2
    for(int i = 0; i < 20; i++)
    {
        tpool.Post(fptr2, i, "hello" + std::to_string(i));
    }
    tpool.Wait();
    std::cout << ">>> " << __func__ << ": End Of TestThreadPool part 2" << std::endl;
    return 0;
}

