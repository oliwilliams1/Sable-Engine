#include "ConsoleDisplay.h"
#include <imgui.h>

using namespace SB;

static size_t prevLogSize = 0;

static ImVec4 LogTypeToColour(LogType type)
{
	switch (type)
	{
	case LogType::SB_LOG:
		return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	case LogType::SB_WARNING:
		return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

	case LogType::SB_ERROR:
		return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

	default:
		return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void SB_EditorConsole::DisplayConsole()
{
	std::vector<LogData> logs = Console::GetLogs();

	ImGui::SetNextWindowSizeConstraints(ImVec2(400, 200), ImVec2(FLT_MAX, FLT_MAX));
	ImGui::Begin("Console");

	ImGui::BeginChild("LogChild", ImVec2(0, 0), true);

	for (const LogData& log : logs)
	{
		ImVec4 logColour = LogTypeToColour(log.type);
		ImGui::TextColored(logColour, "%s", log.message.c_str());
		
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("File: %s, line: %i, function: %s", log.file.c_str(), log.line, log.func.c_str());
		}
	}

	if (prevLogSize < logs.size())
	{
		ImGui::SetScrollHereY(1.0f);
		prevLogSize = logs.size();
	}

	ImGui::EndChild();
	ImGui::End();
}