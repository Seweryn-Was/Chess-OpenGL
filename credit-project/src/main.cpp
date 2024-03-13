#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include "ShaderProgram.h"

GLFWwindow* window;

int main(void)
{
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cout<<(stderr, "Error: %s\n", glewGetErrorString(err));
    }

    ShaderProgarm standard;
    ShaderSource standardSource = { "./res/shaders/standard/standard.frag", "./res/shaders/standard/standard.vert"};
    standard.source = &standardSource;
    CreateShaderProgram(&standard);
    UseShaderProgram(&standard); 
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << err << " | \n";
    }


    float vert[] = {
        //pos         color           UV
        -1.0, -1.0, 0.0, 0.0, 1.0, 0.0,  -1.0, -1.0,
        1.0, -1.0, 0.0, 0.0, 1.0, 0.0,  1.0, -1.0,
        -1.0, 1.0, 0.0, 0.0, 1.0, 0.0,  -1.0, 1.0,
        1.0, 1.0, 0.0, 0.0, 1.0, 0.0,   1.0, 1.0,

    };
    unsigned int indices[] = {
        0, 1,2,
        1, 2, 3
    }; 

    unsigned int VAO, EBO, VBO; 
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)3);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)6);
    glEnableVertexAttribArray(2);

    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << err << " | \n";
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){

        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    DeleteShaderProgram(&standard);

    glfwTerminate();
    return 0;
}