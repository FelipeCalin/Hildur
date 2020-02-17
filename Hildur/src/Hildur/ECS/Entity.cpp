#include "hrpcheaders.h"
#include "Entity.h"

namespace Hildur {
namespace ECS {

    const Entity::Id Entity::INVALID;
    BaseComponent::Family BaseComponent::family_counter_ = 0;

    void Entity::invalidate() {
        id_ = INVALID;
        manager_ = nullptr;
    }

    void Entity::destroy() {
        assert(valid());
        manager_->destroy(id_);
        invalidate();
    }

    std::bitset<ECS::MAX_COMPONENTS> Entity::component_mask() const {
        return manager_->component_mask(id_);
    }

    EntityManager::EntityManager(EventManager& event_manager) : event_manager_(event_manager) {
    }

    EntityManager::~EntityManager() {
        reset();
    }

    void EntityManager::reset() {
        for (Entity entity : entities_for_debugging()) entity.destroy();
        for (BasePool* pool : component_pools_) {
            if (pool) delete pool;
        }
        for (BaseComponentHelper* helper : component_helpers_) {
            if (helper) delete helper;
        }
        component_pools_.clear();
        component_helpers_.clear();
        entity_component_mask_.clear();
        entity_version_.clear();
        free_list_.clear();
        index_counter_ = 0;
    }

    EntityCreatedEvent::~EntityCreatedEvent() {}
    EntityDestroyedEvent::~EntityDestroyedEvent() {}


}  // namespace ECS
}  // namespace Hildur
