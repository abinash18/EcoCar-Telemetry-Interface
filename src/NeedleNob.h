#pragma once
#include "imgui-knobs.h"

namespace ImGuiKnobs {
    bool NeedleKnob(const char* label, float* p_value, float v_min, float v_max, float speed = 0, const char* format = NULL, ImGuiKnobVariant variant = ImGuiKnobVariant_Tick, float size = 0, ImGuiKnobFlags flags = 0, int steps = 10);;
} // namespace ImGuiKnobs