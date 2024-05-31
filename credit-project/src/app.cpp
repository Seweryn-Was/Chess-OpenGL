#include "app.h"

bool initApp(App* app) {

    app->scaleUI = 0.22f;
    app->data.scaleUI = 0.22f; 
    app->SCR_HEIGHT = 960;
    app->SCR_WIDTH = 960;

    if (!glfwInit())
        return false;

    /* Create a windowed mode window and its OpenGL context */
    app->window = glfwCreateWindow(app->SCR_WIDTH, app->SCR_HEIGHT, "Chess Game", NULL, NULL);
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(app->window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();


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
    app->data.boardShader = boardShader;

    app->data.mouseBtnLeftPressClicked = false;
    app->data.isMate = false; 
    app->data.canMakeMove = true; 

    const int sizeOfTriangles = ONE_COLOR_SIZE * NUM_OF_TRIS_IN_SQUARE;

    app->data.WhitePieces = new ChessPiece[1][ONE_COLOR_SIZE];//&WhitePieces; 
    app->data.whitePiecesIndicies = new uivec3[1][ONE_COLOR_SIZE * NUM_OF_TRIS_IN_SQUARE];//&whitePiecesIndicies;

    app->data.blackPieces = new ChessPiece[1][ONE_COLOR_SIZE];//&WhitePieces; 
    app->data.blackPiecesIndicies = new uivec3[1][ONE_COLOR_SIZE * NUM_OF_TRIS_IN_SQUARE];//&whitePiecesIndicies;

    for (int i = 0; i < sizeOfTriangles; i += NUM_OF_TRIS_IN_SQUARE) {
        int offset = i / 2 * STANDARD_SQUARE_VERTICIES_SIZE;
        (*app->data.whitePiecesIndicies)[i].x = 0 + offset;
        (*app->data.whitePiecesIndicies)[i].y = 1 + offset;
        (*app->data.whitePiecesIndicies)[i].z = 2 + offset;
        (*app->data.whitePiecesIndicies)[i + 1].x = 2 + offset;
        (*app->data.whitePiecesIndicies)[i + 1].y = 3 + offset;
        (*app->data.whitePiecesIndicies)[i + 1].z = 0 + offset;

        (*app->data.blackPiecesIndicies)[i].x = 0 + offset;
        (*app->data.blackPiecesIndicies)[i].y = 1 + offset;
        (*app->data.blackPiecesIndicies)[i].z = 2 + offset;
        (*app->data.blackPiecesIndicies)[i + 1].x = 2 + offset;
        (*app->data.blackPiecesIndicies)[i + 1].y = 3 + offset;
        (*app->data.blackPiecesIndicies)[i + 1].z = 0 + offset;
    }
    for (int i = 0; i < ONE_COLOR_SIZE; i++) {
        (*app->data.WhitePieces)[i] = createChessPiece({ positionsWhite[i].x - 4 + 0.5f, positionsWhite[i].y - 4 + 0.5f }, texturesIndexes[i], WHITE_CHESS_INDEX, app->scaleUI);
        (*app->data.blackPieces)[i] = createChessPiece({ positionsBlack[i].x - 4 + 0.5f, positionsBlack[i].y - 4 + 0.5f }, texturesIndexes[i], BLACK_CHESS_INDEX, app->scaleUI);
    }

    static Buffers whitePiecesBuffers = {};
    app->data.whitePiecesBuffers = &whitePiecesBuffers;
    glGenVertexArrays(1, &whitePiecesBuffers.VAO);
    glBindVertexArray(whitePiecesBuffers.VAO);

    glGenBuffers(1, &app->data.whitePiecesBuffers->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, app->data.whitePiecesBuffers->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*app->data.WhitePieces), app->data.WhitePieces, GL_STATIC_DRAW);

    glGenBuffers(1, &app->data.whitePiecesBuffers->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->data.whitePiecesBuffers->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*app->data.whitePiecesIndicies), app->data.whitePiecesIndicies, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(LAYOUT_LOCATION_POS, 3, GL_FLOAT, GL_FALSE, sizeof(vertexChessPiece), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_POS);
    glVertexAttribPointer(LAYOUT_LOCATION_UV, 2, GL_FLOAT, GL_FALSE, sizeof(vertexChessPiece), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_UV);

    static Buffers blackPiecesBuffers = {};
    app->data.blackPiecesBuffers = &blackPiecesBuffers;
    glGenVertexArrays(1, &blackPiecesBuffers.VAO);
    glBindVertexArray(blackPiecesBuffers.VAO);

    glGenBuffers(1, &app->data.blackPiecesBuffers->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, app->data.blackPiecesBuffers->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*app->data.blackPieces), app->data.blackPieces, GL_STATIC_DRAW);

    glGenBuffers(1, &app->data.blackPiecesBuffers->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->data.blackPiecesBuffers->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*app->data.blackPiecesIndicies), app->data.blackPiecesIndicies, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(LAYOUT_LOCATION_POS, 3, GL_FLOAT, GL_FALSE, sizeof(vertexChessPiece), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_POS);
    glVertexAttribPointer(LAYOUT_LOCATION_UV, 2, GL_FLOAT, GL_FALSE, sizeof(vertexChessPiece), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_UV);

    createChessAtlasTexture(&app->data.chessPiecesAtlasTexture, "./res/textures/chess.png");

    static Buffers boardBuffers = {};
    app->data.boardBuffers = &boardBuffers;
    static chessBoardSquare boardLocalVert[64] = {};
    app->data.boardLocalVert = &boardLocalVert;
    uivec3 boardTriangleIndicies[64 * 2];
    app->data.boardTriangleIndicies = &boardTriangleIndicies;

    CreateBoardBuffer(&boardLocalVert[0], &boardTriangleIndicies[0], app->scaleUI);

    glGenVertexArrays(1, &app->data.boardBuffers->VAO);
    glBindVertexArray(app->data.boardBuffers->VAO);

    glGenBuffers(1, &app->data.boardBuffers->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, app->data.boardBuffers->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*app->data.boardLocalVert), app->data.boardLocalVert, GL_STATIC_DRAW);

    glGenBuffers(1, &app->data.boardBuffers->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->data.boardBuffers->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*app->data.boardTriangleIndicies), app->data.boardTriangleIndicies, GL_STATIC_DRAW);

    glVertexAttribPointer(LAYOUT_LOCATION_POS, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_POS);
    glVertexAttribPointer(LAYOUT_LOCATION_COLOR, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(LAYOUT_LOCATION_COLOR);

    int size = sizeof(glm::mat4);
    glGenBuffers(1, &app->data.ProjectionUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, app->data.ProjectionUBO);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, app->data.ProjectionUBO); //accesbiel at binding 0 
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

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

    DeleteShaderProgram(app->data.standard);
    DeleteShaderProgram(app->data.boardShader);
    glDeleteVertexArrays(1, &app->data.whitePiecesBuffers->VAO);
    glDeleteBuffers(1, &app->data.whitePiecesBuffers->VBO);
    glDeleteBuffers(1, &app->data.whitePiecesBuffers->EBO);

    glDeleteVertexArrays(1, &app->data.boardBuffers->VAO);
    glDeleteBuffers(1, &app->data.boardBuffers->VBO);
    glDeleteBuffers(1, &app->data.boardBuffers->EBO);

    glDeleteBuffers(1, &app->data.ProjectionUBO);

    delete[] app->data.WhitePieces;
    delete[] app->data.whitePiecesIndicies;
    delete[] app->data.blackPiecesIndicies;
    delete[] app->data.blackPieces;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return true;
}