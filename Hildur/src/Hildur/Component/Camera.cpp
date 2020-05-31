#include "hrpcheaders.h"
#include "Camera.h"

#include "Hildur/Core/Application.h"

#include "Hildur/Component/Renderable.h"

#include <imgui.h>


namespace Hildur {


	Camera* Camera::m_Main = nullptr;

	void Camera::Init()
	{
		SetMain();

		float aspect = (float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight();
		SetPerspectiveProjection(glm::radians(70.0f), aspect, 0.01f, 10000.0f);
	}

	void Camera::Destroy()
	{
	}

	void Camera::RenderInspector()
	{
		ImGui::BeginGroup();
		const char* items[2] = { "Perspctive", "Orthographic" };
		static const char* current_item = NULL;
		float newFOV = glm::degrees(m_Fov);

		if (ImGui::BeginCombo("Projection", current_item)) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(items[n], is_selected))
					current_item = items[n];
					if (is_selected)
						ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}
		ImGui::SliderFloat("Field of View", &newFOV, 0.01f, 120.0f);
		ImGui::EndGroup();
		ImGui::NewLine();
	
		if (newFOV != glm::degrees(m_Fov))
		{
			m_Fov = glm::radians(newFOV);
			UpdateAspect();
		}

		if (current_item == "Perspctive" && !m_IsPerspective)
			SetPerspectiveProjection(glm::radians(60.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

		if (current_item == "Orthographic" && m_IsPerspective)
			SetOrhographicProjection(glm::radians(60.0f), 0.1f, 100.0f);
	}

	void Camera::UpdateAspect()
	{
		m_Aspect = (float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight();
		UpdateProjection();
	}

	void Camera::UpdateAspect(float aspect)
	{
		m_Aspect = aspect;
		UpdateProjection();
	}

	void Camera::UpdateProjection()
	{
		if (m_IsPerspective)
			m_ProjectionMatrix = glm::perspective(m_Fov, m_Aspect, m_ZNear, m_ZFar);
		else
			m_ProjectionMatrix = glm::ortho(-m_Radius * m_Aspect, m_Radius * m_Aspect, -m_Radius, m_Radius, m_ZNear, m_ZFar);
	}

	void Camera::SetFOV(float fov)
	{
		m_Fov = fov;
		UpdateProjection();
	}

	void Camera::SetOrhographicProjection(float radius, float zNear, float zFar)
	{
		m_IsPerspective = false;
		m_Radius = radius;
		m_ZNear = zNear;
		m_ZFar = zFar;
		UpdateProjection();
	}

	void Camera::SetPerspectiveProjection(float fov, float aspect, float zNear, float zFar)
	{
		m_IsPerspective = true;
		m_Fov = fov;
		m_Aspect = aspect;
		m_ZNear = zNear;
		m_ZFar = zFar;
		UpdateProjection();
	}

	void Camera::SetMain()
	{
		m_Main = this;
	}

	bool Camera::FrustumTest(Renderable* object)
	{
		UpdateFrustum();

		float maxScaleAxis = glm::max(glm::max(object->GetTransform()->GetScale().x, object->GetTransform()->GetScale().y), object->GetTransform()->GetScale().z);
		glm::vec3 center = (object->GetTransform()->GetTransformationMatrix() * glm::vec4(object->GetBoundingSphere().GetCenter(), 1.0f));
		glm::dot(m_Frustum[1].normal, center);
		for (size_t i = 0; i < 5; ++i)
		{
			if (glm::dot(m_Frustum[1].normal, center) + m_Frustum[i].distance + maxScaleAxis * object->GetBoundingSphere().GetRadius() < 0)
			{
				return false;
			}
		}
		return true;
	}

	glm::mat4& Camera::GetProjectionMatrix()
	{
		return m_ProjectionMatrix;
	}

	glm::mat4& Camera::GetViewMatrix()
	{
		//if (transform->HasChangedLastTick())
		//{
		//m_ViewMatrix = glm::lookAt(transform->GetPositionWorld(), transform->Up(), transform->Forward());
		glm::mat4 transformMat = glm::rotate(glm::mat4(1.0f), glm::radians(transform->GetRotation().x), glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(transform->GetRotation().y), glm::vec3(0, 1, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(transform->GetRotation().z), glm::vec3(0, 0, 1));
		transformMat = glm::translate(transformMat, transform->GetPositionWorld());
		
		m_ViewMatrix = glm::inverse(transformMat);
		//}

		return m_ViewMatrix;
	}

	glm::mat4& Camera::GetViewProjection()
	{
		return GetProjectionMatrix() * GetViewMatrix();
	}

	glm::vec2& Camera::WorldToScreen(const glm::vec3& point)
	{
		glm::vec4 clipSpacePos = Camera::GetMainCamera()->GetViewProjection() * glm::vec4(point, 1.0);
		if (clipSpacePos.w <= 0)
		{
			clipSpacePos.w = 0.0001f;
		}

		return glm::vec2(clipSpacePos.x / clipSpacePos.w, clipSpacePos.y / clipSpacePos.w);
	}

	void Camera::UpdateFrustum()
	{
		/*if (transform->HasChangedSince(m_LastFrustumUpdate))
		{*/
		//m_LastFrustumUpdate = Logic::getTickIndex();

		glm::mat4 m = GetViewProjection();

		//Left
		m_Frustum[0].normal.x = m[0].w + m[0].x;
		m_Frustum[0].normal.y = m[1].w + m[1].x;
		m_Frustum[0].normal.z = m[2].w + m[2].x;
		m_Frustum[0].distance = m[3].w + m[3].x;
		
		//Right
		m_Frustum[1].normal.x = m[0].w + m[0].x;
		m_Frustum[1].normal.y = m[1].w + m[1].x;
		m_Frustum[1].normal.z = m[2].w + m[2].x;
		m_Frustum[1].distance = m[3].w + m[3].x;
		
		// Top
		m_Frustum[2].normal.x = m[0].w + m[0].y;
		m_Frustum[2].normal.y = m[1].w + m[1].y;
		m_Frustum[2].normal.z = m[2].w + m[2].y;
		m_Frustum[2].distance = m[3].w + m[3].y;
		
		//Bottom
		m_Frustum[3].normal.x = m[0].w + m[0].x;
		m_Frustum[3].normal.y = m[1].w + m[1].x;
		m_Frustum[3].normal.z = m[2].w + m[2].x;
		m_Frustum[3].distance = m[3].w + m[3].x;
		
		//Near
		m_Frustum[4].normal.x = m[0].w + m[0].y;
		m_Frustum[4].normal.y = m[1].w + m[1].y;
		m_Frustum[4].normal.z = m[2].w + m[2].y;
		m_Frustum[4].distance = m[3].w + m[3].y;
		
		//Far
		m_Frustum[5].normal.x = m[0].w + m[0].y;
		m_Frustum[5].normal.y = m[1].w + m[1].y;
		m_Frustum[5].normal.z = m[2].w + m[2].y;
		m_Frustum[5].distance = m[3].w + m[3].y;

		// Normalize all plane normals
		for (int i = 0; i < 6; i++)
			m_Frustum[i].normalize();
		/*}*/
	}


}