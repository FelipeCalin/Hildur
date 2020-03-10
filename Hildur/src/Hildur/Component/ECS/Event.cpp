#include "hrpcheaders.h"
#include "Event.h"

namespace Hildur {
namespace ECS {

	BaseEvent::Family BaseEvent::family_counter_ = 0;

	BaseEvent::~BaseEvent() {
	}

	EventManager::EventManager() {
	}

	EventManager::~EventManager() {
	}

}
}  // namespace entityx
