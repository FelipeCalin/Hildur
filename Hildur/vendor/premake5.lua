project "ImGui"

	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	IncludeDir = {}
	IncludeDir["SFML"] = "../SFML/include"
	IncludeDir["Glad"] = "../Glad/include"


	files
	{
		"imconfig.h",
		"imgui.h",
		"imgui.cpp",
		"imgui_draw.cpp",
		"imgui_internal.h",
		"imgui_widgets.cpp",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"imgui_demo.cpp",
		"imgui-SFML.cpp",
		"imgui-SFML.h",
		"imgui-SFML_export.h"
	}

	includedirs
    {
		"%{IncludeDir.SFML}"
	}
	
	links
	{
		"opengl32.lib",
		"sfml-audio",
    	"sfml-graphics",
    	"sfml-main",
    	"sfml-network",
    	"sfml-system",
		"sfml-window"
	}

	defines 
    {
        "SFML_STATIC"
    }


	filter "system:windows"

		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"


	filter "system:linux"

		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"


	filter "configurations:Debug"

		runtime "Debug"
		buildoptions "/MDd"
		symbols "on"


	filter "configurations:Release"

		runtime "Release"
		buildoptions "/MD"
		optimize "on"


	filter "configurations:Dist"

		runtime "Release"
		buildoptions "/MD"
		optimize "on"