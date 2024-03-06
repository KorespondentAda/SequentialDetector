cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME "SignalDetector")
project(${PROJECT_NAME} VERSION 0.0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME}
# My files
	main.cpp
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
)
target_link_libraries(${PROJECT_NAME} PRIVATE
	OpenGL::GL
	glfw ${GLFW_LIBRARIES}
)
target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Debug>:DEBUG>")
add_compile_options(-Wall)

