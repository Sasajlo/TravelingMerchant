cmake_minimum_required(VERSION 3.16)
project(TravelingMerchant VERSION 1.0.0 LANGUAGES CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find required packages
find_package(OpenGL REQUIRED)

# Set include directories for external libraries
include_directories(${CMAKE_SOURCE_DIR}/Extern/glfw-3.4.bin.WIN64/include)
include_directories(${CMAKE_SOURCE_DIR}/Extern/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/Extern/glm)
include_directories(${CMAKE_SOURCE_DIR}/Extern)
include_directories(${CMAKE_SOURCE_DIR}/Source)  # Add Source directory to include path

# Add source files
set(SOURCES
    Source/main.cpp
    Source/Core/Engine.cpp
    Source/Core/Scene.cpp
    Source/Core/SceneManager.cpp
    Source/Core/Window.cpp
    Source/Core/Time.cpp
    Source/Core/Input.cpp
    Source/Core/GameObject.cpp
    Source/Core/Component.cpp

    Source/Graphics/Camera.cpp
    Source/Graphics/SpriteRenderer.cpp

    Source/Utils/Vector3.cpp
    Source/Utils/Color.cpp
    
    Extern/glad/src/glad.c
)

# Add header files
set(HEADERS
    Source/Core/Engine.hpp
    Source/Core/Scene.hpp
    Source/Core/SceneManager.hpp
    Source/Core/Window.hpp
    Source/Core/Time.hpp
    Source/Core/Input.hpp
    Source/Core/GameObject.hpp
    Source/Core/Component.hpp

    Source/Graphics/Camera.hpp
    Source/Graphics/SpriteRenderer.hpp

    Source/Utils/Vector3.hpp
    Source/Utils/Color.hpp
    Source/Utils/File.hpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} "Source/Graphics/Shader.cpp" "Source/Graphics/Sprite.cpp")

# Set the GLAD source file to be compiled as C
set_source_files_properties(Extern/glad/src/glad.c PROPERTIES LANGUAGE C)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    OpenGL::GL
    ${CMAKE_SOURCE_DIR}/Extern/glfw-3.4.bin.WIN64/lib-vc2022/glfw3_mt.lib
)

# Copy DLL files to output directory (for Windows)
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/Extern/glfw-3.4.bin.WIN64/lib-vc2022/glfw3.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

# Set compiler-specific flags
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

# Copy assets to output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets
)

# Copy shaders to output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Shaders
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders
)