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
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
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
	ImGui::MenuItem("(demo menu)", NULL, false, false);
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
    if (ImGui::BeginMenu("Open Recent"))
    {
        ImGui::MenuItem("fish_hat.c");
        ImGui::MenuItem("fish_hat.inl");
        ImGui::MenuItem("fish_hat.h");
        if (ImGui::BeginMenu("More.."))
        {
            ImGui::MenuItem("Hello");
            ImGui::MenuItem("Sailor");
            if (ImGui::BeginMenu("Recurse.."))
            {
                //ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Save", "Ctrl+S")) {}
    if (ImGui::MenuItem("Save As..")) {}

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
	static float value = 0;

	if (ImGuiKnobs::Knob("Speed", &value, -6.0f, 6.0f, 0.1f, "%.1fdB", ImGuiKnobVariant_Wiper, 150))
	{
		// value was changed
	}
	ImGui::EndChild();
	//ImGui::SameLine();
	ImGui::BeginChild("ChildR2", size, true);
	static float arr[] = {0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f};
	ImGui::PlotHistogram("Histogram", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f, ImVec2(0, 80.0f));
	ImGui::EndChild();
	//ImGui::End();
	ImGui::SameLine();
	ImGui::BeginChild("Knob");
	//static float value = 0;
	ImGui::TextWrapped(
		"Below we are displaying the font texture (which is the only texture we have access to in this demo). "
		"Use the 'ImTextureID' type as storage to pass pointers or identifier to your own texture data. "
		"Hover the texture for a zoomed view!");

	// Below we are displaying the font texture because it is the only texture we have access to inside the demo!
	// Remember that ImTextureID is just storage for whatever you want it to be. It is essentially a value that
	// will be passed to the rendering backend via the ImDrawCmd structure.
	// If you use one of the default imgui_impl_XXXX.cpp rendering backend, they all have comments at the top
	// of their respective source file to specify what they expect to be stored in ImTextureID, for example:
	// - The imgui_impl_dx11.cpp renderer expect a 'ID3D11ShaderResourceView*' pointer
	// - The imgui_impl_opengl3.cpp renderer expect a GLuint OpenGL texture identifier, etc.
	// More:
	// - If you decided that ImTextureID = MyEngineTexture*, then you can pass your MyEngineTexture* pointers
	//   to ImGui::Image(), and gather width/height through your own functions, etc.
	// - You can use ShowMetricsWindow() to inspect the draw data that are being passed to your renderer,
	//   it will help you debug issues if you are confused about it.
	// - Consider using the lower-level ImDrawList::AddImage() API, via ImGui::GetWindowDrawList()->AddImage().
	// - Read https://github.com/ocornut/imgui/blob/master/docs/FAQ.md
	// - Read https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
	ImTextureID my_tex_id = io.Fonts->TexID;
	float my_tex_w = (float)io.Fonts->TexWidth;
	float my_tex_h = (float)io.Fonts->TexHeight;
	{
		static bool use_text_color_for_tint = false;
		ImGui::Checkbox("Use Text Color for Tint", &use_text_color_for_tint);
		ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 uv_min = ImVec2(0.0f, 0.0f); // Top-left
		ImVec2 uv_max = ImVec2(1.0f, 1.0f); // Lower-right
		ImVec4 tint_col = use_text_color_for_tint
			                  ? ImGui::GetStyleColorVec4(ImGuiCol_Text)
			                  : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
		ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
		ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
		if (ImGui::IsItemHovered() && ImGui::BeginTooltip())
		{
			float region_sz = 32.0f;
			float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
			float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
			float zoom = 4.0f;
			if (region_x < 0.0f) { region_x = 0.0f; }
			else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
			if (region_y < 0.0f) { region_y = 0.0f; }
			else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
			ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
			ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
			ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
			ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
			ImGui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
			ImGui::EndTooltip();
		}
	}
	if (ImGuiKnobs::Knob("Speed", &value, -6.0f, 6.0f, 0.1f, "%.1fdB", ImGuiKnobVariant_Wiper, 150))
	{
		// value was changed
	}
	struct Funcs
	{
		static float Sin(void*, int i) { return sinf(i * 0.1f); }
		static float Saw(void*, int i) { return (i & 1) ? 1.0f : -1.0f; }
	};
	static int func_type = 0, display_count = 70;
	float (*func)(void*, int) = (func_type == 0) ? Funcs::Sin : Funcs::Saw;
	ImGui::PlotLines("Lines", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0, 80));
	ImGui::PlotHistogram("Histogram", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0, 80));
	ImGui::Separator();
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
