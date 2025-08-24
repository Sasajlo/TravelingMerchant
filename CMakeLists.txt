cmake_minimum_required(VERSION 3.16)
project(TravelingMerchant VERSION 1.0.0 LANGUAGES CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

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
    Source/Core/Transform.cpp

    Source/Graphics/Camera.cpp
    Source/Graphics/SpriteRenderer.cpp
    Source/Graphics/TextRenderer.cpp
    Source/Graphics/ImageRenderer.cpp
    Source/Graphics/Shader.cpp
    Source/Graphics/Sprite.cpp
    Source/Graphics/Texture.cpp

    Source/Physics/Raycast.cpp

    Source/Utils/Vector3.cpp
    Source/Utils/Color.cpp
    Source/Utils/Math.cpp
    
    Source/Audio/AudioSource.cpp
    
    Extern/glad/src/glad.c
)

# Add header files
set(HEADERS
    Source/Core/Core.hpp
    Source/Core/Engine.hpp
    Source/Core/Scene.hpp
    Source/Core/SceneManager.hpp
    Source/Core/Window.hpp
    Source/Core/Time.hpp
    Source/Core/Input.hpp
    Source/Core/GameObject.hpp
    Source/Core/Component.hpp
    Source/Core/Transform.hpp

    Source/Graphics/Graphics.hpp
    Source/Graphics/Camera.hpp
    Source/Graphics/Sprite.hpp
    Source/Graphics/Shader.hpp
    Source/Graphics/SpriteRenderer.hpp
    Source/Graphics/ImageRenderer.hpp
    Source/Graphics/TextRenderer.hpp
    Source/Graphics/Texture.hpp

    Source/Physics/Physics.hpp
    Source/Physics/Raycast.hpp

    Source/Utils/Utils.hpp
    Source/Utils/Vector3.hpp
    Source/Utils/Color.hpp
    Source/Utils/File.hpp
    Source/Utils/Math.hpp
    Source/Utils/Size.hpp

    Source/Game/Game.hpp
    Source/Game/Player.hpp
    Source/Game/Debug.hpp
    Source/Game/Billboard.hpp
    Source/Game/Interactable.hpp
    Source/Game/SpriteManager.hpp

    Source/Game/Inventory/Inventory.hpp
    
    Source/Audio/AudioSource.hpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} "Source/Game/Interactable.cpp")

# Set the GLAD source file to be compiled as C
set_source_files_properties(Extern/glad/src/glad.c PROPERTIES LANGUAGE C)

# FMOD configuration
set(FMOD_DIR "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows" CACHE PATH "Path to FMOD Studio/Core API")

# Add FMOD include directory
include_directories(${FMOD_DIR}/api/core/inc)

# Link libraries (GLFW + FMOD)
target_link_libraries(${PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/Extern/glfw-3.4.bin.WIN64/lib-vc2022/glfw3_mt.lib
    $<$<CONFIG:Debug>:${FMOD_DIR}/api/core/lib/x64/fmodL_vc.lib>
    $<$<NOT:$<CONFIG:Debug>>:${FMOD_DIR}/api/core/lib/x64/fmod_vc.lib>
)

# Copy FMOD runtime DLLs to output directory (Debug uses fmodL.dll)
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<$<CONFIG:Debug>:${FMOD_DIR}/api/core/lib/x64/fmodL.dll>
            $<$<NOT:$<CONFIG:Debug>>:${FMOD_DIR}/api/core/lib/x64/fmod.dll>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

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