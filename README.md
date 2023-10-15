# thread_pool

Simple and efficient c++ implementation of thread pool design pattern.
Check example.cpp for detailed example.
Here is the basic example:

1. Create Thread Pool anywhere in your program:

ThreadPool tpool;
tpool.Create(8); // 8 threads

2. Post set of tasks at any time and wait for completion:

for(int i=0; i<100; i++
{
   tpool.Post([](){ /* do something 1 */ });
}
Wait();

3. Post another set of tasks and wait for completion:

for(int i=0; i<100; i++
{
   tpool.Post([](){ /* do something 2 */ });
}
Wait();

If something goes wrong during task execution, call Stop() method to cause Wait() to return immediately. 
