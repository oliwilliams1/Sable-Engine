#pragma once

#include <vector>
#include <string>

enum class LogType
{
	SB_LOG = 0,
	SB_WARNING = 1,
	SB_ERROR = 2
};

namespace SB
{
	struct LogData
	{
		LogType type;
		std::string message;
		std::string file;
		int line;
		std::string func;
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

		static void Log(const char* format, const char* file, int line, const char* func, ...);
		static void Warn(const char* format, const char* file, int line, const char* func, ...);
		static void Error(const char* format, const char* file, int line, const char* func, ...);
		static void RuntimeError(const char* format, const char* file, int line, const char* func, ...);

	private:
		Console();

		static std::string EnumToString(LogType type);
		static inline std::vector<LogData> m_Logs;
	};
}

#define SABLE_LOG(format, ...)           Console::Log(format, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define SABLE_WARN(format, ...)          Console::Warn(format, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define SABLE_ERROR(format, ...)         Console::Error(format, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define SABLE_RUNTIME_ERROR(format, ...) Console::RuntimeError(format, __FILE__, __LINE__, __func__, ##__VA_ARGS__)