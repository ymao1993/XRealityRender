#include "XRDebug.h"
#include <vector>

#include <Windows.h>

namespace XRDebug{

	struct XRLog
	{
		std::string text;
		int level;

		XRLog(std::string text, int level)
		{
			this->text = text;
			this->level = level;
		}
	};

	std::vector<XRLog> logs[XRDEBUG_LOGLEVEL_NUM];

	static const char* INVALID_LOG_LEVEL_INFO = "The log level specified is invalid.";
	static bool logLevelCheck(int level)
	{
		if (level <0 || level >= XRDEBUG_LOGLEVEL_NUM)
		{
			log(INVALID_LOG_LEVEL_INFO, XRDEBUG_LOGLEVEL_WARNNING);
			return false;
		}
		return true;
	}
	static std::string level2Str(int level)
	{
		switch (level)
		{
		case XRDEBUG_LOGLEVEL_INFO:		return "[Info]";	break;
		case XRDEBUG_LOGLEVEL_WARNNING:	return "[Warning]"; break;
		case XRDEBUG_LOGLEVEL_ERROR:	return "[Error]";	break;
		default:						return "[Unknown]"; break;
		}
	}

	static void Vlog(const char* format, va_list argList, int lev = XRDEBUG_LOGLEVEL_INFO)
	{
		const unsigned int MAX_CHARS = 1023;
		static char buffer[MAX_CHARS + 1];
		int charsWritten = vsnprintf(buffer, MAX_CHARS, format, argList);
		buffer[MAX_CHARS] = '\0';
		log(buffer, lev);
		return;
	}

	void log(std::string text, int level)
	{
		logs[level].push_back(XRLog(text, level));
	}

	void logI(const char* format, ...)
	{
		va_list argList;
		va_start(argList, format);
		Vlog(format, argList, XRDEBUG_LOGLEVEL_INFO);
		va_end(argList);
		return;
	}
	
	void logW(const char* format, ...)
	{
		va_list argList;
		va_start(argList, format);
		Vlog(format, argList, XRDEBUG_LOGLEVEL_WARNNING);
		va_end(argList);
		return;
	}

	void logE(const char* format, ...)
	{
		va_list argList;
		va_start(argList, format);
		Vlog(format, argList, XRDEBUG_LOGLEVEL_ERROR);
		va_end(argList);
		return;
	}
	void logClear(int level)
	{
		if (!logLevelCheck(level)) return;
		for (int i = 0; i < XRDEBUG_LOGLEVEL_NUM; i++)
			logs[i].clear();
	}

	std::string logs2Str(int level)
	{
		if (!logLevelCheck(level)) return std::string();

		std::string result = "";
		for each (XRLog log in logs[level])
		{
			result += level2Str(log.level) + log.text + "\n";
		}
		return result;
	}

}