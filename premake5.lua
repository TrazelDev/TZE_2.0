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
        "third_party_extentions",
        "third_party_extentions/glfw/include",
        "third_party_extentions/spdlog/include/spdlog",
        "third_party_extentions/spdlog/include",
        "third_party_extentions/vulkan_sdk/Include",
        "third_party_extentions/glm-master",
        "third_party_extentions/glm-master/glm/detail",
        "third_party_extentions/glm-master/glm",
        "third_party_extentions/glm-master/glm/gtc"
    }

    links {
        "third_party_extentions/glfw/lib-vc2022/glfw3.lib",
        "third_party_extentions/vulkan_sdk/Lib/vulkan-1.lib"
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
        
        prebuildcommands 
        {
            ("$(ProjectDir)src\\TZE\\layers\\Vulkan\\pipeline\\Shaders\\glslc.exe $(ProjectDir)src\\TZE\\layers\\Vulkan\\pipeline\\Shaders\\shader.frag -o $(ProjectDir)..\\Client\\fragment.spv"),
            ("$(ProjectDir)src\\TZE\\layers\\Vulkan\\pipeline\\Shaders\\glslc.exe $(ProjectDir)src\\TZE\\layers\\Vulkan\\pipeline\\Shaders\\shader.vert -o$(ProjectDir)..\\Client\\vertex.spv"),

        }
        postbuildcommands {
            ("{COPY} third_party_extentions/glfw/lib-vc2022/glfw3.lib bin/" .. output_dir .. "/Client"),
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. output_dir .. "/Client"),
        }


    filter "configurations:Debug"
        defines "DEBUG_MODE"
        symbols "On"
    
    filter "configurations:Release"
        defines "REALSE_MODE"
        optimize "On"

    filter "configurations:Client"
        defines "CLIENT_MODE"
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
        "TRAZEL_ENGINE_2.0/src/TZE",
        "third_party_extentions",
        "third_party_extentions/glfw/include",
        "third_party_extentions/spdlog/include/spdlog",
        "third_party_extentions/spdlog/include",
        "third_party_extentions/vulkan_sdk/Include",
        "third_party_extentions/glm-master",
        "third_party_extentions/glm-master/glm/detail",
        "third_party_extentions/glm-master/glm",
        "third_party_extentions/glm-master/glm/gtc"
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

        links {
            "third_party_extentions/glfw/lib-vc2022/glfw3.lib",
            "third_party_extentions/vulkan_sdk/Lib/vulkan-1.lib"
        }
    
    filter "configurations:Debug"
        defines "DEBUG_MODE"
        symbols "On"
    
    filter "configurations:Release"
        defines "REALSE_MODE"
        optimize "On"

    filter "configurations:Client"
        kind "WindowedApp"
        defines "CLIENT_MODE"
        symbols "On"
