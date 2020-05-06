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

		void SetPosition(glm::vec3 position);
		void SetRotation(glm::vec3 rotation);
		void SetRotation(glm::quat rotation);
		void SetScale(glm::vec3 scale);
		void SetParent(Transform* _parent);
		void SetParent(Entity* _entity);
		void Rotate(glm::quat quat);
		glm::mat4 GetRotationMatrix();
		glm::quat GetRotation();
		glm::vec3 GetRotationEuler();
		glm::vec3 GetPosition();
		glm::vec3 GetPositionWorld();
		glm::vec3 GetScale();

		glm::mat4 GetTransformationMatrix();
		glm::mat4 GetLocalTransformationMatrix();
		glm::vec3 Forward();
		glm::vec3 ForwardWorld();
		glm::vec3 Up();
		glm::vec3 UpWorld();
		glm::vec3 Right();
		glm::vec3 RightWorld();
		bool HasChangedSince(unsigned long int tick);
		bool HasChangedLastTick();
		Transform* GetParent();
		std::vector<Transform*> GetChildren();
		void RenderHandels();

	private:

		unsigned long int m_LastChangedTick = -1;
		bool m_IsMatrixCached = false;
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::quat m_Rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 m_Scale = glm::vec3(1.0f);
		glm::mat4 m_TransformationMatrixCached;
		glm::mat4 m_TransformationMatrixLast;
		Transform* m_Parent;
		std::vector<Transform*> m_Children;

		void RemoveChild(Transform* child);
		void SetChanged();
	};


}