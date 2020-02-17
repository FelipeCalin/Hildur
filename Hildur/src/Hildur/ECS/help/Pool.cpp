#include "hrpcheaders.h"
#include "Pool.h"

namespace Hildur {
namespace ECS {

    BasePool::~BasePool() {
        for (char* ptr : blocks_) {
            delete[] ptr;
        }
    }

}
}  // namespace entityx
