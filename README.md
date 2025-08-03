# TravelingMerchant

A C++ game engine project using CMake for building.

## Prerequisites

- Visual Studio 2019 or later (with C++ development tools)
- CMake 3.16 or later
- Windows 10/11

## Building with Visual Studio

### Method 1: Open as CMake Project (Recommended)

1. Open Visual Studio
2. Go to `File` → `Open` → `Folder`
3. Select the `TravelingMerchant` folder
4. Visual Studio will automatically detect the CMakeLists.txt and configure the project
5. Wait for CMake configuration to complete
6. Select your build configuration (Debug/Release) from the toolbar
7. Press `Ctrl+Shift+B` to build the project

### Method 2: Generate Visual Studio Solution

1. Open Command Prompt in the project directory
2. Run the build script: `build.bat`
3. Or manually run:
   ```cmd
   mkdir build
   cd build
   cmake .. -G "Visual Studio 17 2022" -A x64
   ```
4. Open the generated `.sln` file in Visual Studio

### Method 3: Command Line Build

```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

## Project Structure

- `Core/` - Main source code
- `Extern/` - External libraries (GLFW, GLM, STB)
- `Assets/` - Game assets (textures, tiles)
- `Shaders/` - GLSL shader files

## Running the Application

After building, the executable will be located at:
- Debug: `build/bin/Debug/TravelingMerchant.exe`
- Release: `build/bin/Release/TravelingMerchant.exe`

The build process automatically copies assets and shaders to the output directory.

## Troubleshooting

1. **CMake not found**: Install CMake from https://cmake.org/download/
2. **OpenGL not found**: Make sure you have graphics drivers installed
3. **Build errors**: Check that all external libraries are present in the `Extern/` directory

## Adding New Source Files

To add new source files to the project:

1. Add your `.cpp` files to the `SOURCES` list in `CMakeLists.txt`
2. Add your `.h` or `.hpp` files to the `HEADERS` list in `CMakeLists.txt`
3. Rebuild the project 