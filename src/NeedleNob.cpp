#include "NeedleNob.h"
#include <cmath>
#include <cstdlib>
#include <imgui.h>
#include <imgui_internal.h>

#define PI 3.14159265358979323846f

namespace ImGuiKnobs {
	namespace detail{
        void draw_needle(ImVec2 center, float radius, float angle, float length, float thickness, ImColor color) {
            auto needle_x = center[0] + cosf(angle) * radius * length;
            auto needle_y = center[1] + sinf(angle) * radius * length;

            auto angle_cos = cosf(angle + PI * 0.5f);
            auto angle_sin = sinf(angle + PI * 0.5f);

            auto half_thickness = thickness * 0.5f;

            // Calculate tip position
            ImVec2 tip = {
                center[0] + angle_cos * half_thickness,
                center[1] + angle_sin * half_thickness
            };

            ImVec2 points[3] = {
                {center[0] - angle_cos * half_thickness, center[1] - angle_sin * half_thickness},
                {needle_x, needle_y},
                tip
            };

            auto* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddTriangleFilled(points[0], points[1], points[2], color);
        }

	}
    bool NeedleKnob(const char* label, float* p_value, float v_min, float v_max, float speed = 0, const char* format = NULL, ImGuiKnobVariant variant = ImGuiKnobVariant_Tick, float size = 0, ImGuiKnobFlags flags = 0, int steps = 10) {
        
    }
}