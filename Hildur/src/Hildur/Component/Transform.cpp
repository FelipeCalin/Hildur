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


	glm::vec3 Transform::forward()
	{
		glm::mat4 r = getRotationMatrix();
		return glm::vec3(r[0].z, r[1].z, r[2].z);
	}

	glm::vec3 Transform::forwardWorld()
	{
		glm::mat4 r = getTransformationMatrix();
		return glm::vec3(r[0].z, r[1].z, r[2].z);
	}

	glm::vec3 Transform::up()
	{
		glm::mat4 r = getRotationMatrix();
		return glm::vec3(r[0].y, r[1].y, r[2].y);
	}

	glm::vec3 Transform::upWorld()
	{
		glm::mat4 r = getTransformationMatrix();
		return glm::vec3(r[0].y, r[1].y, r[2].y);
	}

	glm::vec3 Transform::right()
	{
		glm::mat4 r = getRotationMatrix();
		return glm::vec3(r[0].x, r[1].x, r[2].x);
	}

	glm::vec3 Transform::rightWorld()
	{
		glm::mat4 r = getTransformationMatrix();
		return glm::vec3(r[0].x, r[1].x, r[2].x);
	}

	void Transform::setChanged()
	{
		//_lastChangedTick = Logic::getTickIndex(); //TODO: FIX!!
		_isMatrixCached = false;
	}

	bool Transform::hasChangedSince(unsigned long int tick)
	{
		return tick != _lastChangedTick;
	}

	bool Transform::hasChangedLastTick()
	{
		//return Logic::getTickIndex() == _lastChangedTick; //TODO: FIX!!
		return false;
	}

	Transform* Transform::getParent()
	{
		return _parent;
	}

	std::vector<Transform*> Transform::getChildren()
	{
		return _children;
	}

	void Transform::removeChild(Transform* child)
	{
		_children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());
	}

	void Transform::renderHandels()
	{
		{
			ImGui::BeginGroup();
			ImGui::TextUnformatted(entity->m_Name.c_str());
			ImGui::SliderFloat3(("Pos##" + entity->m_Name).c_str(), &(_position[0]), -10, 10);
			//ImGui::SliderFloat4("Rotation", &(_rotation[0]), -10, 10);
			//setChanged(); //TODO: FIX!!!
			ImGui::EndGroup();
			ImGui::NewLine();
		}
	}

	glm::mat4 Transform::getRotationMatrix()
	{
		//return _rotation.createRotation(); //TODO: FIX!!
		return glm::rotate(glm::mat4(1.0f), glm::eulerAngles(_rotation).x, glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::eulerAngles(_rotation).y, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::eulerAngles(_rotation).z, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	glm::quat Transform::getRotation()
	{
		return _rotation;
	}

	glm::vec3 Transform::getPosition()
	{
		return _position;
	}

	glm::vec3 Transform::getPositionWorld()
	{
		glm::mat4 temp = getTransformationMatrix();
		return glm::vec3(temp[3].x, temp[3].y, temp[3].z);
	}

	glm::vec3 Transform::getScale()
	{
		return _scale;
	}

	void Transform::setPosition(glm::vec3 position)
	{
		if (position != _position)
		{
			_position = position;
			//setChanged();
		}
	}

	void Transform::setRotation(glm::vec3 rotation)
	{
		setRotation(glm::quat(rotation));
	}

	void Transform::setRotation(glm::quat rotation)
	{
		if (rotation != _rotation)
		{
			_rotation = rotation;
			//setChanged();
		}
	}

	void Transform::setScale(glm::vec3 scale)
	{
		if (scale != _scale)
		{
			_scale = scale;
			//setChanged();
		}
	}

	void Transform::setParent(Transform* parent)
	{
		if (_parent == parent) return;

		if (_parent != nullptr)
		{
			_parent->removeChild(this);
		}

		_parent = parent;

		if (parent != nullptr)
			parent->_children.push_back(this);

		//setChanged();
	}

	void Transform::setParent(Entity* _entity)
	{
		setParent(_entity->m_Transform);
	}

	void Transform::rotate(glm::quat quat)
	{
		_rotation = glm::normalize(quat * _rotation);
		//setChanged();
	}

	glm::mat4 Transform::getTransformationMatrix()
	{
		if (_parent != nullptr)
			return (_parent->getTransformationMatrix()) * getLocalTransformationMatrix();
		else
			return getLocalTransformationMatrix();
	}

	glm::mat4 Transform::getLocalTransformationMatrix()
	{
		if (!_isMatrixCached)
		{
			_transformationMatrixCached = glm::mat4(1.0f);
			_transformationMatrixCached = getRotationMatrix() * glm::scale(glm::mat4(1.0f), _scale);

			/*
			_transformationMatrixCached[3, 0] = _position.x;
			_transformationMatrixCached[3, 1] = _position.y;
			_transformationMatrixCached[3, 2] = _position.z;
			*/

			_transformationMatrixCached = glm::translate(_transformationMatrixCached, _position);

			_isMatrixCached = true;
		}

		return _transformationMatrixCached;
	}



}