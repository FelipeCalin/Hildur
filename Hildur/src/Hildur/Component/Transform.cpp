#include "hrpcheaders.h"
#include "Transform.h"

#include "Hildur/Core/Entity.h"

//#include "PhotonBox/component/Camera.h"
//#include "PhotonBox/core/Entity.h"
//#include "PhotonBox/core/system/DebugGUI.h"
//#include "PhotonBox/math/Math.h"
//#include "PhotonBox/core/system/Logic.h"

#include <algorithm>

#include <imgui.h>


namespace Hildur {


	glm::vec3 Transform::Forward()
	{
		glm::mat4 r = GetRotationMatrix();
		return glm::vec3(r[0].z, r[1].z, r[2].z);
	}

	glm::vec3 Transform::ForwardWorld()
	{
		glm::mat4 r = GetTransformationMatrix();
		return glm::vec3(r[0].z, r[1].z, r[2].z);
	}

	glm::vec3 Transform::Up()
	{
		glm::mat4 r = GetRotationMatrix();
		return glm::vec3(r[0].y, r[1].y, r[2].y);
	}

	glm::vec3 Transform::UpWorld()
	{
		glm::mat4 r = GetTransformationMatrix();
		return glm::vec3(r[0].y, r[1].y, r[2].y);
	}

	glm::vec3 Transform::Right()
	{
		glm::mat4 r = GetRotationMatrix();
		return glm::vec3(r[0].x, r[1].x, r[2].x);
	}

	glm::vec3 Transform::RightWorld()
	{
		glm::mat4 r = GetTransformationMatrix();
		return glm::vec3(r[0].x, r[1].x, r[2].x);
	}

	void Transform::SetChanged()
	{
		//_lastChangedTick = Logic::getTickIndex(); //TODO: FIX!!
		m_IsMatrixCached = false;
	}

	bool Transform::HasChangedSince(unsigned long int tick)
	{
		return tick != m_LastChangedTick;
	}

	bool Transform::HasChangedLastTick()
	{
		//return Logic::getTickIndex() == _lastChangedTick; //TODO: FIX!!
		return false;
	}

	Transform* Transform::GetParent()
	{
		return m_Parent;
	}

	std::vector<Transform*> Transform::GetChildren()
	{
		return m_Children;
	}

	void Transform::RemoveChild(Transform* child)
	{
		m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child), m_Children.end());
	}

	void Transform::RenderHandels()
	{
		{
			ImGui::BeginGroup();
			ImGui::TextUnformatted(entity->m_Name.c_str());
			ImGui::SliderFloat3(("Pos##" + entity->m_Name).c_str(), &(m_Position[0]), -10, 10);
			//ImGui::SliderFloat4("Rotation", &(_rotation[0]), -10, 10);
			//setChanged(); //TODO: FIX!!!
			ImGui::EndGroup();
			ImGui::NewLine();
		}
	}

	glm::mat4 Transform::GetRotationMatrix()
	{
		//return _rotation.createRotation(); //TODO: FIX!!
		return glm::rotate(glm::mat4(1.0f), glm::eulerAngles(m_Rotation).x, glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::eulerAngles(m_Rotation).y, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::eulerAngles(m_Rotation).z, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	glm::quat Transform::GetRotation()
	{
		return m_Rotation;
	}

	glm::vec3 Transform::GetPosition()
	{
		return m_Position;
	}

	glm::vec3 Transform::GetPositionWorld()
	{
		glm::mat4 temp = GetTransformationMatrix();
		return glm::vec3(temp[3].x, temp[3].y, temp[3].z);
	}

	glm::vec3 Transform::GetScale()
	{
		return m_Scale;
	}

	void Transform::SetPosition(glm::vec3 position)
	{
		if (position != m_Position)
		{
			m_Position = position;
			//setChanged();
		}
	}

	void Transform::SetRotation(glm::vec3 rotation)
	{
		SetRotation(glm::quat(rotation));
	}

	void Transform::SetRotation(glm::quat rotation)
	{
		if (rotation != m_Rotation)
		{
			m_Rotation = rotation;
			//setChanged();
		}
	}

	void Transform::SetScale(glm::vec3 scale)
	{
		if (scale != m_Scale)
		{
			m_Scale = scale;
			//setChanged();
		}
	}

	void Transform::SetParent(Transform* parent)
	{
		if (m_Parent == parent) return;

		if (m_Parent != nullptr)
		{
			m_Parent->RemoveChild(this);
		}

		m_Parent = parent;

		if (parent != nullptr)
			parent->m_Children.push_back(this);

		//setChanged();
	}

	void Transform::SetParent(Entity* _entity)
	{
		SetParent(_entity->m_Transform);
	}

	void Transform::Rotate(glm::quat quat)
	{
		m_Rotation = glm::normalize(quat * m_Rotation);
		//setChanged();
	}

	glm::mat4 Transform::GetTransformationMatrix()
	{
		if (m_Parent != nullptr)
			return (m_Parent->GetTransformationMatrix()) * GetLocalTransformationMatrix();
		else
			return GetLocalTransformationMatrix();
	}

	glm::mat4 Transform::GetLocalTransformationMatrix()
	{
		if (!m_IsMatrixCached)
		{
			m_TransformationMatrixCached = glm::mat4(1.0f);
			m_TransformationMatrixCached = GetRotationMatrix() * glm::scale(glm::mat4(1.0f), m_Scale);

			/*
			_transformationMatrixCached[3, 0] = _position.x;
			_transformationMatrixCached[3, 1] = _position.y;
			_transformationMatrixCached[3, 2] = _position.z;
			*/

			m_TransformationMatrixCached = glm::translate(m_TransformationMatrixCached, m_Position);

			m_IsMatrixCached = true;
		}

		return m_TransformationMatrixCached;
	}


}