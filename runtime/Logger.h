#pragma once

#include "../../framework.h"
#include "../frameworks/GameFramework.h"

//Just a global variable.
extern std::ofstream GLoggerFile;

class Logger {

public:

	//Used for get type of current logger method.
	enum LoggerType {
		CONSOLE = 0,
		FILE = 1
	};

	/**
	 * Initialize log.
	 *
	 * @param type - Logger type.
	 **/

	Logger (LoggerType type) {
		if (type == LoggerType::CONSOLE) Setup ();
		this->m_Type = type;
	}

	/**
	 * Setup logger.
	 *
	 * For console example: check interfaces, set log color, show console, clear console.
	 **/

	void Setup ();

	/**
	 * Just add empty line to current logger.
	 *
	 * @param count - Lines count.
	 **/

	void Space (int count = 1);

	/**
	 * Write or show message to logger. (For console it's show)
	 * 
	 * @param message - msg.
	 **/

	void Info (const char* message...);

	/**
	 * Write or show message to alternative logger type.
	 *
	 * @pararm message - msg.
	 */

	void SwitchedInfo (const char* message...);

	/**
  	 * Clean up.
	 **/

	void Free ();

private:

	GameFramework m_Game;
	int m_Type;

};

