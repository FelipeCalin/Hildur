#pragma once

#include "Hildur/Component/LightEmitter.h"

#include <glm/glm.hpp>


namespace Hildur {


	class DirectionalLight : public LightEmitter
	{
	public:

		~DirectionalLight();

		void OnEnable() override;
		void OnDisable() override;

		void Init() override;
		void Destroy() override;

		glm::vec3 GetDirection() const { return m_Direction; }
		void SetDirection(const glm::vec3& direction) { m_Direction = direction; }

		glm::mat4 GetLightProjection() const { return m_LightProjection; }
		void SetLightProjection(const glm::mat4& projectionMatrix) { m_LightProjection = m_LightProjection; }

		//void renderShadowMap(bool captureMode);

	private:

		glm::vec3 m_Direction;
		glm::mat4 m_LightProjection;
		//FrameBuffer* shadowBuffer;

		//DepthShader* _depthShader;
		//int _shadowMapResolution;
	};


}