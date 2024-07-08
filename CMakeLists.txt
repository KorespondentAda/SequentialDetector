cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME "SignalDetector")
project(${PROJECT_NAME} VERSION 0.0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(OpenGL REQUIRED)

# Compile GLFW in-place
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
set(GLFW_LIBRARY_TYPE "STATIC")
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "GLFW for X11 only")
set(GLAD_GL "")
add_subdirectory(glfw)
set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h")

# ImGuiFileDialog including
add_subdirectory(imgui-file-dialog)
# Specify ImGui includes
target_include_directories(ImGuiFileDialog PRIVATE ${CMAKE_SOURCE_DIR}/imgui ${CMAKE_SOURCE_DIR})
target_compile_definitions(ImGuiFileDialog PUBLIC "CUSTOM_IMGUIFILEDIALOG_CONFIG=<FileDialogConfig.hpp>")

# Add time measurement elements
option(SD_MEASURE_TIME "Measure characteristics modelling time" OFF)

add_executable(${PROJECT_NAME} WIN32
# My files
	main.cpp
	winMain.cpp
	FontDroidSans.cpp
# ImGui
	imgui/imgui.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
	imgui/backends/imgui_impl_glfw.cpp
	imgui/backends/imgui_impl_opengl3.cpp
	imgui/misc/cpp/imgui_stdlib.cpp
# ImPlot
	implot/implot.cpp
	implot/implot_items.cpp
	implot/implot_demo.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/imgui
	${CMAKE_SOURCE_DIR}/imgui/backends
	${CMAKE_SOURCE_DIR}/imgui/misc/cpp
	${CMAKE_SOURCE_DIR}/implot/
	${GLFW_INCLUDE_DIR}
)
set(LINK_STATIC "")
if (WIN32)
	set(LINK_STATIC "-static")
endif(WIN32)
target_link_libraries(${PROJECT_NAME} PRIVATE
	OpenGL::GL
	glfw
	ImGuiFileDialog
	${LINK_STATIC}
)
target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Debug>:DEBUG>")
if (${SD_MEASURE_TIME})
	target_compile_definitions(${PROJECT_NAME} PUBLIC "MEASURE_TIME")
endif(${SD_MEASURE_TIME})
add_compile_options(-Wall)

