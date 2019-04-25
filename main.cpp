#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

namespace fs = std::experimental::filesystem;

void error_callback(int error, const char *description);
void error_callback(int error, const char *description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

void close_window_callback(GLFWwindow *window)
{
	fprintf(stdout, "Windows is closed\n");
}

void compileShader(const GLuint shader, const std::string &shaderPath)
{
	const std::string fullPath = fs::current_path().string() + shaderPath;
	std::ifstream ifs(fullPath);
	if (ifs.is_open())
	{

		std::string shaderSource{std::istreambuf_iterator<char>(ifs),
								 std::istreambuf_iterator<char>()};
		const char *shaderSourceCStr = shaderSource.c_str();
		glShaderSource(shader, 1, &shaderSourceCStr, NULL);
		glCompileShader(shader);

		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED "
					  << infoLog << "\n";
			return;
		}
		std::cout << fullPath << " has been compiled successfully.\n";
	}
	else
	{
		std::cerr << "Couldn't opent the file\n";
		return;
	}
}

int main()
{
	// Set an error callback
	glfwSetErrorCallback(error_callback);
	/* Initialize the library */
	if (glfwInit() == 0)
	{
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow *window;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		return -1;
	}
	glfwSetWindowCloseCallback(window, close_window_callback);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW is not ok" << '\n';
		glfwTerminate();
		return -1;
	}
	// glfwSwapInterval(1);

	// float vertices[] = {
    // -0.5f, -0.5f, 0.0f,
    //  0.5f, -0.5f, 0.0f,
    //  0.0f,  0.5f, 0.0f
	// };
	float vertices[] = {
     0.0f,  0.5f, // Vertex 1 (X, Y)
     0.5f, -0.5f, // Vertex 2 (X, Y)
    -0.5f, -0.5f  // Vertex 3 (X, Y)
	};

	GLuint vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	GLuint index_buffer;
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float), vertices, GL_STATIC_DRAW);
	std::cout << sizeof(vertices) << "\n";

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexShader, "/../vertex_shader.glsl");
	glAttachShader(shaderProgram, vertexShader);

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentShader, "/../fragment_shader.glsl");
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");

	glLinkProgram(shaderProgram);

	// GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	// // glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	// glEnableVertexAttribArray(posAttrib);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::LINK_PROGRAM_FAILED: " << infoLog << "\n";
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/* Loop until the user closes the window */
	while (glfwWindowShouldClose(window) == 0)
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		// glBindVertexArray(vertex_array);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
