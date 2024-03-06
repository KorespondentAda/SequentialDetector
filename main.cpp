#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#include <GLFW/glfw3.h>

void winMain(GLFWwindow *);

#define CLASS_NAME "SequentialSignalDetector"
// TODO Translation
#define WIN_NAME "Последовательный обнаружитель"
#define WIN_WIDTH 640
#define WIN_HEIGTH 480
#define ass(exp, msg) assert(((void)msg, exp))

namespace ImGui {

void SetNavID(ImGuiID id)
{
	ImGuiContext* g = ImGui::GetCurrentContext();
	auto win = g->CurrentWindow;
	SetFocusID(id, win);
}

}

//static bool running;
// Presumably it's called after GLFW initialization
//extern "C" void sighandler(int sig) {
//	running = false;
//}

void glfwErrorCallBack(
		int errCode,
		const char* desc)
{
	std::cerr << "GLFW Error (callback) [" << errCode << "]: " << desc << std::endl;
}

void init()
{
	// TODO Logging instruments
	std::cerr << "Program started!" << std::endl;
	std::cerr << "Compiled with GLFW version: "
		<< GLFW_VERSION_MAJOR << "."
		<< GLFW_VERSION_MINOR << "."
		<< GLFW_VERSION_REVISION << std::endl;
	std::cerr << "Using GLFW version: " << glfwGetVersionString() << std::endl;

	// Error callback initialization
	glfwSetErrorCallback(glfwErrorCallBack);

	// Before windows open — init GLFW
	if (!glfwInit()) {
		std::cerr << "GLFW Initialization fail" << std::endl;
	} else {
		std::cerr << "GLFW Initialized successfully" << std::endl;
	}

	// Setup UNIX signal handlers
	//std::signal(SIGINT, sighandler);
}

void term()
{
	// After window closes — terminate GLFW
	glfwTerminate();
	std::cerr << "GLFW Terminated" << std::endl;
}

int main()
{
	init();

	// Make window floating
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
	glfwWindowHintString(GLFW_X11_CLASS_NAME, CLASS_NAME);
	GLFWwindow *win = glfwCreateWindow(
			WIN_WIDTH,
			WIN_HEIGTH,
			WIN_NAME,
			NULL,
			NULL
	);
	ass(win, "Window and context creation failed!");
	glfwMakeContextCurrent(win);
	glfwSwapInterval(2);
	std::cerr << "Window with context created successfully!" << std::endl;

	// Init ImGui things
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(win, true);
	ImGui_ImplOpenGL3_Init(nullptr);
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	std::cerr << "ImGui settings initialized" << std::endl;

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();

		// Start Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		ImGui::Render();
		int dispW, dispH;
		glfwGetFramebufferSize(win, &dispW, &dispH);
		glViewport(0, 0, dispW, dispH);
		glClearColor(0.3f, 0.1f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(win);
	}

	// ImGui Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();

	glfwDestroyWindow(win);
	term();

	std::cerr << "Program ended!" << std::endl;
	return 0;
}

