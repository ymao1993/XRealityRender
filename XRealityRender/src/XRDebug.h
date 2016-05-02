#ifndef XRDEBUG_H
#define XRDEBUG_H

#include <string>

namespace XRDebug{

	enum
	{
		XRDEBUG_LOGLEVEL_ALL,
		XRDEBUG_LOGLEVEL_INFO,
		XRDEBUG_LOGLEVEL_WARNNING,
		XRDEBUG_LOGLEVEL_ERROR,
		XRDEBUG_LOGLEVEL_NUM
	};

	void log(std::string text, int level = XRDEBUG_LOGLEVEL_INFO);
	void logI(const char* format, ...);
	void logW(const char* format, ...);
	void logE(const char* format, ...);
	void logClear(int level = 0);
	std::string logs2Str(int level = XRDEBUG_LOGLEVEL_ALL);
}

#endif