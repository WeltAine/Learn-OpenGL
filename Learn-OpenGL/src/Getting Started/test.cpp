#include "Core.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>
//看来还有引入顺序的要求

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>


#include "Log.h"
#include "ShaderReader.h"
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/ext/vector_float3.hpp> // vec3
#include <glm/ext/matrix_float4x4.hpp> // mat4x4
#include <glm/ext/matrix_transform.hpp> // translate, rotate, scale, identity
#include <glm/gtc/type_ptr.hpp>





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
	

	// 这两者有什么区别
	//glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) //实际的绘制尺寸，这却决于DPI和分配的虚拟窗口尺寸
	//	{
	//		glViewport(0, 0, width, height);
	//	});
	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) // 操作系统上的虚拟窗口尺寸
		{
			glViewport(0, 0, width, height);
		});



	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");


	// Shader 
	// 顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	std::string sourceCode = LearnOpenGL::ShaderReader::Read("O:/CppProgram/Learn-OpenGL/Learn-OpenGL/Shader/vertexshader.txt");
	const char* vertexShaderSource = sourceCode.c_str();

	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);


	// 片元着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

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
		// 顶点					// 颜色				//UV
		-0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		 0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
	};
	unsigned int vertexs_VBO;
	glGenBuffers(1, &vertexs_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexs_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);
	// 先“生成”一个空缓冲区
	// 通过绑定（binding）操作将其设为 OpenGL 状态机中的当前缓冲区
	// 随后将顶点数据复制到该当前绑定的缓冲区中

	// 显存解读（解读哪一个VBO对象取决于，但是状态机绑定哪一个VBO对象，所以这是一个代码上下文敏感的步骤）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);//为显存中数据被vectorShader中的aPos使用做准备
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//为显存中数据被vectorShader中的_colour使用做准备
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// EBO 和Unity一样左手环绕序(顺时针环绕序)
	int indices[] = {
		0, 1, 2,
		0, 3, 1
	};
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	int xOffsetLocation = glGetUniformLocation(shaderProgram, "xOffset");
	float xOffset = 0.0f;


	// 纹理
	unsigned int texture = 0;
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("O:/CppProgram/Learn-OpenGL/Learn-OpenGL/src/Getting Started/SS.jpg", &width, &height, &nrChannels, 0);
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
	//glTexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
	glGenerateMipmap(GL_TEXTURE_2D);



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
		
		glUniform1f(xOffsetLocation, xOffset);


		//变换(glm的translate，rotate，scale是右乘)
		glm::mat4 transform{ 1.0f };
		transform = glm::translate(transform, glm::vec3(0.5f, 0.0f, 0.0f));
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3{0.0f, 0.0f, 1.0f});

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

		//glUseProgram(shaderProgram);
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		#pragma endregion

		#pragma region 交互
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("水平偏移");
			ImGui::SliderFloat("float", &xOffset, -0.5f, 0.5f);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		#pragma endregion


		#pragma region 轮询事件队列 & 交换缓冲
		glfwPollEvents();
		glfwSwapBuffers(window);
		#pragma endregion

	}

	glDeleteVertexArrays(1, &VAO_1);
	glDeleteBuffers(1, &vertexs_VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);


	glfwTerminate();
	return 0;

}




