#include "ViewerScene.h"
#include "..\..\entity\RotatingTriangle.h"
#include "GLFW\glfw3.h"


bool ViewerScene::init()
{
	//init gui
	gui = new ViewerSceneGUI();
	gui->init();

	//init game entities
	triangle = new RotatingTriangle();
	triangle->init();
	//...

	return true;
}

bool ViewerScene::update(float time)
{
	//update game logic
	triangle->update(time);
	//....


	//update gui
	gui->update(time);

	return true;
}

bool ViewerScene::destroy()
{
	delete triangle;
	delete gui;
	return true;
}