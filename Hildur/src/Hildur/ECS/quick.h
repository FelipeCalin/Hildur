#pragma once

#include "hrpcheaders.h"

#include "Entity.h"
#include "Event.h"
#include "System.h"
#include "config.h"

namespace Hildur {
namespace ECS {

    /**
     * A convenience class for instantiating an EventManager, EntityManager and
     * SystemManager.
     */
    class EntityComponentSystem {
    public:
        EntityComponentSystem() : entities(events), systems(entities, events) {}

        EventManager events;
        EntityManager entities;
        SystemManager systems;
    };

}
}  // namespace entityx
