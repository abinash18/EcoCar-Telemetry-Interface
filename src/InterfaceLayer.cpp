#include "InterfaceLayer.h"

#include <AEngine/core/CoreEngine.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/include/GLFW/glfw3.h"

using namespace AEngine;

InterfaceLayer::InterfaceLayer() : AEngine::Layer("Telemetry Interface") {}
InterfaceLayer::~InterfaceLayer() {}

void InterfaceLayer::attach() {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// (void) io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding              = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	CoreEngine& app = CoreEngine::Get();

	auto window = static_cast<GLFWwindow*>(app.m_window->getWindowHandle());

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void InterfaceLayer::detach() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

void InterfaceLayer::render() {
	ImGuiIO& io = ImGui::GetIO();
	begin();

	int bar_data[11] =
		{0, 1, 2, 1, 2, 6, 5, 5, 5, 4, 6};
	float x_data[1000] = {0, 1};
	float y_data[1000] = {0, 1};

	ImGui::Begin("Test");
	if (ImPlot::BeginPlot("My Plot")) {
		ImPlot::PlotBars("My Bar Plot", bar_data, 11);
		ImPlot::PlotLine("My Line Plot", x_data, y_data, 1000);
		ImPlot::EndPlot();
	}
	ImGui::End();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);
	ImPlot::ShowDemoWindow();
	ImGui::Begin("Window A");
	ImGui::Text("This is window A");
	ImGui::End();

	ImGui::ShowMetricsWindow();

	end();
}

void InterfaceLayer::update(float delta) {
	Layer::update(delta);
	ImGuiIO& io    = ImGui::GetIO();
	io.DisplaySize = ImVec2(CoreEngine::s_instance->m_window->getWidth(),
	                        CoreEngine::s_instance->m_window->getHeight());
	io.DeltaTime = delta;
}

void InterfaceLayer::begin() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void InterfaceLayer::end() {
	ImGuiIO&    io  = ImGui::GetIO();
	CoreEngine& app = CoreEngine::Get();
	io.DisplaySize  = ImVec2(app.m_window->getWidth(), app.m_window->getHeight());

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}
