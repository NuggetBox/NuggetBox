#pragma once
#include <sstream>

#define DEBUGLOG(message) DebugLogger::Log(message, MessageType::Message, __FILE__, std::to_string(__LINE__))
#define DEBUGWARNING(message) DebugLogger::Log(message, MessageType::Warning, __FILE__, std::to_string(__LINE__))
#define DEBUGERROR(message) DebugLogger::Log(message, MessageType::Error, __FILE__, std::to_string(__LINE__))

enum class MessageType
{
	Message,
	Warning,
	Error
};

class DebugLogger
{
public:
	static void Log(const std::string& aMessage, MessageType aMessageType, const std::string& aFilename = "", const std::string& aCodeLine = "");

	static void Log(const std::string& aMessage, const std::string& aFilename = "", const std::string& aCodeLine = "");
	static void Warning(const std::string& aMessage, const std::string& aFilename = "", const std::string& aCodeLine = "");
	static void Error(const std::string& aMessage, const std::string& aFilename = "", const std::string& aCodeLine = "");

	static void SetupCrashDump();
	static void WriteCrashLog();

private:
	static void SignalHandler(int signal);

	static std::string GetTimeStamp();
	static std::string GetTimeStampRaw();
	static std::string GetDate();
	static std::string GetTime();

	inline static const std::string myCrashLogsDir = "CrashLogs/";
	inline static std::stringstream myLog;
};