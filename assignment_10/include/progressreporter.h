#ifndef ASSIGNMENT_10_INCLUDE_PROGRESSREPORTER_HPP
#define ASSIGNMENT_10_INCLUDE_PROGRESSREPORTER_HPP

#include <atomic>
#include <chrono>
#include <thread>

// modified from
// https://github.com/mmp/pbrt-v3/blob/aaa552a4b9cbf9dccb71450f47b268e0ed6370e2/src/core/progressreporter.h
class ProgressReporter {
   public:
    // ProgressReporter Public Methods
    ProgressReporter(int64_t totalWork, const std::string &title);
    ~ProgressReporter();
    void Update(int64_t num = 1) { workDone += num; }
    float ElapsedMS() const {
        std::chrono::system_clock::time_point now =
            std::chrono::system_clock::now();
        int64_t elapsedMS =
            std::chrono::duration_cast<std::chrono::milliseconds>(now -
                                                                  startTime)
                .count();
        return (float)elapsedMS;
    }
    void Done();

   private:
    // ProgressReporter Private Methods
    void PrintBar();

    // ProgressReporter Private Data
    const int64_t totalWork;
    const std::string title;
    const std::chrono::system_clock::time_point startTime;
    std::atomic<int64_t> workDone;
    std::atomic<bool> exitThread;
    std::thread updateThread;
};

#endif /* ASSIGNMENT_10_INCLUDE_PROGRESSREPORTER_HPP */
