#ifndef XRENGINE_H
#define XRENGINE_H

#include <vector>

class XRApplication;

class XREngine
{
public:
	bool init(XRApplication* app);
	bool update();
	bool destroy();
	static XREngine* instance();

public:
	inline void setWindowWH(int w, int h) { windowW = w; windowH = h; }
	inline int getWindowW(){ return windowW; };
	inline int getWindowH(){ return windowH; };

private:
	XREngine();
	static XREngine* engine;
	XREngine& operator= (XREngine&) {};
	XREngine(const XREngine&){};
	XRApplication* application;

private:
	inline void setAppliction(XRApplication* app) { this->application = app; }


	int windowW;
	int windowH;
};

#endif