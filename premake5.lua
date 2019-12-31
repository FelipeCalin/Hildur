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

 project "Hildur"
  location "Hildur"
  kind "SharedLib"
  language "C++"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  pchheader "hrpcheaders.h"
  pchsource "Hildur/src/hrpcheaders.cpp"

  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp"
  }

  includedirs
  {
    "%{prj.name}/vendor/spdlog/include",
    "%{prj.name}/src",
  }

  filter "system:windows"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"

    defines
    {
      "HR_PLATFORM_WINDOWS",
      "HR_BUILD_DLL"
    }

    postbuildcommands
    {
      ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
    }

  filter "configurations:Debug"
    defines "HR_DEBUG"
    symbols "On"

  filter "configurations:Release"
    defines "HR_RELEASE"
    optimize "On"

  filter "configurations:Dist"
    defines "HR_DIST"
    optimize "On"

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
     "Hildur/src"
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
     symbols "On"

   filter "configurations:Release"
     defines "HR_RELEASE"
     optimize "On"

   filter "configurations:Dist"
     defines "HR_DIST"
     optimize "On"