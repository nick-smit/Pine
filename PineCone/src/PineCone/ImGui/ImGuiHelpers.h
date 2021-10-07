#pragma once

#define PC_IMGUI_SAME_LINE_SPACE_BETWEEN_TEXT(text) ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(text).x)

#define PC_IMGUI_BEGIN_INPUT_WIDGET_COLUMNS() ImGui::Columns(2, nullptr, false); ImGui::SetColumnWidth(0, 100.0f)
#define PC_IMGUI_END_INPUT_WIDGET_COLUMNS() ImGui::Columns(1)
