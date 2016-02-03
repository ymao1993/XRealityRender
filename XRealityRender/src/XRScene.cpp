#include "XRScene.h"

bool XRScene::init()
{
	//init ui
	ui = createUserInterface();
	ui->init();

	//init scene
	initScene();

	return true;
}

bool XRScene::update(double deltaTime)
{
	updateScene(deltaTime);
	ui->update(deltaTime);
	return true;
}

bool XRScene::destroy()
{
	destroyScene();
	ui->destroy();
	delete ui;
	return true;
}