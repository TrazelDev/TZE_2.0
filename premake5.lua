workspace "TRAZEL_ENGINE_2.0"
    architecture "x64"

    configurations { 
        "Debug",            -- this the configuration that I devlope in it
        "Release",          -- this is the configuration that I run the code to be fast
        "Client"            -- this is the final prduct that is going to be created
    }

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "TRAZEL_ENGINE_2.0"
    location "TRAZEL_ENGINE_2.0"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "%{prj.name}/src/TZE/pch.cpp"

    files {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp"
    }

    includedirs {
        "%{prj.name}/src/TZE",
        "third_party_extentions/glfw/include",
        "third_party_extentions/spdlog/include/spdlog",
        "third_party_extentions/spdlog/include",
    }

    links {
        "third_party_extentions/glfw/lib-vc2022/glfw3.lib"
        -- "third_party_extentions/glfw/lib-vc2022/glfw3.dll"
    }

    filter "system:windows"
        cppdialect "C++20" 
        staticruntime "off"
        runtime "Debug"
        systemversion "latest"

        defines {
            "TZE_PLATFORM_WINDOWS",
            "TZE_BUILD_DLL",
        }

        postbuildcommands {
            ("{COPY} third_party_extentions/glfw/lib-vc2022/glfw3.lib bin/" .. output_dir .. "/Client"),
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. output_dir .. "/Client")
        }

    filter "configurations:Debug"
        defines "DEBUG_MODE"
        symbols "On"
    
    filter "configurations:Release"
        defines "REALSE_MODE"
        optimize "On"

    filter "configurations:Client"
        defines "Client_MODE"
        optimize "On"


project "Client"
    location "Client"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"
    runtime "Debug"
    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "TRAZEL_ENGINE_2.0/src",
        "third_party_extentions/glfw/include",
        "third_party_extentions/spdlog/include/spdlog",
        "third_party_extentions/spdlog/include",
    }

    links {
        "TRAZEL_ENGINE_2.0"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines {
            "TZE_PLATFORM_WINDOWS"
        }

    
    filter "configurations:Debug"
        defines "DEBUG_MODE"
        symbols "On"
    
    filter "configurations:Release"
        defines "REALSE_MODE"
        optimize "On"

    filter "configurations:Client"
        kind "WindowedApp"
        defines "Client_MODE"
        symbols "On"
