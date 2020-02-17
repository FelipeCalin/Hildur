workspace "Hildur"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Hildur/vendor/GLFW/include"
IncludeDir["Glad"] = "Hildur/vendor/Glad/include"
IncludeDir["ImGui"] = "Hildur/vendor/imgui"
IncludeDir["glm"] = "Hildur/vendor/glm"
IncludeDir["stb_image"] = "Hildur/vendor/stb_image"

include "Hildur/vendor/GLFW"
include "Hildur/vendor/Glad"
include "Hildur/vendor/imgui"

project "Hildur"
	location "Hildur"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hrpcheaders.h"
	pchsource "Hildur/src/hrpcheaders.cpp"

	files
	{
		"%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	defines
	{
		--"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
        "%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"HR_PLATFORM_WINDOWS",
			"HR_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "HR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HR_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
        "Hildur/vendor/spdlog/include",
		"Hildur/src",
		"Hildur/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Hildur"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"HR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "HR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HR_DIST"
		runtime "Release"
		optimize "on"