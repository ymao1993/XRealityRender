#ifndef XRCONSOLE_H
#define XRCONSOLE_H

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

#include "../XRWidget.h"
#include "../XRIMUI.h"

class XRConsoleWindow: public XRWidget
{
private:
	char inputBuf[256];
	bool* opened;
	bool scrollToBottom;
	ImGuiTextEditCallbackData data;
	ImVector<char*>       Items;
	ImVector<char*>       History;
	int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
	ImVector<const char*> Commands;

	void  execCommand(char* command_line);
	void clearLog();
	void addLog(const char* fmt, ...);

public:
	virtual bool init();
	virtual bool update(double time);
	virtual bool destroy();
	
};


#endif