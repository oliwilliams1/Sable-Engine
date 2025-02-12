#include "ConsoleDisplay.h"
#include <imgui.h>

using namespace SB;

static ImVec4 LogTypeToColour(LogType type)
{
	switch (type)
	{
	case SB_LOG:
		return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	case SB_WARNING:
		return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

	case SB_ERROR:
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

	for (const LogData& log : logs)
	{
		ImVec4 colour = LogTypeToColour(log.type);
		ImGui::TextColored(colour, "%s", log.message.c_str());
	}

	ImGui::End();
}