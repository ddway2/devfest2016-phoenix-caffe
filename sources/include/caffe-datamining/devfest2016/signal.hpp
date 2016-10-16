#pragma once

#include <memory>
#include <atomic>
#include <thread>
#include <chrono>
#include <csignal>

#include <caffe-datamining/config.h>

namespace devfest2016 {

class CAFFE_DATAMINING_API signal
{
public:
    void operator() ()
    {
        std::signal(SIGINT, signal::signal_catch);
        std::signal(SIGTERM, signal::signal_catch);
        while (!done_) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(500)
            );
        }
    }

private:
    static void signal_catch(int sig);

private:
    static std::atomic<bool>        done_;
};

}   // namespace devfest2016