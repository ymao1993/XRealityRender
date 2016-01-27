#include <stddef.h>
#include "XRApplication.h"
#include "XREngine.h"

XREngine* XREngine::engine = NULL;

XREngine* XREngine::instance()
{
	if (engine)
	{
		return engine;
	}
	else
	{
		engine = new XREngine();
		return engine;
	}
}

XREngine::XREngine(){}

bool XREngine::init(XRApplication* app)
{
	setAppliction(app);
	application->init();
	return true;
}

bool XREngine::update()
{
	application->update(0);
	return true;
}

bool XREngine::destroy()
{
	application->destroy();
	return true;
}
