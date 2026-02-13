workspace "Learn-OpenGL"
    architecture "x64"
    startproject "Learn-OpenGL"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" --比如Debug-Windows-x64，为不同平台做准备，虽然我没这个打算就是了，不过是个好习惯

IncludeDir = {}
IncludeDir["spdlog"] = "Learn-OpenGL/Dependency/spdlog/include"
IncludeDir["GLFW"] = "Learn-OpenGL/Dependency/GLFW/include"
IncludeDir["ImGui"] = "Learn-OpenGL/Dependency/ImGui"
IncludeDir["Glad"] = "Learn-OpenGL/Dependency/Glad/include"

include "Learn-OpenGL/Dependency/GLFW"
include "Learn-OpenGL/Dependency/ImGui"
include "Learn-OpenGL/Dependency/Glad"

project "Learn-OpenGL"
    location "Learn-OpenGL"
    kind "ConsoleApp"
    language "C++"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    buildoptions "/utf-8"

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.Glad}"
    }

    links{
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    files --构建时包含的文件
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }


    filter "system:windows"
        systemversion "latest"
        cppdialect "C++20"

    filter "configurations:Debug" --暂时没用，但先写着
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        
    filter "configurations:Dist"
        runtime "Release"
        optimize "On"
