#pragma once

#ifndef ParallelIterator_H
#define ParallelIterator_H
template<typename T>
class ParallelIterator {
private:
    std::function<void(T&)> workFunction;
    size_t numThreads;
    std::vector<std::thread> threads;
    std::atomic<bool> running;
    std::atomic<bool> hasWork;
    T* arrayData;
    size_t arraySize;
    std::atomic<size_t> currentIndex;
    std::mutex mutex;
    std::condition_variable cv;

    void threadFunction(int threadId);

public:
    ParallelIterator();

    ~ParallelIterator();

    void setFunction(std::function<void(T&)> func);

    void process(T* array, size_t size);

    void waitForCompletion();

    size_t getThreadCount() const;
};
#endif