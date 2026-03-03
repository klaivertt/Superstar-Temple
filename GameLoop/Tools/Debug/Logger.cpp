#include "Logger.hpp"

Logger& Logger::Instance()
{
	static Logger instance;
	return instance;
}

Logger::Logger()
{
	std::filesystem::create_directories("Logs");

	LoadDateAndTime();

	std::string logFileName = std::string("Logs/Log_") + GetCurrentDateString() + ".txt";
	logFile.open(logFileName, std::ios::app);
	if (!logFile.is_open())
	{
		std::cerr << "Failed to open log file: " << logFileName << std::endl;
	}
}

void Logger::Log(const std::string& text, bool inFiles)
{
	Info(text, inFiles);
}

void Logger::Log(LogLevel level, const std::string& text, bool inFiles)
{
	Instance().LogImpl(level, text, inFiles);
}

void Logger::Debug(const std::string& text, bool inFiles)
{
	Instance().LogImpl(LogLevel::DEBUG, text, inFiles);
}

void Logger::Info(const std::string& text, bool inFiles)
{
	Instance().LogImpl(LogLevel::INFO, text, inFiles);
}

void Logger::Warning(const std::string& text, bool inFiles)
{
	Instance().LogImpl(LogLevel::WARNING, text, inFiles);
}

void Logger::Error(const std::string& text, bool inFiles)
{
	Instance().LogImpl(LogLevel::C_ERROR, text, inFiles);
}

void Logger::Success(const std::string& text, bool inFiles)
{
	Instance().PrintColoredImpl(text, ConsoleColor::SUCCESS, inFiles, false);
}

// ----------------- Settings -----------------
void Logger::SetLogLevel(LogLevel level)
{
	Instance().SetLogLevelImpl(level);
}

LogLevel Logger::GetLogLevel()
{
	return Instance().GetLogLevelImpl();
}

void Logger::ClearConsole()
{
	Instance().ClearConsoleImpl();
}

// ----------------- Internal instance impl -----------------
void Logger::PrintColoredImpl(const std::string& _text, ConsoleColor _color, bool _inFiles, bool _havePrefix)
{
	std::string datetime = "";
	std::string prefix = "";
	if (!_havePrefix)
	{
		datetime = GetCurrentTimeString();
		prefix = datetime + LevelPrefix(_color);
	}

	if (_inFiles && logFile.is_open())
	{
		logFile << prefix << _text << std::endl;
		logFile.flush();
	}

	const char* code = "\033[0m";
	switch (_color)
	{
	case ConsoleColor::SUCCESS:  
		code = "\033[1;32m";
		break;
	case ConsoleColor::INFO:     
		code = "\033[1;36m";
		break;
	case ConsoleColor::WARNING: 
		code = "\033[1;33m"; 
		break;
	case ConsoleColor::C_ERROR: 
		code = "\033[1;31m";
		break;
	case ConsoleColor::DEBUG:    
		code = "\033[1;35m";
		break;
	case ConsoleColor::ORANGE:    
		code = "\033[0;33m";
		break;
	case ConsoleColor::YELLOW:    
		code = "\033[0;93m"; 
		break;
	case ConsoleColor::LIGHT_RED:
		code = "\033[0;91m";
		break;
	case ConsoleColor::GRAY:     
		code = "\033[0;90m";
		break;
	case ConsoleColor::LIGHT_BLUE:
		code = "\033[0;94m"; 
		break;
	case ConsoleColor::DARK_GRAY: 
		code = "\033[1;90m";
		break;
	case ConsoleColor::DARK_BLUE: 
		code = "\033[0;34m";
		break;
	case ConsoleColor::DARK_GREEN: 
		code = "\033[0;32m";
		break;
	case ConsoleColor::DARK_CYAN: 
		code = "\033[0;36m";
		break;
	case ConsoleColor::DEFAULT:
	default:                     
		code = "\033[0m";
		break;
	}

	std::cout << code << prefix << _text << "\033[0m" << std::endl;
}

void Logger::LogImpl(LogLevel _level, const std::string& _text, bool _inFiles)
{
	if (currentLogLevel > _level)
		return;

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

	PrintColoredImpl(_text, color, _inFiles, false);
}

void Logger::ClearConsoleImpl()
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

void Logger::SetLogLevelImpl(LogLevel level)
{ 
	currentLogLevel = level;
}

LogLevel Logger::GetLogLevelImpl() 
{
	return currentLogLevel;
}

std::string Logger::LevelPrefix(ConsoleColor _color)
{
	switch (_color)
	{
	case ConsoleColor::SUCCESS: 
		return " [SUCCESS] ";
	case ConsoleColor::INFO:   
		return " [INFO] ";
	case ConsoleColor::WARNING: 
		return " [WARNING] ";
	case ConsoleColor::C_ERROR: 
		return " [ERROR] ";
	case ConsoleColor::DEBUG:  
		return " [DEBUG] ";
	default:                   
		return " [LOG] ";
	}
}


std::string Logger::PrintVec2(const sf::Vector2f& v, const std::string& label)
{
	return label + ": (" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
}

std::string Logger::PrintVec2(const sf::Vector2i& v, const std::string& label)
{
	return label + ": (" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
}

std::string Logger::Vec2(const sf::Vector2i& v, const std::string& label) 
{
	return PrintVec2(v, label);
}

std::string Logger::Vec2(const sf::Vector2f& v, const std::string& label) 
{ 
	return PrintVec2(v, label);
}

std::string Logger::PrintBool(const std::string& label, bool value)
{
	return label + ": " + (value ? "true" : "false");
}

std::string Logger::Bool(const std::string& label, bool value) 
{ 
	return PrintBool(label, value);
}

std::string Logger::PrintRect(const sf::FloatRect& r, const std::string& label)
{
	return label + " [RECT]: (left: " + std::to_string(r.left) + ", top: " + std::to_string(r.top)
		+ ", width: " + std::to_string(r.width) + ", height: " + std::to_string(r.height) + ")";
}

std::string Logger::Rect(const sf::FloatRect& r, const std::string& label) 
{ 
	return PrintRect(r, label);
}


void Logger::LoadDateAndTime()
{
	auto now = std::chrono::system_clock::now();
	timeAndDate = std::chrono::zoned_time<std::chrono::system_clock::duration>(
		std::chrono::current_zone(), now);
}

std::string Logger::GetCurrentTimeString()
{
	using namespace std::chrono;

	auto now = system_clock::now();
	auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

	std::time_t t = system_clock::to_time_t(now);
	std::tm localTime;
	localtime_s(&localTime, &t);

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