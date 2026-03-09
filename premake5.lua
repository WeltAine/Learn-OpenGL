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
IncludeDir["glm"] = "Learn-OpenGL/Dependency/glm"

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
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}"
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
        "%{IncludeDir.glm}/**.hpp",
        "%{IncludeDir.glm}/**.inl"
    }


    filter "system:windows"
        systemversion "latest"
        cppdialect "C++20"

        defines
        {
            "GLFW_INCLUDE_NONE" --让glfw不包含OpenGL头文件，因为我们用的是Glad来加载OpenGL函数，而且由glfw引入头文件会报错（不理解为什么要这么处理）
        }

    filter "configurations:Debug" --暂时没用，但先写着
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        
    filter "configurations:Dist"
        runtime "Release"
        optimize "On"
