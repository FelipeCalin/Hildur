#pragma once

#include <glm/glm.hpp>


namespace Hildur {


	class BoundingSphere
	{
	public:

		BoundingSphere() {}
		BoundingSphere(const glm::vec3& center, float radius) : m_Center(center), m_Radius(radius) {}

		glm::vec3 GetCenter() const { return m_Center; };
		void SetCentre(const glm::vec3& center) { m_Center = center; }

		float GetRadius() const { return m_Radius; }
		void SetRadius(float radius) { m_Radius = radius; }

	private:

		glm::vec3 m_Center;
		float m_Radius;
	};


}