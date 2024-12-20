//
// example.cpp
//
#include <iostream>     // std::cout
#include <atomic>       // std::atomic
#include "ThreadPool.hpp"
#include "Pipe.hpp"

// Helper macro to synchronize writing to std::cout
static std::mutex sLogMutex;
#define TRACE(msg) \
{ \
     std::unique_lock<std::mutex> lock(sLogMutex); \
     std::cout << "[tid=" << ThreadPool::GetThreadId() << "] " << msg << std::endl; \
}

// Lambda to test thread pool with lambda expression
auto func1 = [](int count)
{
    // Do something here...
    usleep((random() % 5) * 1000); // Add a random 0-4 ms delay
    TRACE("count=" << count);
};

// Function to test thread pool with function pointer
void func2(int count, const std::string& str)
{
    // Do something here...
    usleep((random() % 5) * 1000); // Add a random 0-4 ms delay
    TRACE("count=" << count << ", str='" << str << "'");
};

// Class to test thread pool with pointer to member function
class MyClass
{
public:
    void func3(int count)
    {
        // Do something here...
        usleep((random() % 5) * 1000); // Add a random 0-4 ms delay
        TRACE("count=" << count << ", name='" << name << "'");
    }
    std::string name{"MyClass"};
};

void TestThreadPool()
{
    ThreadPool tpool;
    tpool.Create(4); // 4 threads

    // Test thread pool with lambda expression (func1)
    std::cout << ">>> Begin Of ThreadPool 'lambda exression' test" << std::endl;
    for(int i = 0; i < 20; i++)
    {
        tpool.Post(func1, i);
    }
    tpool.Wait();
    std::cout << ">>> End Of ThreadPool 'lambda exression' test" << std::endl;

    // Test thread pool with function pointer (func2)
    std::cout << ">>> Begin Of ThreadPool 'function pointer' test" << std::endl;
    for(int i = 0; i < 20; i++)
    {
        tpool.Post(func2, i, "hello" + std::to_string(i));
    }
    tpool.Wait();
    std::cout << ">>> End Of ThreadPool 'function pointer' test" << std::endl;

    // Test thread pool with pointer to member function (MyClass::func3)
    std::cout << ">>> Begin Of ThreadPool 'pointer to member function' test" << std::endl;
    MyClass obj;
    for(int i = 0; i < 20; i++)
    {
        tpool.Post(&MyClass::func3, &obj, i);
    }
    tpool.Wait();
    std::cout << ">>> End Of ThreadPool 'pointer to member function' test" << std::endl;
}

void TestPipe()
{
    std::cout << ">>> Begin Of Pipe test" << std::endl;

    Pipe<int> pipe(15); // Small pipe capacity for test

    std::atomic<int> writeCount{0};
    std::atomic<int> readCount{0};

    // Create threads to write to the pipe
    std::vector<std::thread> writer(5); // 5 threads
    for(auto& thread : writer)
    {
        thread = std::thread([&]()
        {
            for(int i = 0; i < 20; i++)
            {
                usleep((random() % 100) * 1000); // Add a random 0-100 ms delay
                pipe.Push(++writeCount);
            }
        });
    }

    // Create threads to read from the pipe
    std::vector<std::thread> reader(10); // 10 threads
//    std::vector<std::thread> reader(1);
    for(auto& thread : reader)
    {
        thread = std::thread([&]()
        {
            int data{0};
            while(pipe.Pop(data))
            {
                // Do something here...
                //usleep((random() % 5) * 1000); // Add a random 0-4 ms delay
                usleep((random() % 300) * 1000); // Add a random 0-300 ms delay
                TRACE("data=" << data);
                ++readCount;
            }
        });
    }

    // Wait for writer threads to complete
    for(auto& thread : writer)
        thread.join();

    pipe.SetHasMore(false); // No more data to write
    TRACE("Writer is done");

    // Wait for reader threads to complete
    for(auto& thread : reader)
        thread.join();

    TRACE("Reader is done");

    TRACE("Total writeCount=" << writeCount << ", readCount=" << readCount);

    std::cout << ">>> End Of Pipe test" << std::endl;
}

int main()
{
    TestThreadPool();
    TestPipe();
    return 0;
}

