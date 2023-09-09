#include "InterfaceLayer.h"

#include <AEngine/core/CoreEngine.h>

#include "imgui-knobs.h"
#include "GLFW/include/GLFW/glfw3.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//using namespace ImGui;

InterfaceLayer::InterfaceLayer() : AEngine::Layer("Telemetry Interface")
{
}

InterfaceLayer::~InterfaceLayer()
{
}

void InterfaceLayer::attach()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// (void) io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	AEngine::CoreEngine& app = AEngine::CoreEngine::Get();

	auto window = static_cast<GLFWwindow*>(app.m_window->getWindowHandle());

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void InterfaceLayer::detach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

const int x[] = {1, 2, 2, 3, 3};
const int y[] = {1, 1, 1, 3, 2};

struct ScrollingBuffer
{
	int MaxSize;
	int Offset;
	ImVector<ImVec2> Data;

	ScrollingBuffer(int max_size = 2000)
	{
		MaxSize = max_size;
		Offset = 0;
		Data.reserve(MaxSize);
	}

	void AddPoint(float x, float y)
	{
		if (Data.size() < MaxSize)
			Data.push_back(ImVec2(x, y));
		else
		{
			Data[Offset] = ImVec2(x, y);
			Offset = (Offset + 1) % MaxSize;
		}
	}

	void Erase()
	{
		if (Data.size() > 0)
		{
			Data.shrink(0);
			Offset = 0;
		}
	}
};

// utility structure for realtime plot
struct RollingBuffer
{
	float Span;
	ImVector<ImVec2> Data;

	RollingBuffer()
	{
		Span = 10.0f;
		Data.reserve(2000);
	}

	void AddPoint(float x, float y)
	{
		float xmod = fmodf(x, Span);
		if (!Data.empty() && xmod < Data.back().x)
			Data.shrink(0);
		Data.push_back(ImVec2(xmod, y));
	}
};

constexpr ImVec2 size(400, 400);

template <typename T>
inline T RandomRange(T min, T max)
{
	T scale = rand() / (T)RAND_MAX;
	return min + scale * (max - min);
}

void InterfaceLayer::render()
{
	ImGuiIO& io = ImGui::GetIO();
	begin();
	ImGui::Begin("Vehicle Telemetry");
	ImGui::SeparatorText("Child windows");
	ImGui::BeginChild("ChildR", size, true);
	//  SeparatorText("Voltage");
	if (ImPlot::BeginPlot("Voltage"))
	{
		ImPlot::PlotDigital("Voltage", x, y, 5, 0, 0, 0);
		ImPlot::EndPlot();
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("ChildR1", size, true);

	if (ImPlot::BeginPlot("Rolling"))
	{
		static ScrollingBuffer sdata1, sdata2;
		static RollingBuffer rdata1, rdata2;
		ImVec2 mouse = ImGui::GetMousePos();
		static float t = 0;
		t += ImGui::GetIO().DeltaTime;
		sdata1.AddPoint(t, mouse.x * 0.0005f);
		rdata1.AddPoint(t, mouse.x * 0.0005f);
		sdata2.AddPoint(t, mouse.y * 0.0005f);
		rdata2.AddPoint(t, mouse.y * 0.0005f);

		static float history = 10.0f;
		ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");
		rdata1.Span = history;
		rdata2.Span = history;
		static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;
		ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
		ImPlot::SetupAxisLimits(ImAxis_X1, 0, history, ImGuiCond_Always);
		ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
		ImPlot::PlotLine("Mouse X", &rdata1.Data[0].x, &rdata1.Data[0].y, rdata1.Data.size(), 0, 0, 2 * sizeof(float));
		ImPlot::PlotLine("Mouse Y", &rdata2.Data[0].x, &rdata2.Data[0].y, rdata2.Data.size(), 0, 0, 2 * sizeof(float));
		ImPlot::EndPlot();
	}

	ImGui::EndChild();
	//ImGui::SameLine();
	ImGui::BeginChild("ChildR2", size, true);
	static float xs[1001], ys[1001], ys1[1001], ys2[1001], ys3[1001], ys4[1001];
	srand(0);
	for (int i = 0; i < 1001; ++i)
	{
		xs[i] = i * 0.001f;
		ys[i] = 0.25f + 0.25f * sinf(25 * xs[i]) * sinf(5 * xs[i]) + RandomRange(-0.01f, 0.01f);
		ys1[i] = ys[i] + RandomRange(0.1f, 0.12f);
		ys2[i] = ys[i] - RandomRange(0.1f, 0.12f);
		ys3[i] = 0.75f + 0.2f * sinf(25 * xs[i]);
		ys4[i] = 0.75f + 0.1f * cosf(25 * xs[i]);
	}
	static float alpha = 0.25f;
	ImGui::DragFloat("Alpha", &alpha, 0.01f, 0, 1);

	if (ImPlot::BeginPlot("Shaded Plots"))
	{
		ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, alpha);
		ImPlot::PlotShaded("Uncertain Data", xs, ys1, ys2, 1001);
		ImPlot::PlotLine("Uncertain Data", xs, ys, 1001);
		ImPlot::PlotShaded("Overlapping", xs, ys3, ys4, 1001);
		ImPlot::PlotLine("Overlapping", xs, ys3, 1001);
		ImPlot::PlotLine("Overlapping", xs, ys4, 1001);
		ImPlot::PopStyleVar();
		ImPlot::EndPlot();
	}

	ImGui::EndChild();
	//ImGui::End();
	ImGui::SameLine();
	ImGui::BeginChild("Knob");
	static float value = 0;

	if (ImGuiKnobs::Knob("Speed", &value, -6.0f, 6.0f, 0.1f, "%.1fdB", ImGuiKnobVariant_Wiper, 150))
	{
		// value was changed
	}
	ImGui::EndChild();
	ImGui::End();

	//ImGui::ShowMetricsWindow();
	//ImPlot::ShowDemoWindow();
	end();
}

void InterfaceLayer::update(float delta)
{
	Layer::update(delta);
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(AEngine::CoreEngine::s_instance->m_window->getWidth(),
	                        AEngine::CoreEngine::s_instance->m_window->getHeight());
	io.DeltaTime = delta;
}

void InterfaceLayer::begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void InterfaceLayer::end()
{
	ImGuiIO& io = ImGui::GetIO();
	AEngine::CoreEngine& app = AEngine::CoreEngine::Get();
	io.DisplaySize = ImVec2(app.m_window->getWidth(), app.m_window->getHeight());

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	//ImGui::EndFrame();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}
