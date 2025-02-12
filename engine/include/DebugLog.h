#pragma once

#include <vector>
#include <string>

namespace SB
{
	enum LogType
	{
		SB_LOG = 0,
		SB_WARNING = 1,
		SB_ERROR = 2
	};

	struct LogData
	{
		LogType type;
		std::string message;
	};

	class Console
	{
	public:
		Console(const Console&) = delete;
		Console& operator=(const Console&) = delete;

		static void Init();
		static void Shutdown();

		static std::vector<LogData>& GetLogs();

		static void Clear();

		static void Log(const char* format, ...);
		static void Warn(const char* format, ...);
		static void Error(const char* format, ...);

	private:
		Console();

		static std::string EnumToString(LogType type);
		static inline std::vector<LogData> m_Logs;
	};
}