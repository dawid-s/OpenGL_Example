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
		std::cerr << "Couldn't opent the file: " << fullPath << "\n";
		return;
	}
}

void linkProgram(GLuint program)
{
	glLinkProgram(program);

	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::LINK_PROGRAM_FAILED: " << infoLog << "\n";
	}
}

unsigned int initGL(GLFWwindow** window)
{
	// Set an error callback
	glfwSetErrorCallback(error_callback);
	/* Initialize the library */
	if (glfwInit() == 0)
	{
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	*window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(*window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW is not ok" << '\n';
		glfwTerminate();
		return -1;
	}
	glfwSwapInterval(1);
	return 0;
}

void createShaderProgram(GLuint shaderProgram)
{
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexShader, "/../vertex_shader.vert");
	glAttachShader(shaderProgram, vertexShader);

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentShader, "/../fragment_shader.frag");
	glAttachShader(shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");
}

int main()
{
	GLFWwindow *window;
	unsigned int error = initGL(&window);
	if(error != 0)
	{
		return error; 
	}

	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // x, y, z, r, g, b
		 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f
	};

	GLuint elements[] = {
		0, 1, 2,
		0, 1, 3
	};

	GLuint vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	GLuint index_buffer;
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint element_buffer;
	glGenBuffers(1, &element_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	createShaderProgram(shaderProgram);
	linkProgram(shaderProgram);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

	GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
						  reinterpret_cast<void *>(3 * sizeof(GLfloat)));

	while ((error = glGetError()) != GL_NO_ERROR)
	{
		std::cerr << std::hex << error << "\n";
	}
	glEnableVertexAttribArray(posAttrib);
	glEnableVertexAttribArray(colorAttrib);

	// glDeleteShader(vertexShader);
	// glDeleteShader(fragmentShader);

	/* Loop until the user closes the window */
	while (glfwWindowShouldClose(window) == 0)
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(vertex_array);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
