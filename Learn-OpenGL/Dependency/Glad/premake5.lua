project "Glad"
    location "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "on" --使用静态运行时库（比如msvcrt.lib），以获得更好的性能和较小的二进制文件

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/glad/gl.h",
        "include/KHR/khrplatform.h",
        "src/gl.c"
    }

    includedirs{
        "include"
    }

    filter "system:windows"
        systemversion "latest"

        -- defines
        -- {
        --     "GLAD_GLAPI_EXPORT"
        -- }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"

    filter {"system:windows", "configurations:Release"}
        buildoptions "/MD"