#include "hrpcheaders.h"
#include "Shader.h"

#include "Hildur/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"


namespace Hildur {


	//  Shader  ////////////////////////////////////////////////////////////////

	Ref<Shader> Shader::Create(const std::string& filepath) {

		switch (Renderer::GetAPI()) {

			case RendererAPI::API::None:       HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:     return std::make_shared<OpenGLShader>(filepath);

		}

		HR_CORE_ASSERT(false, "Unknown RenderAPI!");

		return nullptr;

	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {

		switch (Renderer::GetAPI()) {

			case RendererAPI::API::None:       HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:     return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);

		}

		HR_CORE_ASSERT(false, "Unknown RenderAPI!");

		return nullptr;

	}


	//  Shader Library  ////////////////////////////////////////////////////////

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {

		HR_CORE_ASSERT(!Exist(name), "A Shader with that name already exists!");

		m_Shaders[name] = shader;

	}

	void ShaderLibrary::Add(const Ref<Shader>& shader) {

		auto& name = shader->GetName();
		
		Add(name, shader);

	}

	void ShaderLibrary::Remove(const std::string& name) {

		HR_CORE_ASSERT(Exist(name), "A Shader with that name doesn't exists!");

		m_Shaders.erase(name);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath) {

		auto shader = Shader::Create(filepath);
		Add(shader);

		return shader;

	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {

		auto shader = Shader::Create(filepath);
		Add(name, shader);

		return shader;

	}

	Ref<Shader> ShaderLibrary::Reload(const std::string& filepath) {

		auto shader = Shader::Create(filepath);
		
		Remove(shader->GetName());
		Add(shader);

		return shader;

	}

	Ref<Shader> ShaderLibrary::Reload(const std::string& name, const std::string& filepath) {

		auto shader = Shader::Create(filepath);

		Remove(name);
		Add(name, shader);

		return shader;

	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name) {

		HR_CORE_ASSERT(Exist(name), "A Shader with that name could not be found!");

		return m_Shaders[name];

	}

	bool ShaderLibrary::Exist(const std::string& name) const {

		return m_Shaders.find(name) != m_Shaders.end();

	}

}