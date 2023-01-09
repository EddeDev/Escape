#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Game", nullptr, nullptr);
	
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	
	glfwShowWindow(window);

	bool running = true;
	while (running)
	{
		glfwPollEvents();
		if (glfwWindowShouldClose(window))
			running = false;

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}