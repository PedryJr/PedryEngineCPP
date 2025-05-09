#pragma once
#include "PedryEngine.h"

using namespace std;

template<typename T>
inline ParallelIterator<T>::ParallelIterator() : running(true), hasWork(false), arrayData(nullptr), arraySize(0) {
    numThreads = max(thread::hardware_concurrency() / 2, 1u);
    threads.reserve(numThreads);
    for (size_t i = 0; i < numThreads; ++i) {
        threads.emplace_back(&ParallelIterator::threadFunction, this, i);
    }
}

template<typename T>
inline ParallelIterator<T>::~ParallelIterator() {
    running = false;
    cv.notify_all();
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

template<typename T>
inline void ParallelIterator<T>::setFunction(function<void(T&)> func) {
    workFunction = func;
}

template<typename T>
inline void ParallelIterator<T>::process(T* array, size_t size) {
    while (hasWork) {
        this_thread::yield();
    }

    {
        lock_guard<std::mutex> lock(mutex);
        arrayData = array;
        arraySize = size;
        currentIndex = 0;
        hasWork = true;
    }

    cv.notify_all();
}

template<typename T>
inline void ParallelIterator<T>::waitForCompletion() {
    while (hasWork) {
        this_thread::yield();
    }
}

template<typename T>
inline size_t ParallelIterator<T>::getThreadCount() const {
    return numThreads;
}

template<typename T>
inline void ParallelIterator<T>::threadFunction(int threadId) {
    while (running) {
        {
            unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [this] { return hasWork || !running; });

            if (!running) break;
            if (!hasWork) continue;
        }
        size_t index;
        while (hasWork && (index = currentIndex.fetch_add(1)) < arraySize) {
            workFunction(arrayData[index]);
        }
        if (currentIndex >= arraySize) {
            hasWork = false;
        }
    }
}

template class ParallelIterator<vec2>;
template class ParallelIterator<vec3>;
template class ParallelIterator<vec4>;
template class ParallelIterator<mat4>;
template class ParallelIterator<quat>;

template class ParallelIterator<GLbyte>;
template class ParallelIterator<GLubyte>;
template class ParallelIterator<GLint>;
template class ParallelIterator<GLuint>;
template class ParallelIterator<GLfloat>;
template class ParallelIterator<GLushort>;
template class ParallelIterator<GLint64>;

template class ParallelIterator<Component>;
template class ParallelIterator<Component*>;
template class ParallelIterator<GameObject>;
template class ParallelIterator<GameObject*>;

//Engine::vecIterator.setFunction([deltaTime](vec4& value)
//    {
//		//ITERATE
//    });
//Engine::vecIterator.process(/*Collection start*/, /*Collection size*/);
//Engine::vecIterator.waitForCompletion();