#pragma once

#include "imgui.h"

class ImGuiLog
{
private:
    ImGuiTextBuffer Buf;
    ImVector<int> LineOffsets;
    bool AutoScroll;
    bool ScrollToBottom;

public:
    ImGuiLog()
    {
        AutoScroll = true;
        ScrollToBottom = false;
    }

    void AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
        if (AutoScroll)
            ScrollToBottom = true;
    }

    void Draw(const char* title, bool* p_open = NULL)
    {
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        ImGui::BeginChild("scrolling", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        
        ImGui::TextUnformatted(Buf.begin(), Buf.end());
           
        ImGui::PopStyleVar();

        if (ScrollToBottom)
            ImGui::SetScrollHereY(1.0f);
        ScrollToBottom = false;
        ImGui::EndChild();
        ImGui::End();
    }
};