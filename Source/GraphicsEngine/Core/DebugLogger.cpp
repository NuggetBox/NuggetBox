#include "NuggetBox.pch.h"
#include "DebugLogger.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <csignal>

#include "color.hpp"

void DebugLogger::Log(const std::string& aMessage, MessageType aMessageType, const std::string& aFilename, const std::string& aCodeLine)
{
    //Write to Log
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
    //

#ifdef _DEBUG
    //Write to console
    std::cout << dye::grey(GetTimeStamp()) << " ";
    switch (aMessageType)
    {
        case MessageType::Message: std::cout << dye::light_green(aMessage); break;
        case MessageType::Warning: std::cout << dye::light_yellow(aMessage); break;
        case MessageType::Error: std::cout << dye::light_red(aMessage); break;
    }
    std::cout << std::endl;
    //
#endif
}

void DebugLogger::Message(const std::string& aMessage, const std::string& aFilename, const std::string& aCodeLine)
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
    signal;
    DEBUGLOG("Crash signal detected, writing crash log file");
    WriteCrashLog();
}

//[yyyy.mm.dd HH:MM:SS.fff] 
std::string DebugLogger::GetTimeStamp()
{
    return "[" + GetTimeStampRaw() + "]";
}

//yyyy.mm.dd HH:MM:SS.fff
std::string DebugLogger::GetTimeStampRaw()
{
    std::string result("Yo");
    return result;
}

//yyyy.mm.dd
std::string DebugLogger::GetDate()
{
    std::string timeStamp = GetTimeStampRaw();
    timeStamp.erase(10, 13);
    return timeStamp;
}

//HH:MM:SS.fff
std::string DebugLogger::GetTime()
{
    std::string timeStamp = GetTimeStampRaw();
    timeStamp.erase(0, 11);
    return timeStamp;
}