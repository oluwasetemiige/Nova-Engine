cmake_minimum_required(VERSION 3.20)
project(NovaEngine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type" FORCE)
endif()

option(NOVA_BUILD_EDITOR "Build the engine + editor (downloads GLFW, GLM, ImGui, GLAD)" ON)
option(NOVA_BUILD_TESTS  "Build the dependency-free core tests" ON)

# Warnings are applied to our own targets only, never to third-party code.
function(nova_set_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive- /utf-8)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

add_subdirectory(Nova)

if(NOVA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()

if(NOVA_BUILD_EDITOR)
    add_subdirectory(NovaEditor)
endif()
