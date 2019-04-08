#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

void error_callback(int error, const char* description);
void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void close_window_callback(GLFWwindow* window)
{
    fprintf(stdout, "Windows is closed\n");
}

int main() {
		// Set an error callback
		glfwSetErrorCallback(error_callback);
		/* Initialize the library */
		if (glfwInit() == 0) {
			return -1;
		}

		/* Create a windowed mode window and its OpenGL context */
		GLFWwindow *window;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
		if (window == nullptr) {
			glfwTerminate();
			return -1;
		}
		glfwSwapInterval(1);
		glfwSetWindowCloseCallback(window, close_window_callback);

		/* Make the window's context current */
		glfwMakeContextCurrent(window);

		// Initialize GLEW
		if (glewInit() != GLEW_OK) {
			std::cerr << "GLEW is not ok" << '\n';
			glfwTerminate();
			return -1;
		}
	
	  GLuint* index_buffer;
		glGenBuffers(2, index_buffer);
		std::cout << index_buffer[1] << std::endl;
		
		/* Loop until the user closes the window */
		while (glfwWindowShouldClose(window) == 0) {
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		glfwTerminate();
		return 0;
}
