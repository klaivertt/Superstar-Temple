#include "Logger.hpp"
#include <filesystem>

Logger::Logger(void)
{
	//Create a new log file with current date
	std::filesystem::create_directories("Logs");

	LoadDateAndTime();

	std::string logFileName = std::string("Logs/Log_") + GetCurrentDateString() + ".txt";
	logFile.open(logFileName, std::ios::app);
	if (!logFile.is_open())
	{
		Error("Failed to open log file: " + logFileName, true);
	}
}

void Logger::PrintColored(const std::string& _text, ConsoleColor _color, bool _inFiles, bool _havePrefix)
{
	std::string datetime = "";
	std::string prefix = "";
	if (!_havePrefix)
	{
		datetime = GetCurrentTimeString();
		prefix = datetime + LevelPrefix(_color);
	}
	// Écriture dans le fichier de log si ouvert
	if (_inFiles && logFile.is_open())
	{
		logFile << prefix << _text << std::endl;
		logFile.flush();
	}

	// POSIX : codes ANSI
	const char* code = "\033[0m";
	switch (_color)
	{
	case ConsoleColor::SUCCESS:
		code = "\033[1;32m";
		break; // bright green
	case ConsoleColor::INFO:
		code = "\033[1;36m"; // bright cyan 
		break;
	case ConsoleColor::WARNING:
		code = "\033[1;33m"; // bright yellow
		break;
	case ConsoleColor::C_ERROR:
		code = "\033[1;31m"; // bright red
		break;
	case ConsoleColor::DEBUG:
		code = "\033[1;35m"; // bright magenta
		break;
	case ConsoleColor::ORANGE:
		code = "\033[0;33m"; // orange
		break;
	case ConsoleColor::YELLOW:
		code = "\033[0;93m"; // light yellow
		break;
	case ConsoleColor::LIGHT_RED:
		code = "\033[0;91m"; // light red
		break;
	case ConsoleColor::GRAY:
		code = "\033[0;90m"; // gray
		break;
	case ConsoleColor::LIGHT_BLUE:
		code = "\033[0;94m"; // light blue
		break;
	case ConsoleColor::DARK_GRAY:
		code = "\033[1;90m"; // dark gray
		break;
	case ConsoleColor::DARK_BLUE:
		code = "\033[0;34m"; // dark blue
		break;
	case ConsoleColor::DARK_GREEN:
		code = "\033[0;32m"; // dark green
		break;
	case ConsoleColor::DARK_CYAN:
		code = "\033[0;36m"; // dark cyan
		break;
	case ConsoleColor::DEFAULT:
	default:
		code = "\033[0m"; // default
		break;
	}

	std::cout << code << prefix << _text << "\033[0m" << std::endl;
}

void Logger::Log(LogLevel _level, const std::string& _text, bool _inFiles)
{
	if (currentLogLevel > _level)
	{
		return;
	}

	ConsoleColor color = ConsoleColor::DEFAULT;
	switch (_level)
	{
	case LogLevel::DEBUG:
		color = ConsoleColor::DEBUG;
		break;
	case LogLevel::INFO:
		color = ConsoleColor::INFO;
		break;
	case LogLevel::WARNING:
		color = ConsoleColor::WARNING;
		break;
	case LogLevel::C_ERROR:
		color = ConsoleColor::C_ERROR;
		break;
	default:
		color = ConsoleColor::DEFAULT;
		break;
	}
	PrintColored(_text, color, _inFiles, false);
}

void Logger::Log(LogLevel _level, const std::string& _prefix, const std::string& _text, ConsoleColor _color)
{
	if (currentLogLevel > _level)
	{
		return;
	}
	const std::string datetime = GetCurrentTimeString();
	const std::string fullText = datetime + " [" + _prefix + "] " + _text;

	// Affichage dans la console
	PrintColored(fullText, _color, false, true);
}

void Logger::ClearConsole(void)
{
	if (!doClearConsole)
	{
		return;
	}
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void Logger::SetLogLevel(LogLevel level)
{
	currentLogLevel = level;
}

LogLevel Logger::GetLogLevel(void)
{
	return currentLogLevel;
}

std::string Logger::LevelPrefix(ConsoleColor _color)
{
	switch (_color)
	{
	case ConsoleColor::SUCCESS:
		return std::string(" [SUCCESS] ");
	case ConsoleColor::INFO:
		return std::string(" [INFO] ");
	case ConsoleColor::WARNING:
		return std::string(" [WARNING] ");
	case ConsoleColor::C_ERROR:
		return std::string(" [ERROR] ");
	case ConsoleColor::DEBUG:
		return std::string(" [DEBUG] ");
	default:
		return std::string(" [LOG] ");
	}
}

#pragma region Simplified log interfaces
void Logger::Warning(const std::string& text, bool _inFiles)
{
	if (currentLogLevel <= LogLevel::WARNING)
	{
		PrintColored(text, ConsoleColor::WARNING, _inFiles, false);
	}
}

void Logger::Error(const std::string& text, bool _inFiles)
{
	if (currentLogLevel <= LogLevel::C_ERROR)
	{
		PrintColored(text, ConsoleColor::C_ERROR, _inFiles, false);
	}
}

void Logger::Success(const std::string& text, bool _inFiles)
{
	if (currentLogLevel <= LogLevel::INFO)
	{
		PrintColored(text, ConsoleColor::SUCCESS, _inFiles, false);
	}
}

void Logger::Debug(const std::string& text, bool _inFiles)
{
	if (currentLogLevel <= LogLevel::DEBUG)
	{
		PrintColored(text, ConsoleColor::DEBUG, _inFiles, false);
	}
}

void Logger::Info(const std::string& text, bool _inFiles)
{
	if (currentLogLevel <= LogLevel::INFO)
	{
		PrintColored(text, ConsoleColor::INFO, _inFiles, false);
	}
}
#pragma endregion

#pragma region Print simplified interfaces
std::string Logger::PrintVec2(const sf::Vector2f& _v, const std::string& _label)
{
	return _label + ": (" + std::to_string(_v.x) + ", " + std::to_string(_v.y) + ")";
}

std::string Logger::PrintVec2(const sf::Vector2i& _v, const std::string& _label)
{
	return _label + ": (" + std::to_string(_v.x) + ", " + std::to_string(_v.y) + ")";
}

std::string Logger::Vec2(const sf::Vector2i& _v, const std::string& _label)
{
	return PrintVec2(_v, _label);
}

std::string Logger::Vec2(const sf::Vector2f& _v, const std::string& _label)
{
	return PrintVec2(_v, _label);
}

std::string Logger::PrintBool(const std::string& label, bool value)
{
	return label + ": " + (value ? "true" : "false");
}

std::string Logger::Bool(const std::string& _label, bool _value)
{
	return PrintBool(_label, _value);
}

std::string Logger::PrintRect(const sf::FloatRect& r, const std::string& label)
{
	return label + " [RECT]: (left: " + std::to_string(r.left) + ", top: " + std::to_string(r.top)
		+ ", width: " + std::to_string(r.width) + ", height: " + std::to_string(r.height) + ")";
}

std::string Logger::Rect(const sf::FloatRect& _r, const std::string& _label)
{
	return PrintRect(_r, _label);
}
#pragma endregion

#pragma region Date and Time functions
void Logger::LoadDateAndTime()
{
	auto now = std::chrono::system_clock::now();
	timeAndDate = std::chrono::zoned_time<std::chrono::system_clock::duration>(
		std::chrono::current_zone(), now);
}

std::string Logger::GetCurrentDateTimeString()
{
	auto tp = timeAndDate.get_local_time();
	return std::format("[ {:%Y-%m-%d %H:%M:%S} ]",
		std::chrono::zoned_time{ std::chrono::current_zone(), tp });
}

std::string Logger::GetCurrentTimeString()
{
	using namespace std::chrono;

	auto now = system_clock::now();
	auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

	std::time_t t = system_clock::to_time_t(now);
	std::tm localTime;
	localtime_s(&localTime, &t); // version sûre, spécifique à MSVC

	std::ostringstream oss;
	oss << "[ "
		<< std::put_time(&localTime, "%H:%M:%S")
		<< '.' << std::setw(3) << std::setfill('0') << ms.count()
		<< " ]";

	return oss.str();
}

std::string Logger::GetCurrentDateString()
{
	auto tp = timeAndDate.get_local_time();

	auto tp_seconds = std::chrono::time_point_cast<std::chrono::seconds>(tp);

	return std::format("{:%Y-%m-%d_%H-%M-%S}",
		std::chrono::zoned_time{ std::chrono::current_zone(), tp_seconds });
}
#pragma endregion