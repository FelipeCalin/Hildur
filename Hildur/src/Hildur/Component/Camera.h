#pragma once

#include "Hildur/Core/Component.h"

#include <glm/glm.hpp>


namespace Hildur {


	class Renderable;

	class Camera : public Component
	{
	public:

		struct Plane
		{
			glm::vec3 normal;
			float distance;

			void normalize()
			{
				float mag = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
				normal.x = normal.x / mag;
				normal.y = normal.y / mag;
				normal.z = normal.z / mag;
				distance = distance / mag;
			}
		};

		void Init() override;
		void Destroy() override;

		void UpdateAspect();
		void UpdateProjection();
		void SetOrhographicProjection(float radius, float zNear, float zFar);
		void SetPerspectiveProjection(float fov, float aspect, float zNear, float zFar);
		void SetMain();
		bool FrustumTest(Renderable* renderable);

		glm::mat4& GetProjectionMatrix();
		glm::mat4& GetViewMatrix();
		glm::mat4& GetViewProjection();
		
		float GetFOV() { return m_Fov; }
		void SetFOV(float fov);

		static glm::vec2& WorldToScreen(const glm::vec3& point);
		static Camera* GetMainCamera() { return m_Main; }

	private:

		static Camera* m_Main;

		unsigned long int m_LastFrustumUpdate;
		float m_Fov, m_ZNear, m_ZFar, m_Aspect, m_Radius;
		bool m_IsPerspective;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		Plane m_Frustum[6];

		void UpdateFrustum();
	};


}