#include "app.h"

bool initApp(App* app) {

    app->scaleUI = 0.24f;
    app->SCR_HEIGHT = 640;
    app->SCR_WIDTH = 1060;

    if (!glfwInit())
        return false;

    /* Create a windowed mode window and its OpenGL context */
    app->window = glfwCreateWindow(app->SCR_WIDTH, app->SCR_HEIGHT, "Hello World", NULL, NULL);
    if (!app->window)
    {
        std::cout << "window wasnot created\n"; 
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(app->window);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "GLEW_NOT_OK\n";
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cout << (stderr, "Error: %s\n", glewGetErrorString(err));
        return false;
    }


    SetAppData(app);
    return true;
}

void SetAppData(App* app)
{


    ShaderProgarm *standard = new ShaderProgram;
    ShaderSource standardSource = { "./res/shaders/standard/standard.frag", "./res/shaders/standard/standard.vert" };
    standard->source = &standardSource; 
    CreateShaderProgram(standard);
    app->data.standard = standard;

    ShaderProgarm *boardShader = new ShaderProgram;
    ShaderSource boardSource = { "./res/shaders/board/board.frag", "./res/shaders/standard/standard.vert" };
    boardShader->source = &boardSource;
    CreateShaderProgram(boardShader);
    std::cout<<" board id1: " << boardShader->programId << "\n";
    app->data.boardShader = boardShader;
    std::cout << " board id2: " << app->data.boardShader->programId << "\n";
   
}

bool runApp(App* app) {
    std::cout << "RUNING APP\n"; 
    while (!glfwWindowShouldClose(app->window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(app->window);
        glfwPollEvents();
    }
    return true;
}

bool terminateApp(App* app) {

    delete app->data.boardShader;
    delete app->data.standard; 
   
    glfwTerminate();
    return true;
}