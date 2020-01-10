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
  IncludeDir["SFML"] = "Hildur/vendor/SFML/include"
  IncludeDir["Glad"] = "Hildur/vendor/Glad/include"
  IncludeDir["ImGui"] = "Hildur/vendor/imgui"
  IncludeDir["GLM"] = "Hildur/vendor/GLM"

  group "Dependencies"

    include "Hildur/vendor/SFML"
    include "Hildur/vendor/Glad"
    include "Hildur/vendor/ImGui"

  group ""

  project "Hildur"
    location "Hildur"
    kind "SharedLib"
    language "C++"
    staticruntime "off"
  
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
  
    pchheader "hrpcheaders.h"
    pchsource "Hildur/src/hrpcheaders.cpp"
  
    files
    {
      "%{prj.name}/src/**.h",
      "%{prj.name}/src/**.cpp",
      "%{prj.name}/vendor/GLM/glm/**.hpp",
      "%{prj.name}/vendor/GLM/glm/**.inl"
    }
  
    includedirs
    {
      "%{prj.name}/vendor/spdlog/include",
      "%{prj.name}/src",
      --"%{IncludeDir.SFML}",
		  --"%{IncludeDir.Glad}",
      --"%{IncludeDir.ImGui}",
      --"%{IncludeDir.GLM}"
      "%{prj.name}/vendor/SFML/include",
      "%{prj.name}/vendor/Glad/include",
      "%{prj.name}/vendor/ImGui",
      "%{prj.name}/vendor/GLM"
    }
  
    libdirs 
    { 
      "%{prj.name}/vendor/SFML/extlibs/bin/x64",
      "%{prj.name}/vendor/SFML/extlibs/libs-msvc-universal/x64"
    }
  
    links
    {
      "Glad",                   --Glad
      "ImGui",                  --ImGui
      "sfml-audio",             --SFML
      "sfml-graphics",
      "sfml-main",
      "sfml-network",
      "sfml-system",
      "sfml-window",
      "sfml-system",
      "openal32",               --SFML Dependencies
      "flac",
      "vorbisenc",
      "vorbisfile",
      "vorbis",
      "ogg",
      "opengl32.lib"
    }
    
	  defines
	  {
	  	"SFML_STATIC"
	  }

    filter "system:windows"
      cppdialect "C++17"
      systemversion "latest"

      defines
      {
        "HR_PLATFORM_WINDOWS",
        "HR_BUILD_DLL"
      }

      postbuildcommands
      {
        ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
      }

    filter "configurations:Debug"
      defines "HR_DEBUG"
      runtime "Debug"
      symbols "On"

    filter "configurations:Release"
      defines "HR_RELEASE"
      runtime "Release"
      optimize "On"

    filter "configurations:Dist"
      defines "HR_DIST"
      runtime "Release"
      optimize "On"


-- Sandbox test project (placeholder for actual app)

 project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

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
      "%{IncludeDir.SFML}",
      --"%{IncludeDir.GLM}"
      "Hildur/vendor/GLM"
    }

    links
    {
      "Hildur"
    }

    filter "system:windows"
      cppdialect "C++17"
      staticruntime "On"
      systemversion "latest"

      defines
      {
        "HR_PLATFORM_WINDOWS"
      }

    filter "configurations:Debug"
      defines "HR_DEBUG"
      buildoptions "/MDd"
      symbols "On"

    filter "configurations:Release"
      defines "HR_RELEASE"
      buildoptions "/MD"
      optimize "On"

    filter "configurations:Dist"
      defines "HR_DIST"
      buildoptions "/MD"
      optimize "On"