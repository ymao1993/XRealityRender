#include "gl3w\gl3w.h"
#include <GLFW\glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#include "XREngine.h"


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

	printf("initialize gl3w...");
	if (gl3wInit()) {
		fprintf(stderr, "failed to initialize OpenGL\n");
		return -1;
	}
	if (!gl3wIsSupported(3, 2)) {
		fprintf(stderr, "OpenGL 3.2 not supported\n");
		return -1;
	}
	printf("done\n");

	//initialize engine
	printf("initializing engine...");

	{//initialize window size
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		XREngine::instance()->setWindowWH(width, height);
	}
	XREngine::instance()->init(window);
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
