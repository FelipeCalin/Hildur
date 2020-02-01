#pragma once


namespace Hildur {


	enum class RendererAPI {

		None = 0,
		OpenGL = 1

	};

	class Renderer {

	public:

		inline static RendererAPI GetAPI() { return m_Renderer; };

	private:

		static RendererAPI m_Renderer;

	};


}