#pragma once


namespace Hildur {


	class Timestep {

	public:

		Timestep(float time = 0.0f)
			:m_Time(time) {


		}

		operator float() const { return m_Time; }

		float GetTimeSeconds() const { return m_Time; }
		float GetTimeMiliseconds() const { return m_Time * 1000.0f; }

	private:

		float m_Time;

	};


}