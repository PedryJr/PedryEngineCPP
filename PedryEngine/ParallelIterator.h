#pragma once

class Parallel {
public:
    using Task = std::function<void()>;

    static void Init(size_t threadCount = std::thread::hardware_concurrency()) {
        Shutdown(); // Reset if already running

        stop = false;
        activeTasks = 0;

        for (size_t i = 0; i < threadCount; ++i) {
            workers.emplace_back([] {
                while (true) {
                    Task task;

                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [] {
                            return stop || !tasks.empty();
                            });

                        if (stop && tasks.empty())
                            return;

                        task = std::move(tasks.front());
                        tasks.pop_front();
                        ++activeTasks;
                    }

                    task();

                    if (--activeTasks == 0 && tasks.empty()) {
                        std::unique_lock<std::mutex> lock(waitMutex);
                        waitCondition.notify_all();
                    }
                }
                });
        }
    }

    static void Shutdown() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }

        condition.notify_all();

        for (std::thread& worker : workers)
            if (worker.joinable())
                worker.join();

        workers.clear();

        // Clear all tasks manually to retain memory
        std::lock_guard<std::mutex> lock(queueMutex);
        tasks.clear();
        activeTasks = 0;
    }

    static void AddTask(const Task& task) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            tasks.emplace_back(task);
        }
        condition.notify_one();
    }

    static void AddTasks(const std::vector<Task>& taskList)
    {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            for (const auto& task : taskList)
                tasks.emplace_back(task);
        }
        condition.notify_all();
    }

    static void Wait() {
        std::unique_lock<std::mutex> lock(waitMutex);
        waitCondition.wait(lock, [] {
            return activeTasks == 0 && tasks.empty();
            });
    }

private:
    static inline std::vector<std::thread> workers;
    static inline std::deque<Task> tasks;
    static inline std::mutex queueMutex;
    static inline std::condition_variable condition;

    static inline std::atomic<bool> stop{ false };
    static inline std::atomic<size_t> activeTasks{ 0 };

    static inline std::mutex waitMutex;
    static inline std::condition_variable waitCondition;
};

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