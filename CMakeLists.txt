cmake_minimum_required(VERSION 3.28)
project(Nuit LANGUAGES CXX)

#####################
# Compiler settings #
#####################

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#################################
# Set constants for the project #
#################################

file(GLOB_RECURSE SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/*.cpp
)

file(GLOB_RECURSE MODULES
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/*.cppm
)

######################
# External libraries #
######################

find_package(assimp REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_library(stb_image STATIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image.cpp)

#####################
# Create executable #
#####################

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_sources(${PROJECT_NAME}
        PUBLIC
        FILE_SET allModules
        TYPE CXX_MODULES
        FILES ${MODULES}
)

if (APPLE)
    # Silence deprecation warnings on macOS
    target_compile_definitions(${PROJECT_NAME} PRIVATE GL_SILENCE_DEPRECATION)
endif()

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        glm::glm
        glfw
        OpenGL::GL
        GLEW::GLEW
        assimp::assimp
        stb_image
        imgui::imgui
)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor
)


###############################
# Compile commands for clangd #
###############################

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
    )
endif()


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extensions)

if (BUILD_SANDBOX)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sandbox)
endif()
