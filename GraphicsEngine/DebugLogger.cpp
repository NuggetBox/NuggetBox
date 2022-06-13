#include "NuggetBox.pch.h"
#include "DebugLogger.h"
#include <iostream>
#include "color.hpp"
#include "Timer.h"
#include <filesystem>

void DebugLogger::Log(const std::string& aMessage, MessageType aMessageType, const std::string& aFilename, const std::string& aCodeLine)
{
    myLog << GetTimeStamp();

    switch (aMessageType)
    {
    case MessageType::Message: /*myLog << "Message ";*/ break;
    case MessageType::Warning: myLog << "Warning "; break;
    case MessageType::Error: myLog << "Error "; break;
    }

    const std::filesystem::path path(aFilename);
    myLog << aMessage << " " << path.filename().string() << ":" << aCodeLine << "\n";

#ifdef _DEBUG
    std::cout << dye::grey(GetTimeStamp());
    switch (aMessageType)
    {
        case MessageType::Message: std::cout << dye::light_green(aMessage); break;
        case MessageType::Warning: std::cout << dye::light_yellow(aMessage); break;
        case MessageType::Error: std::cout << dye::light_red(aMessage); break;
    }
    std::cout << std::endl;
#endif
}

void DebugLogger::Log(const std::string& aMessage, const std::string& aFilename, const std::string& aCodeLine)
{
    Log(aMessage, MessageType::Message, aFilename, aCodeLine);
}

void DebugLogger::Warning(const std::string& aMessage, const std::string& aFilename, const std::string& aCodeLine)
{
    Log(aMessage, MessageType::Warning, aFilename, aCodeLine);
}

void DebugLogger::Error(const std::string& aMessage, const std::string& aFilename, const std::string& aCodeLine)
{
    Log(aMessage, MessageType::Error);
}

void DebugLogger::WriteCrashLog()
{
    //TODO: Fix a good filepath for crashlogs
    std::ofstream outFile;
    const std::string filename = GetTimeStamp(false) + ".txt";
    outFile.open("CrashLogs/" + filename);
    outFile << myLog.rdbuf();
    outFile.close();
}

std::string DebugLogger::GetTimeStamp(bool formatted)
{
    const std::chrono::time_point curTime = std::chrono::system_clock::now();
    const __time64_t timeT = std::chrono::system_clock::to_time_t(curTime);
    const std::chrono::time_point curTimeSec = std::chrono::system_clock::from_time_t(timeT);
    const std::chrono::milliseconds milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - curTimeSec);

    const std::tm* timeStruct = localtime(&timeT);

    const char dateFormat[] = "%Y.%m.%d %H:%M:%S";

    char timeStr[] = "yyyy.mm.dd HH:MM:SS.fff";
    strftime(timeStr, strlen(timeStr), dateFormat, timeStruct);

    if (!formatted)
    {
        std::string raw(timeStr);
        std::replace(raw.begin(), raw.end(), '.', '-');
        std::replace(raw.begin(), raw.end(), ':', '-');
        return raw;
    }

    std::string result("[");
    result.append(timeStr);
    // Show frames as last part of time?
    result.append(".");

    //Append extra 0's to fill millisecond gap
	if (milliseconds.count() < 10)
	{
        result.append("00");
	}
    else if (milliseconds.count() < 100)
    {
        result.append("0");
    }

    result.append(std::to_string(milliseconds.count()) + "] ");

    return result;
}
