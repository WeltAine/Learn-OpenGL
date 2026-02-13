#include "Core.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
//看来还有引入顺序的要求

#include "Log.h"


int main() {


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int success = glfwInit();
	LEARNOPENGL_ASSERT(success, "Could not initialize GLFW!");

	glfwSwapInterval(1);


	GLFWwindow* window = glfwCreateWindow(1280, 1024, "Hello World", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	int status = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
	LEARNOPENGL_ASSERT(status, "Failed to initialize GLAFD!");

	while (true) {

	}


}
