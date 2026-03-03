#ifndef LOGGER_H
#define LOGGER_H

#include "../../Common.hpp"
#include <chrono>

// Couleurs de console pour les messages
// SUCCESS : vert
// INFO : cyan
// WARNING : jaune
// C_ERROR : rouge
// DEBUG : magenta
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

// Niveau de log pour filtrer les messages
// Permet de ne pas afficher les messages en dessous d'un certain niveau
// DEBUG : tous les messages
// INFO : messages d'info, warning et error
// WARNING : messages warning et error
// C_ERROR : seulement les messages d'erreur
// NONE : aucun message
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
	Logger(void);
	// Logs / Console
	// Affiche un message coloré dans la console native (stdout). Color mapping dépend de la plateforme/terminal.
	void PrintColored(const std::string& _text, ConsoleColor _color, bool _inFiles, bool _havePrefix);

	// Log un message avec un niveau spécifié
	// @param _level: Niveau de log du message
	// @param _text: Texte du message
	// @param _inFiles: Si true, écrit aussi dans le fichier de log
	void Log(LogLevel _level, const std::string& _text, bool _inFiles = false);
	void Log(LogLevel _level, const std::string& _prefix, const std::string& _text, ConsoleColor _color);
	// Vide la console (implémentation dépend de la plateforme — à implémenter en .cpp)
	void ClearConsole(void);

	// Reglage du niveau de log (les messages sous ce niveau ne seront pas affichés)
	// Par défaut DEBUG (affiche tout)
	// @param level: Niveau de log à définir
	void SetLogLevel(LogLevel level);
	LogLevel GetLogLevel(void);

	// Simplified log interfaces
	// Affiche un message de log avec le niveau et la couleur appropriés

	// @brief Usage: log->Debug("This is your message");
	// @brief result: [HH:MM:SS] [DEBUG] This is your message
	// @param bool _inFiles: Si true, écrit aussi dans le fichier de log
	void Debug(const std::string& text, bool _inFiles = false);
	// @brief Usage: log->Info("This is your message");
	// @brief result: [HH:MM:SS] [INFO] This is your message
	// @param bool _inFiles: Si true, écrit aussi dans le fichier de log
	void Info(const std::string& text, bool _inFiles = false);

	// @brief Usage: log->Warning("This is your message");
	// @brief result: [HH:MM:SS] [WARNING] This is your message
	// @param bool _inFiles: Si true, écrit aussi dans le fichier de log
	void Warning(const std::string& text, bool _inFiles = true);
	// @brief Usage: log->Error("This is your message");
	// @brief result: [HH:MM:SS] [ERROR] This is your message
	// @param bool _inFiles: Si true, écrit aussi dans le fichier de log
	void Error(const std::string& text, bool _inFiles = true);
	// @brief Usage: log->Success("This is your message");
	// @brief result: [HH:MM:SS] [SUCCESS] This is your message
	// @param bool _inFiles: Si true, écrit aussi dans le fichier de log
	void Success(const std::string& text, bool _inFiles = false);

	// Timers pour mesurer des sections de code
	// startTimer("Load a Map"); ... stopTimer("Load a Map") -> durée en ms 
	// exemple d'utilisation dans un load pour connaitre le temps de chargemnt
	//void StartTimer(const std::string& id);
	//double StopTimer(const std::string& id); // retourne ms, -1.0 si timer absent

	// get a 2D vector as string with optional label
	std::string PrintVec2(const sf::Vector2f& _v, const std::string& _label = "");
	std::string PrintVec2(const sf::Vector2i& _v, const std::string& _label = "");
	std::string Vec2(const sf::Vector2i& _v, const std::string& _label = "");
	std::string Vec2(const sf::Vector2f& _v, const std::string& _label = "");

	// Get a boolean as string with optional label
	std::string PrintBool(const std::string& _label, bool _value);
	std::string Bool(const std::string& _label, bool _value);

	// Get a rectangle as string with optional label
	std::string PrintRect(const sf::FloatRect& _r, const std::string& _label = "");
	std::string Rect(const sf::FloatRect& _r, const std::string& _label = "");
	std::string LevelPrefix(ConsoleColor _color);

private:
	// console clearing timer
	float clearTime = 0.f;
	bool doClearConsole = false;

	// logging / timers
	LogLevel currentLogLevel = LogLevel::DEBUG;
	std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> timers;


	// Time and date for logs
	std::chrono::zoned_time<std::chrono::system_clock::duration> timeAndDate;
	std::ofstream logFile;

	void LoadDateAndTime();
	std::string GetCurrentDateTimeString();
	std::string GetCurrentTimeString();
	std::string GetCurrentDateString();
};



#endif // !LOGGER_H
