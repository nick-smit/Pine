#pragma once

#define PC_IMGUI_SAME_LINE_SPACE_BETWEEN_TEXT(text) ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(text).x)
