#include "Core.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>
//看来还有引入顺序的要求

#include "Log.h"
#include "ShaderReader.h"


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

	//const char* vertexShaderSource = LearnOpenGL::ShaderReader::Read("O:/CppProgram/Learn-OpenGL/Learn-OpenGL/Shader/vertexshader.txt").c_str();//函数返回的对象在此上下文中会变成一个将亡值
	std::string sourceCode = LearnOpenGL::ShaderReader::Read("O:/CppProgram/Learn-OpenGL/Learn-OpenGL/Shader/vertexshader.txt");
	const char* vertexShaderSource = sourceCode.c_str();



	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);

	// 片元着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//const char* fragmentShaderSource = LearnOpenGL::ShaderReader::Read("O:/CppProgram/Learn-OpenGL/Learn-OpenGL/Shader/fragmentshader.txt").c_str();//函数返回的对象在此上下文中会变成一个将亡值
	sourceCode = LearnOpenGL::ShaderReader::Read("O:/CppProgram/Learn-OpenGL/Learn-OpenGL/Shader/fragmentshader.txt");
	const char* fragmentShaderSource = sourceCode.c_str();

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
	unsigned int VAO_1;
	glGenVertexArrays(1, &VAO_1);
	glBindVertexArray(VAO_1);

	// VBO
	float vertexs[] = {
		0.5f,  0.5f, 0.0f,   // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f,   // top left 

		0.5f,  0.5f, 0.0f,   // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f,  0.5f, 0.0f,   // top left 
	};
	unsigned int points_VBO;
	glGenBuffers(1, &points_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);
	// 先“生成”一个空缓冲区
	// 通过绑定（binding）操作将其设为 OpenGL 状态机中的当前缓冲区
	// 随后将顶点数据复制到该当前绑定的缓冲区中

	// 显存解读
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//为显存中数据被vectorShader中的aPos使用做准备
	glEnableVertexAttribArray(0);



	float colors[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};
	unsigned int colors_VBO;
	glGenBuffers(1, &colors_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//为显存中数据被vectorShader中的_colour使用做准备
	glEnableVertexAttribArray(1);




	// EBO 和Unity一样左手环绕序(顺时针环绕序)
	int indices[] = {
		//0, 1, 3,   // first triangle
		1, 2, 3,    // second triangle
		4, 5, 6
	};
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


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
		glBindVertexArray(VAO_1);

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

	glDeleteVertexArrays(1, &VAO_1);
	glDeleteBuffers(1, &points_VBO);
	glDeleteBuffers(1, &colors_VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);


	glfwTerminate();
	return 0;

}




