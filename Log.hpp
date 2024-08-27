#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include <Windows.h>
#include <debugapi.h>
#include <format>
#include <chrono>

namespace Log
{
    enum class Level : unsigned int
    {
	    Info,
	    Debug,
	    Warning,
	    Error,
    };

    void DebugLog(const std::string& filename, const unsigned int line, const std::string& functionName, const std::string& log, const Level logLevel);
}

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define LOG_INFO(log) Log::DebugLog(__FILENAME__, __LINE__, __func__, log, Log::Level::Info);
#define LOG_DEBUG(log) Log::DebugLog(__FILENAME__, __LINE__, __func__, log, Log::Level::Debug);
#define LOG_WARNING(log) Log::DebugLog(__FILENAME__, __LINE__, __func__, log, Log::Level::Warning);
#define LOG_ERROR(log) Log::DebugLog(__FILENAME__, __LINE__, __func__, log, Log::Level::Error);

#ifdef DEBUG
#define Assertion(expression, log) if(!(expression)) { LOG_ERROR(log); __debugbreak(); }
#else
#define Assertion(expression, log) if(!(expression)) { LOG_ERROR(log); abort(); }
#endif

namespace Log
{
    static inline std::ofstream m_logFile;

	static void OpenFile(const std::filesystem::path& filename)
	{
		if (m_logFile && m_logFile.is_open())
		{
            DebugLog(__FILENAME__, __LINE__, __func__, "Log file already open", Level::Warning); // Appel direct
            return;
		}

		m_logFile.open(filename, std::ios::out);
		if (!m_logFile)
		{
            DebugLog(__FILENAME__, __LINE__, __func__, "Fail to open log file " + filename.string(), Level::Warning); // Appel direct
            return;
		}
		else
		{
            DebugLog(__FILENAME__, __LINE__, __func__, "Open log file " + filename.string(), Level::Info); // Appel direct
        }
	}

	static void CloseFile()
	{
		m_logFile.close();
        DebugLog(__FILENAME__, __LINE__, __func__, "Close log file", Level::Info); // Appel direct
    }

    static void Print(const std::string& log, const Level& logLevel)
    {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        std::string logWithLogLevel;

        switch (logLevel)
        {
        case Level::Info:
        {
            SetConsoleTextAttribute(handle, 11); // texte in blue
            std::cout << "[Info]   ";
            SetConsoleTextAttribute(handle, 15); // texte in white (default)
            std::cout << log << std::endl;
            logWithLogLevel = "[Info] " + log + '\n';
            break;
        }

        case Level::Debug:
        {
            SetConsoleTextAttribute(handle, 05); // texte in green
            std::cout << "[Debug]  ";
            SetConsoleTextAttribute(handle, 15); // texte in white (default)
            std::cout << log << std::endl;
            logWithLogLevel = "[Debug] " + log + '\n';
            break;
        }

        case Level::Warning:
            SetConsoleTextAttribute(handle, 14); // texte in yellow
            std::cout << "[Warning]";
            SetConsoleTextAttribute(handle, 15); // texte in white (default)
            std::cout << log << std::endl;
            logWithLogLevel = "[Warning] " + log + '\n';
            break;

        case Level::Error:
            SetConsoleTextAttribute(handle, 207); // background in red, texte in white
            std::cout << "[Error]";
            SetConsoleTextAttribute(handle, 15); // texte in white (default)
            std::cout << "  " + log << std::endl;
            logWithLogLevel = "[Error] " + log + '\n';
            break;

        default:
            std::cout << log << std::endl;;
            logWithLogLevel = log + '\n';
            break;
        }

        if (m_logFile.is_open())
        {
            m_logFile << logWithLogLevel;
        }
    }

    inline void DebugLog(const std::string& filename, const unsigned int line, const std::string& functionName, const std::string& log, const Level logLevel)
    {
        std::string time = std::format("{:%T}", floor<std::chrono::seconds>(std::chrono::system_clock::now()));
        std::string message = "[" + time + "]" + filename + '(' + std::to_string(line) + "):" + functionName + ": " + log;
        OutputDebugStringA(message.c_str());
        Print(message, logLevel);
    }
}