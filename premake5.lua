workspace "Hildur"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
    }
    
    flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Assimp"] = "Hildur/vendor/Assimp/include"
IncludeDir["GLFW"] = "Hildur/vendor/GLFW/include"
IncludeDir["Glad"] = "Hildur/vendor/Glad/include"
IncludeDir["ImGui"] = "Hildur/vendor/imgui"
IncludeDir["glm"] = "Hildur/vendor/glm"
IncludeDir["stb_image"] = "Hildur/vendor/stb_image"
IncludeDir["zlib"] = "Hildur/vendor/zlib"

group "Dependencies"
--include "Hildur/vendor/Assimp"
include "Hildur/vendor/GLFW"
include "Hildur/vendor/Glad"
include "Hildur/vendor/imgui"
--include "Hildur/vendor/zlib"
group ""

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
        "%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		--"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.Assimp}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        --"%{IncludeDir.zlib}"
    }
    
    libdirs
    {
        "%{prj.name}/vendor/Assimp/lib"
    }

	links 
	{ 
        --"Assimp",
        "assimp-vc142-mtd.lib",
		"GLFW",
		"Glad",
        "ImGui",
        --"zlib",
        "opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
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
        "%{IncludeDir.Assimp}",
        "%{IncludeDir.glm}"     
	}

	links
	{
		"Hildur"
	}

	filter "system:windows"
		systemversion "latest"

		defines {}

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
        