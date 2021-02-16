/*Controls
P,Q,E Change view mode for polygons
WASD Camera mouvement
Mouse-Camera Orientation
1-5 Select preset cameras and selected model
U,J Scale Up and down
I,K Rotate selected object
O,L Move selected object up and down
*/

#include <iostream>
#include <math.h>


#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler


#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace glm;


const char* getVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        ""
        "uniform mat4 worldMatrix;"
        "uniform mat4 orientationMatrix = mat4(1.0);"
        "uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
        "uniform mat4 projectionMatrix = mat4(1.0);"
        ""
        "out vec3 vertexColor;"
        "void main()"
        "{"
        "   vertexColor = aColor;"
        "   mat4 modelViewProjection = projectionMatrix * viewMatrix * orientationMatrix * worldMatrix;"
        "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "}";
}

const char* getFragmentShaderSource()
{
    return
        "#version 330 core\n"
        "in vec3 vertexColor;"
        "uniform vec3 objectColor;"
        ""
        "out vec4 FragColor;"
        ""
        "void main()"
        "{"
        "	vec4 baseColor= vec4(objectColor.r, objectColor.g, objectColor.b, 1.0f);"
        "   FragColor = baseColor;"
        "}";
}

int compileAndLinkShaders()
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = getVertexShaderSource();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = getFragmentShaderSource();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int createCubeVertexArrayObject()
{
    //A vertex is a point on a polygon, it contains positions and other data (eg: colors)
   // Cube model
    vec3 vertexArray[] = {
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), //left - red
        vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), // far - blue
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), // bottom - turquoise
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), // near - green
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), // right - purple
        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),

        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), // top - yellow
        vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f)
    };

    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    return vertexBufferObject;
}

//MODEL-VIEW METHODS FOR SHADER
void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix)
{
    glUseProgram(shaderProgram);
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void setViewMatrix(int shaderProgram, mat4 viewMatrix)
{
    glUseProgram(shaderProgram);
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

void setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
    glUseProgram(shaderProgram);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}

void setOrientationMatrix(int shaderProgram, mat4 orientationMatrix)
{
    glUseProgram(shaderProgram);
    GLuint orientationMatrixLocation = glGetUniformLocation(shaderProgram, "orientationMatrix");
    glUniformMatrix4fv(orientationMatrixLocation, 1, GL_FALSE, &orientationMatrix[0][0]);
}

int main(int argc, char* argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window and rendering context using GLFW, resolution is 1024x768
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Comp371 - Project", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Black background
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    // Disabling cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Compile and link shaders here ...
    int shaderProgram = compileAndLinkShaders();

    // SHADER PROGRAM LOCATIONS
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

    // INITIAL Camera parameters for view transform
    vec3 cameraPosition(0.0f, 5.0f, 20.0f);
    vec3 cameraLookAt(0.0f, 0.0f, 0.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);
    // Other camera parameters
    float cameraSpeed = 15.0f;
    float cameraFastSpeed = 7 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    float fov = 70.0f;

    // Set projection matrix for shader
    mat4 projectionMatrix = perspective(radians(fov),            // field of view in degrees
        800.0f / 600.0f,  // aspect ratio
        0.01f, 100.0f);   // near and far (near > 0)

    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,  // eye
        cameraLookAt,  // center
        cameraUp); // up

    setViewMatrix(shaderProgram, viewMatrix);
    setProjectionMatrix(shaderProgram, projectionMatrix);

    // Define and upload geometry to the GPU here ...
    int vbo = createCubeVertexArrayObject();

    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    int lastMouseRightState = GLFW_RELEASE;
    int lastMouseMiddleState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    // Enable Backface culling
    glEnable(GL_CULL_FACE);

    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);

    //END OF TEMPLATE HERE-------------------------------------------------------------------------------------------------------------------------------------------------------

    //Initializing a vallue for current position (1-5)Setting default to 1
    int position = 1;

    // Initialize Matrices
    mat4 model = mat4(1.0f); // identity matrix
    mat4 WorldMatrix; // Matrix used to create objects
    mat4 orientationMatrix = mat4(1.0f); // initialize orientation matrix

    //Predefined matrices for model manipulation
    mat4 pushToEdgeMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -64.0f)); // Matrix to bring objects to edge of circle
    mat4 pullFromEdgeMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 64.0f)); // Matrix to bring back objects from edge of circle

    //These matrices are used to create the offset between digits in a model
    mat4 firstDigitOffsetMatrix = pullFromEdgeMatrix * rotate(mat4(1.0f), radians(7.5f), vec3(0.0f, 1.0f, 0.0f)) * pushToEdgeMatrix;//First digit
    mat4 secondDigitOffsetMatrix = pullFromEdgeMatrix * rotate(mat4(1.0f), radians(2.5f), vec3(0.0f, 1.0f, 0.0f)) * pushToEdgeMatrix;//Second digit
    mat4 thirdDigitOffsetMatrix = pullFromEdgeMatrix * rotate(mat4(1.0f), radians(-2.5f), vec3(0.0f, 1.0f, 0.0f)) * pushToEdgeMatrix;//Third digit
    mat4 fourthDigitOffsetMatrix = pullFromEdgeMatrix * rotate(mat4(1.0f), radians(-7.5f), vec3(0.0f, 1.0f, 0.0f)) * pushToEdgeMatrix;//Second digit

    //Initialising all group matrices and variables
    mat4 GroupTMatrix = model; //To modify final model position
    mat4 GroupOriginRotationMatrix = model; //If we want to rotate whole group on itself at final position
    mat4 GroupScaleMatrix = model; //If we want to scale the whole group of 4 digits
    //Combination matrix to avoid unnecessary calculations and simplicity
    mat4 GroupMatrix = model;

    //Group 1--------------------------------------------------------------------------------------------------------------

    //Initializing group 1 matrices and variables 
    //These a group at the center (Camera 1)
    //These variables are used to modify the above matrices to change the digits according to user input
    float oneRotation = 0.0f;
    float oneScale = 1;
    float oneX = 0;
    float oneZ = 0;
    float oneY = 0;

    //TO DO - PUT THE MODELS FOR GROUP 1 HERE

    //Group 2 ---------------------------------------------------------------------------------------------------------------
    //Initializing Sam,s group matrices (Position 2)
    //Rotation Matrix to bring model to its position around the circle
    mat4 secondGroupCircleRotationMatrix = rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 1.0f, 0.0f));

    //These variables are used to modify the above matrices to change the digits according to user input
    float twoRotation = 0.0f;
    float twoScale = 1;
    float twoX = 0;
    float twoZ = 0;
    float twoY = 0;


    //Transformation matrices for number 4------------------------
    //Right side
    mat4 fourRightTMatrix = translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f));
    mat4 fourRightSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 fourRightMatrix = firstDigitOffsetMatrix * fourRightTMatrix * fourRightSMatrix;

    //Horizontal part
    mat4 fourHorizontalTMatrix = translate(mat4(1.0f), vec3(-2.0f, 2.5f, 0.0f));
    mat4 fourHorizontalSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 fourHorizontalMatrix = firstDigitOffsetMatrix * fourHorizontalTMatrix * fourHorizontalSMatrix;

    //Left part
    mat4 fourLeftTMatrix = translate(mat4(1.0f), vec3(-3.0f, 3.5f, 0.0f));
    mat4 fourLeftSMatrix = scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));
    mat4 fourLeftMatrix = firstDigitOffsetMatrix * fourLeftTMatrix * fourLeftSMatrix;



    //Transformation matrices for number 3--------------------------
    //Right side (3)
    mat4 threeRightTMatrix = translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f));
    mat4 threeRightSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 threeRightMatrix = secondDigitOffsetMatrix * threeRightTMatrix * threeRightSMatrix;
    //Top part (3)
    mat4 threeTopTMatrix = translate(mat4(1.0f), vec3(-1.0f, 4.5f, 0.0f));
    mat4 threeTopSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 threeTopMatrix = secondDigitOffsetMatrix * threeTopTMatrix * threeTopSMatrix;
    //Mid part (3)
    mat4 threeMidTMatrix = translate(mat4(1.0f), vec3(-1.0f, 2.5f, 0.0f));
    mat4 threeMidSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 threeMidMatrix = secondDigitOffsetMatrix * threeMidTMatrix * threeMidSMatrix;
    //Bottom part (3)
    mat4 threeBottomTMatrix = translate(mat4(1.0f), vec3(-1.0f, 0.5f, 0.0f));
    mat4 threeBottomSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 threeBottomMatrix = secondDigitOffsetMatrix * threeBottomTMatrix * threeBottomSMatrix;

    //Transformation matrices for letter S-----------------------------
    //Right side (s)
    mat4 sRightTMatrix = translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.0f));
    mat4 sRightSMatrix = scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));
    mat4 sRightMatrix = thirdDigitOffsetMatrix * sRightTMatrix * sRightSMatrix;
    //Top part (s)
    mat4 sTopTMatrix = translate(mat4(1.0f), vec3(-1.0f, 4.5f, 0.0f));
    mat4 sTopSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 sTopMatrix = thirdDigitOffsetMatrix * sTopTMatrix * sTopSMatrix;
    //Mid part (s)
    mat4 sMidTMatrix = translate(mat4(1.0f), vec3(-1.0f, 2.5f, 0.0f));
    mat4 sMidSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 sMidMatrix = thirdDigitOffsetMatrix * sMidTMatrix * sMidSMatrix;
    //Bottom part (s)
    mat4 sBottomTMatrix = translate(mat4(1.0f), vec3(-1.0f, 0.5f, 0.0f));
    mat4 sBottomSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 sBottomMatrix = thirdDigitOffsetMatrix * sBottomTMatrix * sBottomSMatrix;
    //Left part (s)
    mat4 sLeftTMatrix = translate(mat4(1.0f), vec3(-2.0f, 3.5f, 0.0f));
    mat4 sLeftSMatrix = scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));
    mat4 sLeftMatrix = thirdDigitOffsetMatrix * sLeftTMatrix * sLeftSMatrix;

    //Transformation matrices for letter L-------------------------------
    //Left side (s)
    mat4 lLeftTMatrix = translate(mat4(1.0f), vec3(-2.0f, 2.5f, 0.0f));
    mat4 lLeftSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 lLeftMatrix = fourthDigitOffsetMatrix * lLeftTMatrix * lLeftSMatrix;
    //Bottom part (s)
    mat4 lBottomTMatrix = translate(mat4(1.0f), vec3(-1.0f, 0.0f, 0.0f));
    mat4 lBottomSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 lBottomMatrix = fourthDigitOffsetMatrix * lBottomTMatrix * lBottomSMatrix;

    //Group 3 --------------------------------------------------------------------------------------------
    //Initializing third group matrices (Position 3)
    //These are the behind right group, aka position 3 with camera

    //Rotation Matrix to bring model to its position around the circle
    mat4 thirdGroupCircleRotationMatrix = rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 1.0f, 0.0f));

    //These variables are used to modify the above matrices to change the digits according to user input
    float threeRotation = 0.0f;
    float threeScale = 1;
    float threeX = 0;
    float threeZ = 0;
    float threeY = 0;

    //TO DO -- Models for group 3 here

    //Group 4 ----------------------------------------------------------------------------------------------
    //Initializing fourth group matrices (Position 4)
    //These are the front right group, aka position 4 with camera

    //Rotation Matrix to bring model to its position around the circle
    mat4 fourthGroupCircleRotationMatrix = rotate(mat4(1.0f), radians(-135.0f), vec3(0.0f, 1.0f, 0.0f));


    //These variables are used to modify the above matrices to change the digits according to user input
    float fourRotation = 0.0f;
    float fourScale = 1;
    float fourX = 0;
    float fourZ = 0;
    float fourY = 0;

    //TO DO -- Models for group 4 here

    //Group 5 ----------------------------------------------------------------------------------------------
    //Initializing fifth group matrices (Position 5)
    //These are the front left group, aka position 5 with camera

    //Rotation Matrix to bring model to its position around the circle
    mat4 fifthGroupCircleRotationMatrix = rotate(mat4(1.0f), radians(135.0f), vec3(0.0f, 1.0f, 0.0f));


    //These variables are used to modify the above matrices to change the digits according to user input
    float fiveRotation = 0.0f;
    float fiveScale = 1;
    float fiveX = 0;
    float fiveZ = 0;
    float fiveY = 0;

    //TO DO -- Models for group 5 here



    //More definition (
    vec2 currentOrientation(0.0f, 0.0f); // current orientation of matrix


    //Main loop ------------------------------------------------------------------------------------------------------------------------
    // Entering Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // Each frame, reset color of each pixel to glClearColor
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        glUseProgram(shaderProgram);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);


        // Drawing coordinates -------------------------------------------------
        // X-axis
        WorldMatrix = translate(model, vec3(3.5f, 0.0f, 0.0f)) * scale(model, vec3(7.0f, 0.1f, 0.1f));
        setWorldMatrix(shaderProgram, WorldMatrix);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.0, 0.0)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Y-axis
        WorldMatrix = translate(model, vec3(0.0f, 3.5f, 0.0f)) * scale(model, vec3(0.1f, 7.0f, 0.1f));
        setWorldMatrix(shaderProgram, WorldMatrix);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0, 1.0, 0.0)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Z-axis
        WorldMatrix = translate(model, vec3(0.0f, 0.0f, 3.5f)) * scale(model, vec3(0.1f, 0.1f, 7.0f));
        setWorldMatrix(shaderProgram, WorldMatrix);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0, 0.0, 1.0)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw Grid 128x128 --------------------------------------------------------
        int gridsize = 128;
        for (int i = -gridsize / 2; i < gridsize / 2; ++i)
        {
            WorldMatrix = translate(model, vec3(i, 0.0f, 0.0f)) * scale(model, vec3(0.02f, 0.02f, gridsize));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            WorldMatrix = translate(model, vec3(0.0f, 0.0f, i)) * scale(model, vec3(gridsize, 0.02f, 0.02f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //Draw reference circle (Can be removed before handing in)
        for (int i = 0; i < 360; ++i)
        {
            WorldMatrix = rotate(mat4(1.0f), radians((float)i), vec3(0.0f, 1.0f, 0.0f)) * translate(mat4(1.0f), vec3(64.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
            glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }


        //Section for first group (position 1,center)---------------------------------------------------------------------------------------
        //Samuel Tardif ID : 40051573
        //Checking if our group needs to be modified
        GroupScaleMatrix = scale(mat4(1.0f), vec3(oneScale, oneScale, oneScale));
        GroupOriginRotationMatrix = rotate(mat4(1.0f), radians(oneRotation), vec3(0.0f, 1.0f, 0.0f));
        GroupTMatrix = translate(mat4(1.0f), vec3(oneX, oneY, oneZ));
        //Making our combined group matrix
        GroupMatrix = GroupTMatrix * GroupOriginRotationMatrix * GroupScaleMatrix;

        //Drawing our digits
        //TO DO INSERT DRAWING OF GROUP 1
        //Section for digit 4 (part of group 2)--------------------
        //Right part of 4 
        WorldMatrix = GroupMatrix * fourRightMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.5, 0.5, 0.5)));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Horizontal part of 4
        WorldMatrix = GroupMatrix * fourHorizontalMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Left part of 4
        WorldMatrix = GroupMatrix * fourLeftMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Section for digit 3 (part of group 2)----------------------
        //Right part of three
        WorldMatrix = GroupMatrix * threeRightMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Top part of 3
        WorldMatrix = GroupMatrix * threeTopMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Mid part of 3
        WorldMatrix = GroupMatrix * threeMidMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Bottom part of 3
        WorldMatrix = GroupMatrix * threeBottomMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //Section for letter S (part of group 2)-------------------------
        //Right part of S
        WorldMatrix = GroupMatrix * sRightMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Top part of S
        WorldMatrix = GroupMatrix * sTopMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Mid part of S
        WorldMatrix = GroupMatrix * sMidMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Bottom part of S
        WorldMatrix = GroupMatrix * sBottomMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Left part of S
        WorldMatrix = GroupMatrix * sLeftMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Section for letter L (part of group 2)----------------------------
        //Left part of L
        WorldMatrix = GroupMatrix * lLeftMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.5, 0.5, 0.5)));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Bottom part of L
        WorldMatrix = GroupMatrix * lBottomMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //Section for second group (position 2, behind left)---------------------------------------------------------------------------------------
        //Samuel Tardif ID : 40051573
        //Checking if our group needs to be modified
        GroupScaleMatrix = scale(mat4(1.0f), vec3(twoScale, twoScale, twoScale));
        GroupOriginRotationMatrix = rotate(mat4(1.0f), radians(twoRotation), vec3(0.0f, 1.0f, 0.0f));
        GroupTMatrix = translate(mat4(1.0f), vec3(twoX, twoY, twoZ));
        //Making our combined group matrix
        GroupMatrix = GroupTMatrix * secondGroupCircleRotationMatrix * pushToEdgeMatrix * GroupOriginRotationMatrix * GroupScaleMatrix;

        //Drawing our digits

        //Section for digit 4 (part of group 2)--------------------
        //Right part of 4 
        WorldMatrix = GroupMatrix * fourRightMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.5, 0.5, 0.5)));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Horizontal part of 4
        WorldMatrix = GroupMatrix * fourHorizontalMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Left part of 4
        WorldMatrix = GroupMatrix * fourLeftMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Section for digit 3 (part of group 2)----------------------
        //Right part of three
        WorldMatrix = GroupMatrix * threeRightMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Top part of 3
        WorldMatrix = GroupMatrix * threeTopMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Mid part of 3
        WorldMatrix = GroupMatrix * threeMidMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Bottom part of 3
        WorldMatrix = GroupMatrix * threeBottomMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //Section for letter S (part of group 2)-------------------------
        //Right part of S
        WorldMatrix = GroupMatrix * sRightMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Top part of S
        WorldMatrix = GroupMatrix * sTopMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Mid part of S
        WorldMatrix = GroupMatrix * sMidMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Bottom part of S
        WorldMatrix = GroupMatrix * sBottomMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Left part of S
        WorldMatrix = GroupMatrix * sLeftMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Section for letter L (part of group 2)----------------------------
        //Left part of L
        WorldMatrix = GroupMatrix * lLeftMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.5, 0.5, 0.5)));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Bottom part of L
        WorldMatrix = GroupMatrix * lBottomMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //Section for third group (position 3, behind right)---------------------------------------------------------------------------------------
        //
        //Checking if our group needs to be modified
        GroupScaleMatrix = scale(mat4(1.0f), vec3(threeScale, threeScale, threeScale));
        GroupOriginRotationMatrix = rotate(mat4(1.0f), radians(threeRotation), vec3(0.0f, 1.0f, 0.0f));
        GroupTMatrix = translate(mat4(1.0f), vec3(threeX, threeY, threeZ));
        //Making our combined group matrix
        GroupMatrix = GroupTMatrix * thirdGroupCircleRotationMatrix * pushToEdgeMatrix * GroupOriginRotationMatrix * GroupScaleMatrix;

        //Drawing our digits
        //TO DO PUT GROUP 3 DIGITS HERE

        //Section for fourth group (position 4, front right)---------------------------------------------------------------------------------------
        //
        //Checking if our group needs to be modified
        GroupScaleMatrix = scale(mat4(1.0f), vec3(fourScale, fourScale, fourScale));
        GroupOriginRotationMatrix = rotate(mat4(1.0f), radians(fourRotation), vec3(0.0f, 1.0f, 0.0f));
        GroupTMatrix = translate(mat4(1.0f), vec3(fourX, fourY, fourZ));
        //Making our combined group matrix
        GroupMatrix = GroupTMatrix * fourthGroupCircleRotationMatrix * pushToEdgeMatrix * GroupOriginRotationMatrix * GroupScaleMatrix;

        //Drawing our digits
        //TO DO PUT DIGITS GROUP 4 HERE

        //Section for fifth group (position 4, front left)---------------------------------------------------------------------------------------
        //
        //Checking if our group needs to be modified
        GroupScaleMatrix = scale(mat4(1.0f), vec3(fiveScale, fiveScale, fiveScale));
        GroupOriginRotationMatrix = rotate(mat4(1.0f), radians(fiveRotation), vec3(0.0f, 1.0f, 0.0f));
        GroupTMatrix = translate(mat4(1.0f), vec3(fiveX, fiveY, fiveZ));
        //Making our combined group matrix
        GroupMatrix = GroupTMatrix * fifthGroupCircleRotationMatrix * pushToEdgeMatrix * GroupOriginRotationMatrix * GroupScaleMatrix;

        //Drawing our digits


        /*
        // Draw letters and numbers

        //Group matrix for hierachical modeling
        mat4 groupMatrix = mat4(1.0f);
        //vertical part of the "L" letter
        mat4 LPartMatrix = translate(mat4(1.0f), vec3(-15.0f, 5.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 14.0f, 2.0f));
        mat4 LWorldMatrix = ScaleMatrix * groupMatrix * LPartMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &LWorldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.0, 0.0)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //base of the "L" letter
        LPartMatrix = translate(mat4(1.0f), vec3(-12.0f, -1.0f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(2.0f, 7.0f, 2.0f));
        LWorldMatrix = ScaleMatrix * groupMatrix * LPartMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &LWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //vertical part of the "T" letter
        mat4 TPartMatrix = translate(mat4(1.0f), vec3(15.0f, 5.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 12.0f, 2.0f));
        mat4 TWorldMatrix = ScaleMatrix * groupMatrix * TPartMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &TWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //horizontal part of the "T" letter
        TPartMatrix = translate(mat4(1.0f), vec3(15.0f, 12.0f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(2.0f, 15.0f, 2.0f));
        TWorldMatrix = ScaleMatrix * groupMatrix * TPartMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &TWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Draw number 2
        //Redefine the group matrix
        groupMatrix = rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
        //first vertical of 2
        mat4 FivePartMatrix = translate(mat4(1.0f), vec3(30.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 4.0f, 2.0f));
        mat4 FiveWorldMatrix = ScaleMatrix * groupMatrix * FivePartMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &FiveWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //bottom horizontal line of 2
        FivePartMatrix = translate(mat4(1.0f), vec3(35.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(2.0f, 8.0f, 2.0f));
        FiveWorldMatrix = ScaleMatrix * groupMatrix * FivePartMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &FiveWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //second vertical of 2
        FivePartMatrix = translate(mat4(1.0f), vec3(38.0f, 4.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 8.0f, 2.0f));
        FiveWorldMatrix = ScaleMatrix * groupMatrix * FivePartMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &FiveWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //second horizontal line of 2
        FivePartMatrix = translate(mat4(1.0f), vec3(35.0f, 8.0f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(2.0f, 8.0f, 2.0f));
        FiveWorldMatrix = ScaleMatrix * groupMatrix * FivePartMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &FiveWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //third vertical line of 2
        FivePartMatrix = translate(mat4(1.0f), vec3(30.0f, 10.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 6.0f, 2.0f));
        FiveWorldMatrix = ScaleMatrix * groupMatrix * FivePartMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &FiveWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //last horizontal line of 2
        FivePartMatrix = translate(mat4(1.0f), vec3(35.0f, 12.0f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(2.0f, 8.0f, 2.0f));
        FiveWorldMatrix = ScaleMatrix * groupMatrix * FivePartMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &FiveWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //This part is to draw number 5
        groupMatrix = mat4(1.0f);

        //first vertical of 5
        FivePartMatrix = translate(mat4(1.0f), vec3(30.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 4.0f, 2.0f));
        FiveWorldMatrix = ScaleMatrix * groupMatrix * FivePartMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &FiveWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //bottom horizontal line of 5
        FivePartMatrix = translate(mat4(1.0f), vec3(35.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(2.0f, 8.0f, 2.0f));
        FiveWorldMatrix = ScaleMatrix * groupMatrix * FivePartMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &FiveWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //second vertical of 5

        FivePartMatrix = translate(mat4(1.0f), vec3(38.0f, 4.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 8.0f, 2.0f));
        FiveWorldMatrix = ScaleMatrix * groupMatrix * FivePartMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &FiveWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //second horizontal line of 5

        FivePartMatrix = translate(mat4(1.0f), vec3(35.0f, 8.0f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(2.0f, 8.0f, 2.0f));
        FiveWorldMatrix = ScaleMatrix * groupMatrix * FivePartMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &FiveWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //third vertical line of 5
        FivePartMatrix = translate(mat4(1.0f), vec3(30.0f, 10.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 6.0f, 2.0f));
        FiveWorldMatrix = ScaleMatrix * groupMatrix * FivePartMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &FiveWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //last horizontal line of 5
        FivePartMatrix = translate(mat4(1.0f), vec3(35.0f, 12.0f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(2.0f, 8.0f, 2.0f));
        FiveWorldMatrix = ScaleMatrix * groupMatrix * FivePartMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &FiveWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        */


        // Orientation Matrix
        orientationMatrix = rotate(rotate(mat4(1.0f), currentOrientation.x, vec3(1.0f, 0.0f, 0.0f)), currentOrientation.y, vec3(0.0f, 1.0f, 0.0f));
        setOrientationMatrix(shaderProgram, orientationMatrix);

        // Camera tilt, pan and zoom
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);

        double dx = 0;
        double dy = 0;

        //SECTION FOR CONTROLS-----------------------------------------------------------------------------------------------------------------------------
        // Mouse Controls
        if (lastMouseRightState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            dx = mousePosX - lastMousePosX;
        }
        if (lastMouseMiddleState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
            dy = mousePosY - lastMousePosY;
        }
        if (lastMouseLeftState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            fov += mousePosX - lastMousePosX;
            if (fov <= 0) {
                fov = 0.5f;
            }
            if (fov >= 180) {
                fov = 180.0f;
            }
        }

        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        // Convert to spherical coordinates
        const float cameraAngularSpeed = 30.0f;
        cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

        // Clamp vertical angle to [-85, 85] degrees
        cameraVerticalAngle = max(-85.0f, min(85.0f, cameraVerticalAngle));
        if (cameraHorizontalAngle > 360)
        {
            cameraHorizontalAngle -= 360;
        }
        else if (cameraHorizontalAngle < -360)
        {
            cameraHorizontalAngle += 360;
        }

        float theta = radians(cameraHorizontalAngle);
        float phi = radians(cameraVerticalAngle);

        cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
        vec3 cameraSideVector = cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

        normalize(cameraSideVector);

        // WASD movement TO BE CHANGED------------------------------------------------------
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
        {
            cameraPosition -= cameraSideVector * dt * cameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move camera to the right
        {
            cameraPosition += cameraSideVector * dt * cameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move camera up
        {
            cameraPosition -= cameraLookAt * dt * cameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move camera down
        {
            cameraPosition += cameraLookAt * dt * cameraSpeed;
        }

        // Orientation controls--------------------------------------------------
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            currentOrientation.x += radians(5.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            currentOrientation.x -= radians(5.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            currentOrientation.y += radians(5.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            currentOrientation.y -= radians(5.0f);
        }
        // reset orientation
        if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
            currentOrientation.y = 0;
            currentOrientation.x = 0;
        }

        //Preset Camera locations----------------------------------------------------------------------------
        //Preset for center
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {

            //Snapping camera to center
            cameraPosition = glm::vec3(0.0f, 5.0f, 20.0f);
            cameraLookAt = glm::vec3(0.0f, 0.0f, -1.0f);

            glm::mat4 viewMatrix = glm::lookAt(cameraPosition,  // eye
                cameraLookAt,  // center
                glm::vec3(0.0f, 1.0f, 0.0f));// up

            GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
            glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

            //Setting current position to 1
            position = 1;
        }


        //Preset for behind left
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {

            cameraPosition = glm::vec3(-40 * sin(3.14159 / 4), 5.0f, -40 * cos(3.14159 / 4));
            cameraLookAt = glm::vec3(-1.0f, 0.0f, -1.0f);

            glm::mat4 viewMatrix = glm::lookAt(cameraPosition,  // eye
                cameraLookAt,  // center
                glm::vec3(0.0f, 1.0f, 0.0f));// up

            GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
            glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

            //Setting current position to 2
            position = 2;
        }

        //Preset for behind right
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {

            cameraPosition = glm::vec3(40 * sin(3.14159 / 4), 5.0f, -40 * cos(3.14159 / 4));
            cameraLookAt = glm::vec3(-1.0f, 0.0f, -1.0f);

            glm::mat4 viewMatrix = glm::lookAt(cameraPosition,  // eye
                cameraLookAt,  // center
                glm::vec3(0.0f, 1.0f, 0.0f));// up

            GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
            glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

            //Setting current position to 3
            position = 3;
        }

        //Preset for front right
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {

            cameraPosition = glm::vec3(40 * sin(3.14159 / 4), 5.0f, 40 * cos(3.14159 / 4));
            cameraLookAt = glm::vec3(-1.0f, 0.0f, -1.0f);

            glm::mat4 viewMatrix = glm::lookAt(cameraPosition,  // eye
                cameraLookAt,  // center
                glm::vec3(0.0f, 1.0f, 0.0f));// up

            GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
            glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

            //Setting current position to 4
            position = 4;
        }

        //Preset for front left
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {

            cameraPosition = glm::vec3(-40 * sin(3.14159 / 4), 5.0f, 40 * cos(3.14159 / 4));
            cameraLookAt = glm::vec3(-1.0f, 0.0f, -1.0f);

            glm::mat4 viewMatrix = glm::lookAt(cameraPosition,  // eye
                cameraLookAt,  // center
                glm::vec3(0.0f, 1.0f, 0.0f));// up

            GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
            glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

            //Setting current position to 5
            position = 5;
        }


        //Controls for modifying digits--------------------------------------------------------------------
        // Scale controls
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) // Scale up
        {
            if (position == 1) {
                oneScale += 0.1f;
            }
            if (position == 2) {
                twoScale += 0.1f;
            }
            if (position == 3) {
                threeScale += 0.1f;
            }
            if (position == 4) {
                fourScale += 0.1f;
            }
            if (position == 5) {
                fiveScale += 0.1f;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) // Scale down
        {
            if (position == 1) {
                if (oneScale <= 0)
                    oneScale = 0;
                else {
                    oneScale -= 0.1f;
                }
            }
            if (position == 2) {
                if (twoScale <= 0)
                    twoScale = 0;
                else {
                    twoScale -= 0.1f;
                }
            }
            if (position == 3) {
                if (threeScale <= 0)
                    threeScale = 0;
                else {
                    threeScale -= 0.1f;
                }
            }
            if (position == 4) {
                if (fourScale <= 0)
                    fourScale = 0;
                else {
                    fourScale -= 0.1f;
                }
            }
            if (position == 5) {
                if (fiveScale <= 0)
                    fiveScale = 0;
                else {
                    fiveScale -= 0.1f;
                }
            }
        }
        //Rotation control
        //Rotation with I and K
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) // Rotate counter-clockwise
        {
            if (position == 1) {
                oneRotation += 1.0f;
            }
            if (position == 2) {
                twoRotation += 1.0f;
            }
            if (position == 3) {
                threeRotation += 1.0f;
            }
            if (position == 4) {
                fourRotation += 1.0f;
            }
            if (position == 5) {
                fiveRotation += 1.0f;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) // Rotate clockwise
        {
            if (position == 1) {
                oneRotation -= 1.0f;
            }
            if (position == 2) {
                twoRotation -= 1.0f;
            }
            if (position == 3) {
                threeRotation -= 1.0f;
            }
            if (position == 4) {
                fourRotation -= 1.0f;
            }
            if (position == 5) {
                fiveRotation -= 1.0f;
            }
        }
        //Translation control
        //Using (T,F,G,H) Cause the whole keyboard is taken
        //Using O,L to move the objects up and down on the Y axis
        //It is relative to the starting position's perspective
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) //T = negative Z axis (or back for default view)
        {
            if (position == 1) {
                oneZ -= 0.1f;
            }
            if (position == 2) {
                twoZ -= 0.1f;
            }
            if (position == 3) {
                threeZ -= 0.1f;
            }
            if (position == 4) {
                fourZ -= 0.1f;
            }
            if (position == 5) {
                fiveZ -= 0.1f;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) // G - Positive Z axis
        {
            if (position == 1) {
                oneZ += 0.1f;
            }
            if (position == 2) {
                twoZ += 0.1f;
            }
            if (position == 3) {
                threeZ += 0.1f;
            }
            if (position == 4) {
                fourZ += 0.1f;
            }
            if (position == 5) {
                fiveZ += 0.1f;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) //F = negative X axis (or back for default view)
        {
            if (position == 1) {
                oneX -= 0.1f;
            }
            if (position == 2) {
                twoX -= 0.1f;
            }
            if (position == 3) {
                threeX -= 0.1f;
            }
            if (position == 4) {
                fourX -= 0.1f;
            }
            if (position == 5) {
                fiveX -= 0.1f;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) // G - Positive X axis
        {
            if (position == 1) {
                oneX += 0.1f;
            }
            if (position == 2) {
                twoX += 0.1f;
            }
            if (position == 3) {
                threeX += 0.1f;
            }
            if (position == 4) {
                fourX += 0.1f;
            }
            if (position == 5) {
                fiveX += 0.1f;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) //O = positive Y axis
        {
            if (position == 1) {
                oneY += 0.1f;
            }
            if (position == 2) {
                twoY += 0.1f;
            }
            if (position == 3) {
                threeY += 0.1f;
            }
            if (position == 4) {
                fourY += 0.1f;
            }
            if (position == 5) {
                fiveY += 0.1f;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // L -Negative Y axis
        {
            if (position == 1) {
                oneY -= 0.1f;
            }
            if (position == 2) {
                twoY -= 0.1f;
            }
            if (position == 3) {
                threeY -= 0.1f;
            }
            if (position == 4) {
                fourY -= 0.1f;
            }
            if (position == 5) {
                fiveY -= 0.1f;
            }
        }



        // Render types
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }


        // Projection Matrix
        projectionMatrix = perspective(radians(fov),            // field of view in degrees
            800.0f / 600.0f,  // aspect ratio
            0.01f, 100.0f);   // near and far (near > 0)
        setProjectionMatrix(shaderProgram, projectionMatrix);

        // View Matrix
        viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
        setViewMatrix(shaderProgram, viewMatrix);

        // End frame
        glfwSwapBuffers(window);

        // Detect inputs
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    // Shutdown GLFW
    glfwTerminate();

    return 0;
}