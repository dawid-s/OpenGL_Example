#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

class A {

};

int main() {
        GLFWwindow *window;

        /* Initialize the library */
        if (glfwInit() == 0) {
                return -1;
        }
        glewInit();

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
        if (window == nullptr) {
                glfwTerminate();
                return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        if (glewInit() != GLEW_OK) {
                std::cout << "Goew is not ok" << '\n';
        }

        /* Loop until the user closes the window */
        while (glfwWindowShouldClose(window) == 0) {
                /* Render here */
                glClear(GL_COLOR_BUFFER_BIT);

                glBegin(GL_TRIANGLES);
                glColor3f(0.1f, 0.2f, 0.3f);
                glVertex3f(0, 0, 0);
                glVertex3f(1, 0, 0);
                glVertex3f(0, 1, 0);
                glEnd();

                /* Swap front and back buffers */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                glfwPollEvents();
        }

        glfwTerminate();
        return 0;
}
