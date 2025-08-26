cmake_minimum_required(VERSION 3.28)
project(Shapes LANGUAGES CXX)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE MODULES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cppm
)

######################
# External libraries #
######################

find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

#####################
# Create executable #
#####################

add_executable(${PROJECT_NAME} ${SOURCES})

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
        Nuit
        glm::glm
        glfw
        OpenGL::GL
        GLEW::GLEW
)

# If in debug mode, create symlinks of all resource files to enable hot-reloading capabilities.
# For release/distribution builds, it should create shallow copies (new copies).
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    foreach(dir shaders)
        add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/${dir}
                COMMAND ${CMAKE_COMMAND} -E create_symlink
                ${CMAKE_CURRENT_SOURCE_DIR}/${dir}
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/${dir}
                COMMENT "Symlinking ./${dir} -> build/Debug/${dir}"
        )
    endforeach()
else()
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

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
