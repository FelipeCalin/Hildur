#pragma once

#include <glm/glm.hpp>


namespace Hildur {


	class BoundingSphere
	{
	public:

		BoundingSphere() {}
		BoundingSphere(const glm::vec3& center, float radius) : m_Center(center), m_Radius(radius) {}

		glm::vec3 GetCenter() { return m_Center; };
		float GetRadius() { return m_Radius; }

	private:

		glm::vec3 m_Center;
		float m_Radius;
	};


}