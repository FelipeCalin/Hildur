#pragma once

#include "Hildur/Core/Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>


namespace Hildur {


	class Transform : public Component
	{
	public:

		void setPosition(glm::vec3 position);
		void setRotation(glm::vec3 rotation);
		void setRotation(glm::quat rotation);
		void setScale(glm::vec3 scale);
		void setParent(Transform* _parent);
		void setParent(Entity* _entity);
		void rotate(glm::quat quat);
		glm::mat4 getRotationMatrix();
		glm::quat getRotation();
		glm::vec3 getPosition();
		glm::vec3 getPositionWorld();
		glm::vec3 getScale();

		glm::mat4 getTransformationMatrix();
		glm::mat4 getLocalTransformationMatrix();
		glm::vec3 forward();
		glm::vec3 forwardWorld();
		glm::vec3 up();
		glm::vec3 upWorld();
		glm::vec3 right();
		glm::vec3 rightWorld();
		bool hasChangedSince(unsigned long int tick);
		bool hasChangedLastTick();
		Transform* getParent();
		std::vector<Transform*> getChildren();
		void renderHandels();

	private:

		unsigned long int _lastChangedTick = -1;
		bool _isMatrixCached = false;
		glm::vec3 _position = glm::vec3(0.0f);
		glm::quat _rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 _scale = glm::vec3(1.0f);
		glm::mat4 _transformationMatrixCached;
		glm::mat4 _transformationMatrixLast;
		Transform* _parent;
		std::vector<Transform*> _children;

		void removeChild(Transform* child);
		void setChanged();
	};


}