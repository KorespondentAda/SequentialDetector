cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME "SignalDetector")
project(${PROJECT_NAME} VERSION 0.0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME}
# Compile GLFW in-place
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
set(GLFW_LIBRARY_TYPE "STATIC")
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "GLFW for X11 only")
set(GLAD_GL "")
add_subdirectory(glfw)
include_directories("${GLFW_SOURCE_DIR}/deps")
set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h")

# My files
	main.cpp
	winMain.cpp
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
target_link_libraries(${PROJECT_NAME} PRIVATE
	OpenGL::GL
	glfw
)
target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Debug>:DEBUG>")
add_compile_options(-Wall)

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_SOURCE_DIR}/DroidSans.ttf
		${CMAKE_BINARY_DIR}/DroidSans.ttf
)

