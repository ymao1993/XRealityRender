#include "XRDebug.h"
#include <vector>

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
		case XRDEBUG_LOGLEVEL_ALL:		return "[All]";		break;
		case XRDEBUG_LOGLEVEL_INFO:		return "[Info]";	break;
		case XRDEBUG_LOGLEVEL_WARNNING:	return "[Warning]"; break;
		case XRDEBUG_LOGLEVEL_ERROR:	return "[Error]";	break;
		default:						return "[Unknown]"; break;
		}
	}


	void log(std::string text, int level)
	{
		logs[level].push_back(XRLog(text, level));
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
		if (level == XRDEBUG_LOGLEVEL_ALL)
		{
			for (int i = XRDEBUG_LOGLEVEL_ALL + 1; i < XRDEBUG_LOGLEVEL_NUM; i++)
			{
				result += logs2Str(i);
			}
		}
		else
		{
			for each (XRLog log in logs[level])
			{
				result += level2Str(log.level) + log.text + "\n";
			}
		}
		return result;
	}

}