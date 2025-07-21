#pragma once

template<typename T>
class ParallelIterator {
private:
    std::function<void(T&, GLint)> workFunction;
    size_t numThreads;
    std::vector<std::thread> threads;
    std::atomic<bool> running;
    std::atomic<bool> hasWork;
    T* arrayData;
    size_t arraySize;
    std::atomic<size_t> currentIndex;
    std::mutex mutex;
    std::condition_variable cv;

    void threadFunction(GLint threadId);

public:
    ParallelIterator();

    ~ParallelIterator();

    void setFunction(std::function<void(T&, GLint)> func);

    void process(T* array, size_t size);

    void waitForCompletion();

    size_t getThreadCount() const;
};