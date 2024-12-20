//
// example.cpp
//
#include <iostream>
#include "ThreadPool.hpp"
#include "Pipe.hpp"

static std::mutex sLogMutex;

auto func1 = [](int count)
{
    // Do something here...
    usleep((random() % 5) * 1000); // Add a random 0-4 ms delay
    std::unique_lock<std::mutex> lock(sLogMutex);
    std::cout << "[tid=" << ThreadPool::GetThreadId() << "] count=" << count << std::endl;
};

void func2(int count, const std::string& str)
{
    // Do something here...
    usleep((random() % 5) * 1000); // Add a random 0-4 ms delay
    std::unique_lock<std::mutex> lock(sLogMutex);
    std::cout << "[tid=" << ThreadPool::GetThreadId() << "] count=" << count
            << ", str='" << str << "'"<< std::endl;
};

class MyClass
{
public:
    void func3(int count)
    {
        // Do something here...
        usleep((random() % 5) * 1000); // Add a random 0-4 ms delay
        std::unique_lock<std::mutex> lock(sLogMutex);
        std::cout << "[tid=" << ThreadPool::GetThreadId() << "] count=" << count
                << ", name='" << name << "'"<< std::endl;
    }
    std::string name{"MyClass"};
};

void TestThreadPool()
{
    ThreadPool tpool;
    tpool.Create(4); // 4 threads

    // Test thread pool with lambda exression (func1)
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

    Pipe<int> pipe;

    // Create thread to write to the pipe
    std::thread writer([&pipe]()
    {
        for(int i = 0; i < 20; i++)
        {
            pipe.Push(i);
        }

        pipe.SetHasMore(false); // Writing to the pipe done
    });

    // Create thread to read from the pipe
    std::thread reader([&pipe]()
    {
        int number{0};
        while(pipe.Pop(number))
        {
            // Do something here...
            //usleep((random() % 5) * 1000); // Add a random 0-4 ms delay
            usleep((random() % 200) * 1000); // Add a random 0-200 ms delay
            std::unique_lock<std::mutex> lock(sLogMutex);
            std::cout << "number=" << number << std::endl;
        }
    });

    // Wait for writer & reader threads to complete
    writer.join();
    reader.join();

    std::cout << ">>> End Of Pipe test" << std::endl;
}

int main()
{
    TestThreadPool();
    TestPipe();
    return 0;
}

