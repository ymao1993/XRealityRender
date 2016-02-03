#include "gl3w\gl3w.h"
#include "XRGLFW.h"

#include <stdlib.h>
#include <stdio.h>

#include "XREngine.h"


static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
	XRDebug::log("creating context...");
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(1024, 768, "XRealityRenderer", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	XRDebug::log("initialize gl3w...");
	if (gl3wInit()) {
		fprintf(stderr, "failed to initialize OpenGL\n");
		return -1;
	}
	if (!gl3wIsSupported(3, 2)) {
		fprintf(stderr, "OpenGL 3.2 not supported\n");
		return -1;
	}

	//initialize engine
	XRDebug::log("initializing engine...");

	{//initialize window size
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		XREngine::instance()->setWindowWH(width, height);
	}
	XREngine::instance()->init(window);

	XRDebug::log("game starts\n");

	glfwSetKeyCallback(window, XRDevice::callbackGLFW);
	while (!glfwWindowShouldClose(window))
	{
		//TODO: update it by callback?
		//update window size
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		XREngine::instance()->setWindowWH(width, height);

		//clear buffer
		GLfloat defaultBufferColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, defaultBufferColor);

		//update XREngine
		XREngine::instance()->update();

		//swap double buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	XRDebug::log("exiting...");
	//destroy engine
	XREngine::instance()->destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
