#include "XRConsoleWindow.h"
#include "../XREngine.h"
#include "../scene/ViewerScene/ViewerScene.h"
#include <cstring>

bool XRConsoleWindow::init()
{
	memset(inputBuf, 0, sizeof(inputBuf));
	scrollToBottom = false;

	return true;
}


void XRConsoleWindow::clearLog()
{
	for (int i = 0; i < Items.Size; i++)
		free(Items[i]);
	Items.clear();
	scrollToBottom = true;
}

void XRConsoleWindow::addLog(const char* fmt, ...)
{
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf_s(buf, IM_ARRAYSIZE(buf), fmt, args);
	buf[IM_ARRAYSIZE(buf) - 1] = 0;
	va_end(args);
	Items.push_back(_strdup(buf));
	scrollToBottom = true;
}

static int callbackstub(ImGuiTextEditCallbackData* data)
{
	// no function now
	printf("Hello from callback stub\n");
	return 0;
}

static int Stricmp(const char* str1, const char* str2)               
{ 
	int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } 
	return d; 
}

static int Strnicmp(const char* str1, const char* str2, int count)   {
	int d = 0; 
	while (count > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; count--; } 
	return d; 
}


void  XRConsoleWindow::execCommand(char *command_line)
{
	addLog("# %s\n", command_line);
	rsize_t strmax = sizeof command_line;
	char *next_token;
	// Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
	HistoryPos = -1;
	for (int i = History.Size - 1; i >= 0; i--)
	if (Stricmp(History[i], command_line) == 0)
	{
		free(History[i]);
		History.erase(History.begin() + i);
		break;
	}
	History.push_back(_strdup(command_line));

	// Process command
	if (Stricmp(command_line, "clear") == 0)
	{
		clearLog();
	}
	else if (Stricmp(command_line, "help") == 0)
	{
		addLog("Commands:");
		for (int i = 0; i < Commands.Size; i++)
			addLog("- %s", Commands[i]);
	}
	else if (Stricmp(command_line, "history") == 0)
	{
		for (int i = History.Size >= 10 ? History.Size - 10 : 0; i < History.Size; i++)
			addLog("%3d: %s\n", i, History[i]);
	}
	else if (Stricmp(strtok_s(command_line, " ", &next_token), "reload") == 0)
	{
		addLog("loading... %s\n", next_token);
		addLog("deleting old object\n");
		XRScene* scene = XREngine::instance()->getScene();
		((ViewerScene *)scene)->reload();
		

	}
	else
	{
		addLog("Unknown command: '%s'\n", command_line);
	}
}

bool XRConsoleWindow::update(double time)
{
	// Update Console Window
	// Open Window
	if (!ImGui::Begin("Console", opened))
	{
		ImGui::End();
		return true;
	}
	ImGui::Separator();

	// Filter
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	static ImGuiTextFilter filter;
	filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
	ImGui::PopStyleVar();
	ImGui::Separator();

	// Scrolling Region
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::Selectable("Clear")) clearLog();
		ImGui::EndPopup();
	}
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
	for (int i = 0; i < Items.Size; i++)
	{
		const char* item = Items[i];
		if (!filter.PassFilter(item))
			continue;
		ImVec4 col = ImColor(255, 255, 255); // A better implementation may store a type per-item. For the sample let's just parse the text.
		if (strstr(item, "[error]")) col = ImColor(255, 100, 100);
		else if (strncmp(item, "# ", 2) == 0) col = ImColor(255, 200, 150);
		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::TextUnformatted(item);
		ImGui::PopStyleColor();
	}

	if (scrollToBottom)
		ImGui::SetScrollHere();
	scrollToBottom = false;
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Separator();

	// Command-line
	if (ImGui::InputText("Input", inputBuf, IM_ARRAYSIZE(inputBuf),
		ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | 
		ImGuiInputTextFlags_CallbackHistory, callbackstub, &data))
	{
		char* input_end = inputBuf + strlen(inputBuf);
		printf("string length: %d \n", strlen(inputBuf));
		while (input_end > inputBuf && input_end[-1] == ' ') input_end--; *input_end = 0;
		if (inputBuf[0])
			execCommand(inputBuf);
		strcpy_s(inputBuf, "");
	}

	ImGui::End();

	return true;
}

bool XRConsoleWindow::destroy()
{
	return true;
}

