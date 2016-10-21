#include <devfest2016/signal.hpp>

namespace devfest2016 {

std::atomic<bool>
signal::done_{false};

void
signal::signal_catch(int sig)
{
    done_ = true;
}

}   // namespace devfest2016