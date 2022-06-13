#pragma once
#include <sstream>

#define LOG(message) DebugLogger::Log(message, MessageType::Message, __FILE__, std::to_string(__LINE__))
#define WARNING(message) DebugLogger::Log(message, MessageType::Warning, __FILE__, std::to_string(__LINE__))
#define ERROR(message) DebugLogger::Log(message, MessageType::Error, __FILE__, std::to_string(__LINE__))

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

	//TODO: File write log
	static void WriteCrashLog();

private:
	static std::string GetTimeStamp(bool formatted = true);
	inline static std::stringstream myLog;
};