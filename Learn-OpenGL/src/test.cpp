#include "Core.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>
//看来还有引入顺序的要求

#include "Log.h"


namespace LearnOpenGL {

	void ProcessInput(GLFWwindow* window) {

		// 检测ESC键是否被按下
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}

	}
}



int main() {

	LearnOpenGL::Log::Init();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//glfwWindowHint 的第一个参数代表我们需要配置的选项，第二个参数则是用于设置该选项值的整数。
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int success = glfwInit();
	LEARNOPENGL_ASSERT(success, "Could not initialize GLFW!");

	glfwSwapInterval(1);


	GLFWwindow* window = glfwCreateWindow(1280, 720, "Learn-OpenGL", nullptr, nullptr);
	if (window == nullptr) {
		LOG_ERROR("Failed to create GLFW window");
		glfwTerminate();

		return -1;
	}

	glfwMakeContextCurrent(window);//将该窗口的上下文设置为当前线程的主上下文。

	int status = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
	LEARNOPENGL_ASSERT(status, "Failed to initialize GLAFD!");


	glViewport(0, 0, 1280, 720);
	

	//glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
	//	{
	//		glViewport(0, 0, width, height);
	//	});


	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		});





	// Shader
	// 顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const char* vertexShaderSource = R"(#version 330 core
layout (location = 0) in vec3 aPos;

void main(){
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
})";

	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);

	// 片元着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* fragmentShaderSource = R"(#version 330 core
out vec4 FragColor;

void main(){
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
})";

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);

	// 着色器编译
	int _success;
	char infoLog[512];

	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &_success);
	if (!_success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		LOG_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
	}

	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &_success);
	if (!_success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		LOG_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED");
	}
	

	// ShaderProgram
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// AVO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	float vertexs[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);


	// 显存解读
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//为显存中数据被vectorShader中的aPos使用做准备
	glEnableVertexAttribArray(0);


	// EBO 和Unity一样左手环绕序
	int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);


	while (!glfwWindowShouldClose(window)) {

		// 在每一帧开始时，我们都需要进行清屏操作，否则屏幕上会残留上一帧的渲染结果（除非这正是你想要的效果，但通常情况下并非如此）。
		// 我们可以调用 glClear 函数来清空屏幕的颜色缓冲，并通过传入缓冲位（Buffer Bit）来指定要清空的缓冲类型。
		// 可选的位包括 GL_COLOR_BUFFER_BIT、GL_DEPTH_BUFFER_BIT 和 GL_STENCIL_BUFFER_BIT。目前我们只关心颜色值，因此只需清空颜色缓冲即可。
		// 调用 glClearColor 来设置清屏所用的颜色。每当我们调用 glClear 来清除颜色缓冲时，整个颜色缓冲都会被填充为 glClearColor 所设置的颜色。
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		#pragma region 特定输入轮询
		LearnOpenGL::ProcessInput(window);
		#pragma endregion



		#pragma region 渲染指令
		//glUseProgram(shaderProgram);
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		#pragma endregion



		#pragma region 轮询事件队列 & 交换缓冲
		glfwPollEvents();
		glfwSwapBuffers(window);
		#pragma endregion

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);


	glfwTerminate();
	return 0;

}




