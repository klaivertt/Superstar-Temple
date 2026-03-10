#ifndef LOGGER_H
#define LOGGER_H

#include "../../Common.hpp"

enum class ConsoleColor
{
	SUCCESS,
	INFO,
	WARNING,
	C_ERROR,
	DEBUG,
	ORANGE,
	YELLOW,
	LIGHT_RED,
	GRAY,
	LIGHT_BLUE,
	DARK_GRAY,
	DARK_BLUE,
	DARK_GREEN,
	DARK_CYAN,
	DEFAULT
};

// Log levels for filtering logs
// DEBUG: Detailed information for debugging purposes
// INFO: General information about the application's operation
// WARNING: Indications of potential issues or important events that are not errors
// C_ERROR: Critical errors that may cause the application to malfunction
// NONE: No logs will be printed, but they can still be saved to files if inFiles is true
enum class LogLevel
{
	DEBUG,
	INFO,
	WARNING,
	C_ERROR,
	NONE,
	NO_PRINT
};

class Logger
{
public:
	static Logger& Instance();

	static void Log(const std::string& text, bool inFiles = false);                 
	static void Log(LogLevel level, const std::string& text, bool inFiles = false); 

	static void Debug(const std::string& text, bool inFiles = false);
	static void Info(const std::string& text, bool inFiles = false);
	static void Warning(const std::string& text, bool inFiles = true);
	static void Error(const std::string& text, bool inFiles = true);
	static void Success(const std::string& text, bool inFiles = false);

	static void SetLogLevel(LogLevel level);
	static LogLevel GetLogLevel();

	static std::string PrintVec2(const sf::Vector2f& v, const std::string& label = "");
	static std::string PrintVec2(const sf::Vector2i& v, const std::string& label = "");
	static std::string PrintVec2(const sf::Vector2u& v, const std::string& label = "");
	static std::string Vec2(const sf::Vector2i& v, const std::string& label = "");
	static std::string Vec2(const sf::Vector2f& v, const std::string& label = "");
	static std::string Vec2(const sf::Vector2u& v, const std::string& label = "");

	static std::string PrintBool(const std::string& label, bool value);
	static std::string Bool(const std::string& label, bool value);

	static std::string PrintRect(const sf::FloatRect& r, const std::string& label = "");
	static std::string Rect(const sf::FloatRect& r, const std::string& label = "");

	static void ClearConsole();

private:
	Logger();
	~Logger() = default;

	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	void PrintColoredImpl(const std::string& text, ConsoleColor color, bool inFiles, bool havePrefix);
	void LogImpl(LogLevel level, const std::string& text, bool inFiles);

	void ClearConsoleImpl();
	void SetLogLevelImpl(LogLevel level);
	LogLevel GetLogLevelImpl();

	std::string LevelPrefix(ConsoleColor color);

	void LoadDateAndTime();
	std::string GetCurrentTimeString();
	std::string GetCurrentDateString();

private:
	float clearTime = 0.f;
	bool doClearConsole = false;

	LogLevel currentLogLevel = LogLevel::DEBUG;
	std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> timers;

	std::chrono::zoned_time<std::chrono::system_clock::duration> timeAndDate;
	std::ofstream logFile;
};

#endif // LOGGER_H