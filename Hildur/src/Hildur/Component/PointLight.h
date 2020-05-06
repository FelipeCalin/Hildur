#pragma once

#include "Hildur/Component/LightEmitter.h"

#include <glm/glm.hpp>


namespace Hildur {


	class PointLight : public LightEmitter
	{
	public:

		~PointLight();

		void OnEnable() override;
		void OnDisable() override;

		void Init() override;
		void Destroy() override;

		glm::mat4 GetLightProjection() const { return m_LightProjection; }
		void SetLightProjection(const glm::mat4& projectionMatrix) { m_LightProjection = m_LightProjection; }

		//void renderShadowMap(bool captureMode);

		float GetRange() const { return m_Range; }
		void SetRange(float range) { m_Range = range; }

		float GetConstant() const { return m_Constant; }
		void SetConstant(float constant) { m_Constant = constant; }
		float GetLinear() const { return m_Linear; }
		void SetLinear(float linear) { m_Linear = linear; }
		float GetQuadratic() const { return m_Quadratic; }
		void SetQuadratic(float quadratic) { m_Quadratic = quadratic; }

	private:

		float m_Range = 10;

		float m_Constant = 1.0f;
		float m_Linear = 0.09f;
		float m_Quadratic = 0.032f;

		glm::mat4 m_LightProjection;
		//FrameBuffer* shadowBuffer;

		//DepthShader* _depthShader;
		//int _shadowMapResolution;
	};


}