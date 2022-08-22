#include "NuggetBox.pch.h"
#include "DebugLogger.h"

#include <filesystem>
#include <iostream>
#include <csignal>

#include "Timer.h"
#include "color.hpp"

void DebugLogger::Log(const std::string& aMessage, MessageType aMessageType, const std::string& aFilename, const std::string& aCodeLine)
{
    myLog << "[" << GetTime() << "] ";

    switch (aMessageType)
    {
    case MessageType::Message: myLog << "[MESSAGE"; break;
    case MessageType::Warning: myLog << "[WARNING"; break;
    case MessageType::Error: myLog << "[ERROR"; break;
    }

    if (aMessageType == MessageType::Error && !aFilename.empty())
    {
		const std::filesystem::path path(aFilename);
        myLog << " in " << path.filename().string() << ":" << aCodeLine;
    }

    myLog << "] " << aMessage << std::endl;

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
    Log(aMessage, MessageType::Error, aFilename, aCodeLine);
}

void DebugLogger::SetupCrashDump()
{
    if (std::signal(SIGSEGV, SignalHandler) == SIG_ERR)
        DEBUGERROR("Unable to set signal handler for SIGSEGV");
    if (std::signal(SIGINT, SignalHandler) == SIG_ERR)
        DEBUGERROR("Unable to set signal handler for SIGINT");
    if (std::signal(SIGILL, SignalHandler) == SIG_ERR)
        DEBUGERROR("Unable to set signal handler for SIGILL");
    if (std::signal(SIGABRT, SignalHandler) == SIG_ERR)
        DEBUGERROR("Unable to set signal handler for SIGABRT");
    if (std::signal(SIGFPE, SignalHandler) == SIG_ERR)
        DEBUGERROR("Unable to set signal handler for SIGFPE");
}

void DebugLogger::WriteCrashLog()
{
    std::string filename = GetTimeStampRaw();
    std::ranges::replace(filename, ':', '-');
    std::ranges::replace(filename, '.', '-');

    std::ofstream outFile;
    outFile.open(myCrashLogsDir + filename + ".txt");
    outFile << myLog.rdbuf();
    outFile.close();
}

void DebugLogger::SignalHandler(int signal)
{
    DEBUGLOG("Crash signal detected, writing crash log file");
    WriteCrashLog();
}

std::string DebugLogger::GetTimeStamp()
{
    return "[" + GetTimeStampRaw() + "] ";
}

std::string DebugLogger::GetTimeStampRaw()
{
    const std::chrono::time_point curTime = std::chrono::system_clock::now();
    const __time64_t timeT = std::chrono::system_clock::to_time_t(curTime);
    const std::chrono::time_point curTimeSec = std::chrono::system_clock::from_time_t(timeT);
    const std::chrono::milliseconds milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - curTimeSec);

    const std::tm* timeStruct = localtime(&timeT);

    const char dateFormat[] = "%Y.%m.%d %H:%M:%S";

    char timeStr[] = "yyyy.mm.dd HH:MM:SS.fff";
    strftime(timeStr, strlen(timeStr), dateFormat, timeStruct);

    std::string result(timeStr);
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

    result.append(std::to_string(milliseconds.count()));
    return result;
}

std::string DebugLogger::GetDate()
{
    //yyyy.mm.dd HH:MM:SS.fff
    std::string timeStamp = GetTimeStampRaw();
    timeStamp.erase(10, 13);
    return timeStamp;
}

std::string DebugLogger::GetTime()
{
    std::string timeStamp = GetTimeStampRaw();
    timeStamp.erase(0, 11);
    return timeStamp;
}