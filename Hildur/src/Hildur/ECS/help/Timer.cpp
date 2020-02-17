#include "hrpcheaders.h"
#include "Timer.h"

namespace Hildur {
namespace ECS {
namespace help {

    Timer::Timer() {
        _start = std::chrono::system_clock::now();
    }

    Timer::~Timer() {
    }

    void Timer::restart() {
        _start = std::chrono::system_clock::now();
    }

    double Timer::elapsed() {
        return std::chrono::duration<double>(std::chrono::system_clock::now() - _start).count();
    }

}  // namespace help
}  // namespace ECS
}  // namespace Hildur