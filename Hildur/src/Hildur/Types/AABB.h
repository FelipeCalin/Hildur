#pragma once

#include <glm/glm.hpp>


namespace Hildur {


	class AABB
	{
	public:

		AABB(glm::vec3 min, glm::vec3 max)
			: m_Min(min), m_Max(max) {};
		~AABB() = default;

		glm::vec3 GetCenter() const { return m_Center; }

		glm::vec3 GetMin() const { return m_Min; }
		glm::vec3 GetMax() const { return m_Max; }
	
	private:

		glm::vec3 m_Center;

		glm::vec3 m_Min;
		glm::vec3 m_Max;
	};


}