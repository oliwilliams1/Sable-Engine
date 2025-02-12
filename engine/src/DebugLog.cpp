#include "DebugLog.h"
#include <iostream>
#include <iomanip>
#include <cstdarg>
#include <cstdio>
#include <chrono>
#include <ctime>

using namespace SB;

static Console* s_ConsoleInstance = nullptr;

void Console::Init()
{
	s_ConsoleInstance = new Console();
}

void Console::Shutdown()
{
	delete s_ConsoleInstance;
	s_ConsoleInstance = nullptr;
}

std::string Console::EnumToString(LogType type)
{
	switch (type) 
	{
	case SB_LOG:
		return "[LOG]\t";

	case SB_WARNING:
		return "[WARN]\t";

	case SB_ERROR:
		return "[ERROR]\t";
	}
}

Console::Console() {};

static std::string GetTime()
{
	auto now = std::chrono::system_clock::now();
	std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
	std::tm now_tm = *std::localtime(&now_time_t);

	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

	std::ostringstream oss;
	oss << std::setfill('0')
		<< std::setw(2) << now_tm.tm_hour << ':'
		<< std::setw(2) << now_tm.tm_min << ':'
		<< std::setw(2) << now_tm.tm_sec << '.'
		<< std::setw(3) << milliseconds.count();

	return "[" + oss.str() + "] - ";
}

void Console::Log(const char* format, ...)
{
	std::string time = GetTime();

	char buffer[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 1024, format, args);
	va_end(args);

	std::string finalMessage = time + buffer;

	std::cout << EnumToString(SB_LOG) << finalMessage << std::endl;
	m_Logs.push_back({SB_LOG, finalMessage});
}

void Console::Warn(const char* format, ...)
{
	std::string time = GetTime();

	char buffer[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 1024, format, args);
	va_end(args);

	std::string finalMessage = time + buffer;

	std::cout << EnumToString(SB_WARNING) << finalMessage << std::endl;
	m_Logs.push_back({ SB_WARNING, finalMessage });
}

void Console::Error(const char* format, ...)
{
	std::string time = GetTime();

	char buffer[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 1024, format, args);
	va_end(args);

	std::string finalMessage = time + buffer;

	std::cout << EnumToString(SB_ERROR) << finalMessage << std::endl;
	m_Logs.push_back({ SB_ERROR, finalMessage });
}

void Console::Clear()
{
	m_Logs.clear();
}

std::vector<LogData>& Console::GetLogs()
{
	return m_Logs;
}