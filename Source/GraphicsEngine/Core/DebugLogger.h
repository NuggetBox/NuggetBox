#pragma once
#include <sstream>
#include <format>

//Example use: DEBUGLOG("Hello world!");
#define DEBUGLOG(message) DebugLogger::Log(message, MessageType::Message, __FILE__, std::to_string(__LINE__))
#define DEBUGWARNING(message) DebugLogger::Log(message, MessageType::Warning, __FILE__, std::to_string(__LINE__))
#define DEBUGERROR(message) DebugLogger::Log(message, MessageType::Error, __FILE__, std::to_string(__LINE__))

//Example use: FORMATLOG("Model with entity id: {} and modelpath: {} loaded", entity.id, entity.modelPath);
//Output: Model with entity id: 32 and modelpath: Models/Niklas.fbx loaded
#define FORMATLOG(...) DebugLogger::FormattedLog(MessageType::Message, __FILE__, std::to_string(__LINE__), __VA_ARGS__)
#define FORMATWARNING(...) DebugLogger::FormattedLog(MessageType::Warning, __FILE__, std::to_string(__LINE__), __VA_ARGS__)
#define FORMATERROR(...) DebugLogger::FormattedLog(MessageType::Error, __FILE__, std::to_string(__LINE__), __VA_ARGS__)

enum class MessageType
{
	Message,
	Warning,
	Error
};

class DebugLogger
{
public:
	template <typename... Args>
	static void FormattedLog(MessageType aMessageType, const std::string& aFilename = "", const std::string& aCodeLine = "", const std::string& aFormat = "", Args&&... someArgs);

	static void Log(const std::string& aMessage, MessageType aMessageType, const std::string& aFilename = "", const std::string& aCodeLine = "");

	static void Message(const std::string& aMessage, const std::string& aFilename = "", const std::string& aCodeLine = "");
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

template<typename... Args>
void DebugLogger::FormattedLog(MessageType aMessageType, const std::string& aFilename, const std::string& aCodeLine, const std::string& aFormat, Args&&... someArgs)
{
	const std::string message = std::vformat(aFormat, std::make_format_args(someArgs...));
	Log(message, aMessageType, aFilename, aCodeLine);
}