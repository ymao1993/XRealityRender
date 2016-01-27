#include <GLFW\glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#include "XREngine.h"
#include "XRApplication.h"


static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

int main(void)
{
	printf("creating context...");
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(800, 600, "XRealityRenderer", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	printf("done\n");

	//initialize engine
	printf("initializing engine...");
	XRApplication application;
	glfwSetKeyCallback(window, XRApplication::key_callback);
	XREngine::instance()->init(&application);
	printf("done\n");

	printf("game starts\n");
	while (!glfwWindowShouldClose(window))
	{
		//TODO: update it by callback?
		//update window size
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		XREngine::instance()->setWindowWH(width, height);

		//clear buffer
		glClear(GL_COLOR_BUFFER_BIT);

		//update XREngine
		XREngine::instance()->update();

		//swap double buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	printf("exiting...");
	//destroy engine
	XREngine::instance()->destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	printf("done\n");
	exit(EXIT_SUCCESS);
}
