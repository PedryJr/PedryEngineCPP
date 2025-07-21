class ThreadPool {
public:
    static ThreadPool* s_threadPool;
    explicit ThreadPool(size_t threadCount, size_t initialCapacity = 1024)
        : done(false), head(0), tail(0), capacity(initialCapacity)
    {
        tasks.resize(capacity);
        start(threadCount);
    }

    ~ThreadPool() {
        stop();
    }

    template<typename Func, typename... Args>
    auto enqueue(Func&& func, Args&&... args)
        -> std::future<typename std::invoke_result_t<Func, Args...>>
    {
        using ReturnType = typename std::invoke_result_t<Func, Args...>;

        auto taskPtr = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
        );

        std::future<ReturnType> res = taskPtr->get_future();

        {
            std::unique_lock<std::mutex> lock(queueMutex);

            if (done)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            // Resize buffer if full (one slot left empty to differentiate full/empty)
            if ((tail + 1) % capacity == head) {
                // Grow capacity, allocate new vector
                size_t newCapacity = capacity * 2;
                std::vector<std::function<void()>> newTasks(newCapacity);

                // Copy elements from old buffer to new buffer in correct order
                size_t i = 0;
                for (size_t idx = head; idx != tail; idx = (idx + 1) % capacity) {
                    newTasks[i++] = std::move(tasks[idx]);
                }

                tasks = std::move(newTasks);
                capacity = newCapacity;
                head = 0;
                tail = i;
            }

            // Place new task in buffer
            tasks[tail] = [taskPtr]() { (*taskPtr)(); };
            tail = (tail + 1) % capacity;
        }
        condition.notify_one();
        return res;
    }

private:
    std::vector<std::function<void()>> tasks;
    size_t head;
    size_t tail;
    size_t capacity;

    std::vector<std::thread> workers;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> done;

    void start(size_t threadCount) {
        for (size_t i = 0; i < threadCount; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex);
                        this->condition.wait(lock, [this] { return done || head != tail; });

                        if (done && head == tail)
                            return;

                        task = std::move(tasks[head]);
                        tasks[head] = nullptr;  // Optional: release held function object
                        head = (head + 1) % capacity;
                    }

                    task();
                }
                });
        }
    }

    void stop() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            done = true;
        }
        condition.notify_all();

        for (auto& worker : workers)
            if (worker.joinable())
                worker.join();
    }
};
