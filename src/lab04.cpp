//
// COMP 371 Labs Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//

#include <iostream>
#include <list>
#include <algorithm>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace glm;
using namespace std;



GLuint loadTexture(const char* filename);

const char* getVertexShaderSource();

const char* getFragmentShaderSource();

const char* getTexturedVertexShaderSource();

const char* getTexturedFragmentShaderSource();

int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource);

struct TexturedColoredVertex
{
    TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv, vec3 _normal)
        : position(_position), color(_color), uv(_uv), normal(_normal)  {}

    vec3 position;
    vec3 color;
    vec2 uv;
    vec3 normal;

};

// Textured Cube model
const TexturedColoredVertex texturedCubeVertexArray[] = {                              
                       // position,                color                      UV                      normal
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)),      //left - red
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),

    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f)),       // far - blue
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f)),
    TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),

    TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f,-1.0f, 0.0f)),        // bottom - turquoise
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f,-1.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f), vec3(0.0f,-1.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f,-1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f), vec3(0.0f,-1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f,-1.0f, 0.0f)),

    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f),  vec3(0.0f, 0.0f, 1.0f)),    // near - green
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
    TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f)),
    TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f)),      // right - purple
    TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f)),    // top - yellow
    TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f))
};

int createTexturedCubeVertexArrayObject();

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

    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow* window = glfwCreateWindow(800, 600, "Comp371 - Lab 04", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);


    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }


    // Load Textures
    GLuint brickTextureID = loadTexture("assets/textures/brick.jpg");
    GLuint cementTextureID = loadTexture("assets/textures/cement.jpg");
    GLuint tilesTextureID = loadTexture("assets/textures/tiles.jpg");
    GLuint steelTextureID = loadTexture("assets/textures/steel.jpg");
    GLuint cardboardTextureID = loadTexture("assets/textures/cardboard2.jpg");
    GLuint fabricTextureID = loadTexture("assets/textures/fabric2.jpg");

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Compile and link shaders here ...
    int colorShaderProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());
    int texturedShaderProgram = compileAndLinkShaders(getTexturedVertexShaderSource(), getTexturedFragmentShaderSource());

    // SHADER PROGRAM LOCATIONS
    GLuint worldMatrixLocation = glGetUniformLocation(colorShaderProgram, "worldMatrix");
    GLuint viewMatrixLocation = glGetUniformLocation(colorShaderProgram, "viewMatrix");
    GLuint projectionMatrixLocation = glGetUniformLocation(colorShaderProgram, "projectionMatrix");
    GLuint colorLocation = glGetUniformLocation(colorShaderProgram, "vertexColor");

    // Camera parameters for view transform
    vec3 cameraPosition(0.6f, 1.0f, 10.0f);
    vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);

    // Other camera parameters
    float cameraSpeed = 5.0f;
    float cameraFastSpeed = 2 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    bool  cameraFirstPerson = true; // press 1 or 2 to toggle this variable
    float fov = 70.0f;

    // Spinning cube at camera position
    float spinningCubeAngle = 0.0f;

    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
        800.0f / 600.0f,  // aspect ratio
        0.01f, 100.0f);   // near and far (near > 0)

    //LIGHTS
    vec3 lightPos0(0.0f, 30.0f, 0.0f);   
   glUniform3fv(glGetUniformLocation(texturedShaderProgram, "lightPos0"), 1, value_ptr(lightPos0));

// Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,  // eye
        cameraPosition + cameraLookAt,  // center
        cameraUp); // up

// Set View and Projection matrices on both shaders
    setViewMatrix(colorShaderProgram, viewMatrix);
    setViewMatrix(texturedShaderProgram, viewMatrix);

    setProjectionMatrix(colorShaderProgram, projectionMatrix);
    setProjectionMatrix(texturedShaderProgram, projectionMatrix);


    // Define and upload geometry to the GPU here ...
    int texturedCubeVAO = createTexturedCubeVertexArrayObject();

    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    // Other OpenGL states to set once
    // Enable Backface culling
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);


    glBindVertexArray(texturedCubeVAO);


    //Setting up our components
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
    mat4 firstDigitOffsetMatrix = translate(mat4(1.0f), vec3(-7.5f, 0.0f, 0.0f));
    mat4 secondDigitOffsetMatrix = translate(mat4(1.0f), vec3(-2.5f, 0.0f, 0.0f));
    mat4 thirdDigitOffsetMatrix = translate(mat4(1.0f), vec3(2.5f, 0.0f, 0.0f));
    mat4 fourthDigitOffsetMatrix = translate(mat4(1.0f), vec3(7.5f, 0.0f, 0.0f));

    //Initialising all group matrices and variables
    mat4 GroupTMatrix = model; //To modify final model position
    mat4 GroupOriginRotationMatrix = model; //If we want to rotate whole group on itself at final position
    mat4 GroupScaleMatrix = model; //If we want to scale the whole group of 4 digits
    //Combination matrix to avoid unnecessary calculations and simplicity
    mat4 GroupMatrix = model;

    //Stage matrices
    //Main cube
    mat4 mainStageTMatrix = translate(mat4(1.0f), vec3(0.0f, 1.0f, -50.0f));
    mat4 mainStageSMatrix = scale(mat4(1.0f), vec3(50.0f, 2.0f, 10.0f));
    mat4 mainStageMatrix = mainStageTMatrix * mainStageSMatrix;

    //Right cube
    mat4 rightStageTMatrix = translate(mat4(1.0f), vec3(20.0f, 1.0f, -40.0f));
    mat4 rightStageSMatrix = scale(mat4(1.0f), vec3(10.0f, 2.0f, 10.0f));
    mat4 rightStageMatrix = rightStageTMatrix * rightStageSMatrix;

    //Left cube
    mat4 leftStageTMatrix = translate(mat4(1.0f), vec3(-20.0f, 1.0f, -40.0f));
    mat4 leftStageSMatrix = scale(mat4(1.0f), vec3(10.0f, 2.0f, 10.0f));
    mat4 leftStageMatrix = leftStageTMatrix * leftStageSMatrix;

    //The screen
    mat4 screenTMatrix = translate(mat4(1.0f), vec3(0.0f, 8.0f, -54.0f));
    mat4 screenSMatrix = scale(mat4(1.0f), vec3(20.0f, 10.0f, 0.05f));
    mat4 screenMatrix = screenTMatrix * screenSMatrix;

    //Left pillar top
    mat4 topLeftPillarTMatrix = translate(mat4(1.0f), vec3(-10.0f, 13.5f, -54.0f));
    mat4 topLeftPillarSMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    mat4 topLeftPillarMatrix = topLeftPillarTMatrix * topLeftPillarSMatrix;

    //Left pillar bottom
    mat4 bottomLeftPillarTMatrix = translate(mat4(1.0f), vec3(-10.0f, 2.5f, -54.0f));
    mat4 bottomLeftPillarSMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    mat4 bottomLeftPillarMatrix = bottomLeftPillarTMatrix * bottomLeftPillarSMatrix;

    //Main left pillar
    mat4 mainLeftPillarTMatrix = translate(mat4(1.0f), vec3(-10.0f, 8.0f, -54.0f));
    mat4 mainLeftPillarSMatrix = scale(mat4(1.0f), vec3(0.5f, 10.0f, 0.5f));
    mat4 mainLeftPillarMatrix = mainLeftPillarTMatrix * mainLeftPillarSMatrix;

    //Right pillar top
    mat4 topRightPillarTMatrix = translate(mat4(1.0f), vec3(10.0f, 13.5f, -54.0f));
    mat4 topRightPillarSMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    mat4 topRightPillarMatrix = topRightPillarTMatrix * topRightPillarSMatrix;

    //Right pillar bottom
    mat4 bottomRightPillarTMatrix = translate(mat4(1.0f), vec3(10.0f, 2.5f, -54.0f));
    mat4 bottomRightPillarSMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    mat4 bottomRightPillarMatrix = bottomRightPillarTMatrix * bottomRightPillarSMatrix;

    //Main right pillar
    mat4 mainRightPillarTMatrix = translate(mat4(1.0f), vec3(10.0f, 8.0f, -54.0f));
    mat4 mainRightPillarSMatrix = scale(mat4(1.0f), vec3(0.5f, 10.0f, 0.5f));
    mat4 mainRightPillarMatrix = mainRightPillarTMatrix * mainRightPillarSMatrix;

    //Group 1--------------------------------------------------------------------------------------------------------------

    //Initializing group 1 matrices and variables 
    //These a group at the center (Camera 1)
    //These variables are used to modify the above matrices to change the digits according to user input
    float oneRotation = 0.0f;
    float oneScale = 1;
    float oneX = 0;
    float oneZ = 0;
    float oneY = 0;

    //Group 1 ---------------------------------------------------------------------------------------------------------------
    //Initializing Sam's group matrices (Position 1)

    mat4 onePostitionMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -10.0f));

    //Transformation matrices for number 4------------------------
    //Right side
    mat4 fourRightTMatrix = translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f));
    mat4 fourRightSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 fourRightMatrix = thirdDigitOffsetMatrix * fourRightTMatrix * fourRightSMatrix;

    //Horizontal part
    mat4 fourHorizontalTMatrix = translate(mat4(1.0f), vec3(-2.0f, 2.5f, 0.0f));
    mat4 fourHorizontalSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 fourHorizontalMatrix = thirdDigitOffsetMatrix * fourHorizontalTMatrix * fourHorizontalSMatrix;

    //Left part
    mat4 fourLeftTMatrix = translate(mat4(1.0f), vec3(-3.0f, 3.5f, 0.0f));
    mat4 fourLeftSMatrix = scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));
    mat4 fourLeftMatrix = thirdDigitOffsetMatrix * fourLeftTMatrix * fourLeftSMatrix;



    //Transformation matrices for number 3--------------------------
    //Right side (3)
    mat4 threeRightTMatrix = translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f));
    mat4 threeRightSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 threeRightMatrix = fourthDigitOffsetMatrix * threeRightTMatrix * threeRightSMatrix;
    //Top part (3)
    mat4 threeTopTMatrix = translate(mat4(1.0f), vec3(-1.0f, 4.5f, 0.0f));
    mat4 threeTopSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 threeTopMatrix = fourthDigitOffsetMatrix * threeTopTMatrix * threeTopSMatrix;
    //Mid part (3)
    mat4 threeMidTMatrix = translate(mat4(1.0f), vec3(-1.0f, 2.5f, 0.0f));
    mat4 threeMidSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 threeMidMatrix = fourthDigitOffsetMatrix * threeMidTMatrix * threeMidSMatrix;
    //Bottom part (3)
    mat4 threeBottomTMatrix = translate(mat4(1.0f), vec3(-1.0f, 0.5f, 0.0f));
    mat4 threeBottomSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 threeBottomMatrix = fourthDigitOffsetMatrix * threeBottomTMatrix * threeBottomSMatrix;

    //Transformation matrices for letter S-----------------------------
    //Right side (s)
    mat4 sRightTMatrix = translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.0f));
    mat4 sRightSMatrix = scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));
    mat4 sRightMatrix = firstDigitOffsetMatrix * sRightTMatrix * sRightSMatrix;
    //Top part (s)
    mat4 sTopTMatrix = translate(mat4(1.0f), vec3(-1.0f, 4.5f, 0.0f));
    mat4 sTopSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 sTopMatrix = firstDigitOffsetMatrix * sTopTMatrix * sTopSMatrix;
    //Mid part (s)
    mat4 sMidTMatrix = translate(mat4(1.0f), vec3(-1.0f, 2.5f, 0.0f));
    mat4 sMidSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 sMidMatrix = firstDigitOffsetMatrix * sMidTMatrix * sMidSMatrix;
    //Bottom part (s)
    mat4 sBottomTMatrix = translate(mat4(1.0f), vec3(-1.0f, 0.5f, 0.0f));
    mat4 sBottomSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 sBottomMatrix = firstDigitOffsetMatrix * sBottomTMatrix * sBottomSMatrix;
    //Left part (s)
    mat4 sLeftTMatrix = translate(mat4(1.0f), vec3(-2.0f, 3.5f, 0.0f));
    mat4 sLeftSMatrix = scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));
    mat4 sLeftMatrix = firstDigitOffsetMatrix * sLeftTMatrix * sLeftSMatrix;

    //Transformation matrices for letter L-------------------------------
    //Left side (s)
    mat4 lLeftTMatrix = translate(mat4(1.0f), vec3(-2.0f, 2.5f, 0.0f));
    mat4 lLeftSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 lLeftMatrix = secondDigitOffsetMatrix * lLeftTMatrix * lLeftSMatrix;
    //Bottom part (s)
    mat4 lBottomTMatrix = translate(mat4(1.0f), vec3(-1.0f, 0.0f, 0.0f));
    mat4 lBottomSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 lBottomMatrix = secondDigitOffsetMatrix * lBottomTMatrix * lBottomSMatrix;
    //END OF GROUP 1 -------------------------------------------------------------------------------------

    //GROUP TWO ------------------------------------------------------------------------------------------
        //Rotation Matrix to bring model to its position around the circle
    //mat4 secondGroupCircleRotationMatrix = rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 1.0f, 0.0f));

    //Matrices to place group around the stage
    mat4 twoPostitionMatrix = translate(mat4(1.0f), vec3(-35.0f, 0.0f, -40.0f));
    mat4 twoBaseRotationMatrix = rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 1.0f, 0.0f));

    //These variables are used to modify the above matrices to change the digits according to user input
    float twoRotation = 0.0f;
    float twoScale = 1;
    float twoX = 0;
    float twoZ = 0;
    float twoY = 0;

    //draw "F"
    //vertical part of the "F" letter
    mat4 fVerticalTMatrix = translate(mat4(1.0f), vec3(-2.0f, 2.5f, 0.0f));
    mat4 fVerticalSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 fVerticalMatrix = firstDigitOffsetMatrix * fVerticalTMatrix * fVerticalSMatrix;
    //horizontal part1(upper) of the "F" letter
    mat4 fTopTMatrix = translate(mat4(1.0f), vec3(-1.0f, 4.5f, 0.0f));
    mat4 fTopSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 fTopMatrix = firstDigitOffsetMatrix * fTopTMatrix * fTopSMatrix;
    //horizontal part2(middle) of the "F" letter
    mat4 fMiddleTMatrix = translate(mat4(1.0f), vec3(-1.0f, 2.25f, 0.0f));
    mat4 fMiddleSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 fMiddleMatrix = firstDigitOffsetMatrix * fMiddleTMatrix * fMiddleSMatrix;

    //draw "E"
    //vertical part of the "E" letter
    mat4 eVerticalTMatrix = translate(mat4(1.0f), vec3(-2.0f, 2.5f, 0.0f));
    mat4 eVerticalSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 eVerticalMatrix = secondDigitOffsetMatrix * eVerticalTMatrix * eVerticalSMatrix;
    //horizontal part1(upper) of the "E" letter
    mat4 eTopTMatrix = translate(mat4(1.0f), vec3(-1.0f, 4.5f, 0.0f));
    mat4 eTopSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 eTopMatrix = secondDigitOffsetMatrix * eTopTMatrix * eTopSMatrix;
    //horizontal part2(middle) of the "E" letter
    mat4 eMiddleTMatrix = translate(mat4(1.0f), vec3(-1.0f, 2.25f, 0.0f));
    mat4 eMiddleSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 eMiddleMatrix = secondDigitOffsetMatrix * eMiddleTMatrix * eMiddleSMatrix;
    //horizontal part3(lower) of the "E" letter
    mat4 eBottomTMatrix = translate(mat4(1.0f), vec3(-1.0f, 0.0f, 0.0f));
    mat4 eBottomSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 eBottomMatrix = secondDigitOffsetMatrix * eBottomTMatrix * eBottomSMatrix;

    //section for the first number 4
    //Right side
    mat4 four1RightTMatrix = translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f));
    mat4 four1RightSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 four1RightMatrix = thirdDigitOffsetMatrix * four1RightTMatrix * four1RightSMatrix;

    //Horizontal parts
    mat4 four1HorizontalTMatrix = translate(mat4(1.0f), vec3(-2.0f, 2.5f, 0.0f));
    mat4 four1HorizontalSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 four1HorizontalMatrix = thirdDigitOffsetMatrix * four1HorizontalTMatrix * four1HorizontalSMatrix;

    //Left part
    mat4 four1LeftTMatrix = translate(mat4(1.0f), vec3(-3.0f, 3.5f, 0.0f));
    mat4 four1LeftSMatrix = scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));
    mat4 four1LeftMatrix = thirdDigitOffsetMatrix * four1LeftTMatrix * four1LeftSMatrix;

    //section for the second number 4
    //Right side
    mat4 four2RightTMatrix = translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f));
    mat4 four2RightSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 four2RightMatrix = fourthDigitOffsetMatrix * four2RightTMatrix * four2RightSMatrix;

    //Horizontal part
    mat4 four2HorizontalTMatrix = translate(mat4(1.0f), vec3(-2.0f, 2.5f, 0.0f));
    mat4 four2HorizontalSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 four2HorizontalMatrix = fourthDigitOffsetMatrix * four2HorizontalTMatrix * four2HorizontalSMatrix;

    //Left part
    mat4 four2LeftTMatrix = translate(mat4(1.0f), vec3(-3.0f, 3.5f, 0.0f));
    mat4 four2LeftSMatrix = scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));
    mat4 four2LeftMatrix = fourthDigitOffsetMatrix * four2LeftTMatrix * four2LeftSMatrix;
    //END OF GROUP TWO -----------------------------------------------------------------------------------

    //Group 3 --------------------------------------------------------------------------------------------
    //Initializing third group matrices (Position 3)
    //These are the behind right group, aka position 3 with camera

    //Rotation Matrix to bring model to its position around the circle
    //mat4 thirdGroupCircleRotationMatrix = rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 1.0f, 0.0f));

    //Matrices to place group around the stage
    mat4 threePostitionMatrix = translate(mat4(1.0f), vec3(-35.0f, 0.0f, -20.0f));
    mat4 threeBaseRotationMatrix = rotate(mat4(1.0f), radians(135.0f), vec3(0.0f, 1.0f, 0.0f));

    //These variables are used to modify the above matrices to change the digits according to user input
    float threeRotation = 0.0f;
    float threeScale = 1;
    float threeX = 0;
    float threeZ = 0;
    float threeY = 0;

    //Transformation matrices for number 5-----------------------------
    //Right side (s)
    mat4 FiveRightTMatrix = translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.0f));
    mat4 FiveRightSMatrix = scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));
    mat4 FiveRightMatrix = fourthDigitOffsetMatrix * FiveRightTMatrix * FiveRightSMatrix;
    //Top part (s)
    mat4 FiveTopTMatrix = translate(mat4(1.0f), vec3(-1.0f, 4.5f, 0.0f));
    mat4 FiveTopSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 FiveTopMatrix = fourthDigitOffsetMatrix * FiveTopTMatrix * FiveTopSMatrix;
    //Mid part (s)
    mat4 FiveMidTMatrix = translate(mat4(1.0f), vec3(-1.0f, 2.5f, 0.0f));
    mat4 FiveMidSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 FiveMidMatrix = fourthDigitOffsetMatrix * FiveMidTMatrix * FiveMidSMatrix;
    //Bottom part (s)
    mat4 FiveBottomTMatrix = translate(mat4(1.0f), vec3(-1.0f, 0.5f, 0.0f));
    mat4 FiveBottomSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 FiveBottomMatrix = fourthDigitOffsetMatrix * FiveBottomTMatrix * FiveBottomSMatrix;
    //Left part (s)
    mat4 FiveLeftTMatrix = translate(mat4(1.0f), vec3(-2.0f, 3.5f, 0.0f));
    mat4 FiveLeftSMatrix = scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));
    mat4 FiveLeftMatrix = fourthDigitOffsetMatrix * FiveLeftTMatrix * FiveLeftSMatrix;

    //Transformation matrices for letter L-------------------------------
    //Left side (s)
    mat4 LamLLeftTMatrix = translate(mat4(1.0f), vec3(-2.0f, 2.5f, 0.0f));
    mat4 LamLLeftSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 LamLLeftMatrix = firstDigitOffsetMatrix * LamLLeftTMatrix * LamLLeftSMatrix;
    //Bottom part (s)
    mat4 LamLBottomTMatrix = translate(mat4(1.0f), vec3(-1.0f, 0.0f, 0.0f));
    mat4 LamLBottomSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 LamLBottomMatrix = firstDigitOffsetMatrix * LamLBottomTMatrix * LamLBottomSMatrix;


    //ShearingMatrices
    mat4 shearingMatrixA = { 1,0,0,0,
                           0.3,1,0,0,
                             0,0,1,0,
                             0,0,0,1 };

    mat4 shearingMatrixB = { 1,0,0,0,
                          -0.3,1,0,0,
                             0,0,1,0,
                             0,0,0,1 };
    //Left part of M
    mat4 MLeftTMatrix = translate(mat4(1.0f), vec3(-1.0f, 2.0f, 0.0f));
    mat4 MLeftSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 MLeftMatrix = secondDigitOffsetMatrix * MLeftTMatrix * shearingMatrixA * MLeftSMatrix;

    //middle left part of M
    mat4 MMiddleLeftTMatrix = translate(mat4(1.0f), vec3(0.5f, 2.0f, 0.0f));
    mat4 MMiddleLeftSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 MMiddleLeftMatrix = secondDigitOffsetMatrix * MMiddleLeftTMatrix * shearingMatrixB * MMiddleLeftSMatrix;

    //middle right part of M
    mat4 MMiddleRightTMatrix = translate(mat4(1.0f), vec3(2.0f, 2.0f, 0.0f));
    mat4 MMiddleRightSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 MMiddleRightMatrix = secondDigitOffsetMatrix * MMiddleRightTMatrix * shearingMatrixA * MMiddleRightSMatrix;

    //right part of M
    mat4 MRightTMatrix = translate(mat4(1.0f), vec3(3.5f, 2.0f, 0.0f));
    mat4 MRightSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 MRightMatrix = secondDigitOffsetMatrix * MRightTMatrix * shearingMatrixB * MRightSMatrix;


    //Group 4 ----------------------------------------------------------------------------------------------
    //Initializing fourth group matrices (Position 4)
    //These are the front right group, aka position 4 with camera

    //Rotation Matrix to bring model to its position around the circle
    //mat4 fourthGroupCircleRotationMatrix = rotate(mat4(1.0f), radians(-135.0f), vec3(0.0f, 1.0f, 0.0f));

    //Matrices to place group around the stage
    mat4 fourPostitionMatrix = translate(mat4(1.0f), vec3(35.0f, 0.0f, -20.0f));
    mat4 fourBaseRotationMatrix = rotate(mat4(1.0f), radians(225.0f), vec3(0.0f, 1.0f, 0.0f));


    //These variables are used to modify the above matrices to change the digits according to user input
    float fourRotation = 0.0f;
    float fourScale = 1;
    float fourX = 0;
    float fourZ = 0;
    float fourY = 0;

    //MODELS FOR GROUP FOUR
    // 2 model (group 4)
    //bottom part of 2
    mat4 bottom2TMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
    mat4 bottom2SMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 bottom2Matrix = thirdDigitOffsetMatrix * bottom2TMatrix * bottom2SMatrix;

    //middle part of 2
    mat4 middle2TMatrix = translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f));
    mat4 middle2SMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 middle2Matrix = thirdDigitOffsetMatrix * middle2TMatrix * middle2SMatrix;

    //top part of 2
    mat4 top2TMatrix = translate(mat4(1.0f), vec3(0.0f, 4.0f, 0.0f));
    mat4 top2SMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 top2Matrix = thirdDigitOffsetMatrix * top2TMatrix * top2SMatrix;

    //left part of 2
    mat4 left2TMatrix = translate(mat4(1.0f), vec3(-1.0f, 1.5f, 0.0f));
    mat4 left2SMatrix = scale(mat4(1.0f), vec3(1.0f, 2.0f, 1.0f));
    mat4 left2Matrix = thirdDigitOffsetMatrix * left2TMatrix * left2SMatrix;

    //right part of 2
    mat4 right2TMatrix = translate(mat4(1.0f), vec3(1.0f, 3.5f, 0.0f));
    mat4 right2SMatrix = scale(mat4(1.0f), vec3(1.0f, 2.0f, 1.0f));
    mat4 right2Matrix = thirdDigitOffsetMatrix * right2TMatrix * right2SMatrix;

    // 9 model (group 4)
    //middle part of 9
    mat4 middle9TMatrix = translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f));
    mat4 middle9SMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 middle9Matrix = fourthDigitOffsetMatrix * middle9TMatrix * middle9SMatrix;

    //top part of 9
    mat4 top9TMatrix = translate(mat4(1.0f), vec3(0.0f, 4.0f, 0.0f));
    mat4 top9SMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 top9Matrix = fourthDigitOffsetMatrix * top9TMatrix * top9SMatrix;

    //left part of 9
    mat4 left9TMatrix = translate(mat4(1.0f), vec3(-1.0f, 3.5f, 0.0f));
    mat4 left9SMatrix = scale(mat4(1.0f), vec3(1.0f, 2.0f, 1.0f));
    mat4 left9Matrix = fourthDigitOffsetMatrix * left9TMatrix * left9SMatrix;

    //right part of 9
    mat4 right9TMatrix = translate(mat4(1.0f), vec3(1.0f, 2.0f, 0.0f));
    mat4 right9SMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 right9Matrix = fourthDigitOffsetMatrix * right9TMatrix * right9SMatrix;

    //A model (group 4)
    //Left part of A
    mat4 ALeftTMatrix = translate(mat4(1.0f), vec3(-0.9f, 2.0f, 0.0f));
    mat4 ALeftSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 ALeftMatrix = firstDigitOffsetMatrix * ALeftTMatrix * shearingMatrixA * ALeftSMatrix;

    //Right part of A
    mat4 ARightTMatrix = translate(mat4(1.0f), vec3(0.9f, 2.0f, 0.0f));
    mat4 ARightSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 ARightMatrix = firstDigitOffsetMatrix * ARightTMatrix * shearingMatrixB * ARightSMatrix;

    //Middle part of A
    mat4 AMiddleTMatrix = translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f));
    mat4 AMiddleSMatrix = scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
    mat4 AMiddleMatrix = firstDigitOffsetMatrix * AMiddleTMatrix * AMiddleSMatrix;

    //M model shared with group 3, including digit offset matrix

    //Group 5 ----------------------------------------------------------------------------------------------
    //Initializing fifth group matrices (Position 5)
    //aka position 5 with camera

    //Rotation Matrix to bring model to its position around the circle
    //mat4 fifthGroupCircleRotationMatrix = rotate(mat4(1.0f), radians(135.0f), vec3(0.0f, 1.0f, 0.0f));

    //Matrices to place group around the stage
    mat4 fivePostitionMatrix = translate(mat4(1.0f), vec3(35.0f, 0.0f, -40.0f));
    mat4 fiveBaseRotationMatrix = rotate(mat4(1.0f), radians(315.0f), vec3(0.0f, 1.0f, 0.0f));


    //These variables are used to modify the above matrices to change the digits according to user input
    float fiveRotation = 0.0f;
    float fiveScale = 1;
    float fiveX = 0;
    float fiveZ = 0;
    float fiveY = 0;

    //TO DO -- Models for group 5 here
    //Matrices for letter D
    //The left stick of D
    mat4 DLeftTMatrix = translate(mat4(1.0f), vec3(-1.0f, 2.5f, 0.0f));
    mat4 DLeftSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 DLeftMatrix = firstDigitOffsetMatrix * DLeftTMatrix * DLeftSMatrix;

    //The top stick of D
    mat4 DTopTMatrix = translate(mat4(1.0f), vec3(0.0f, 5.0f, 0.0f));
    mat4 DTopSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 DTopMatrix = firstDigitOffsetMatrix * DTopTMatrix * DTopSMatrix;

    //The bottom stick of D
    mat4 DBottomTMatrix = translate(mat4(1.0f), vec3(0.0f, 0.5f, 0.0f));
    mat4 DBottomSMatrix = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
    mat4 DBottomMatrix = firstDigitOffsetMatrix * DBottomTMatrix * DBottomSMatrix;

    //The right stick of D
    mat4 DRightTMatrix = translate(mat4(1.0f), vec3(2.0f, 2.75f, 0.0f));
    mat4 DRightSMatrix = scale(mat4(1.0f), vec3(1.0f, 4.0f, 1.0f));
    mat4 DRightMatrix = firstDigitOffsetMatrix * DRightTMatrix * DRightSMatrix;

    //Matrices for K
    //The left stick of K
    mat4 KLeftTMatrix = translate(mat4(1.0f), vec3(-1.0f, 2.5f, 0.0f));
    mat4 KLeftSMatrix = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
    mat4 KLeftMatrix = secondDigitOffsetMatrix * DLeftTMatrix * DLeftSMatrix;

    //Bottom slash of K
    mat4 KBottomTMatrix = translate(mat4(1.0f), vec3(0.5f, 1.25f, 0.0f));
    mat4 KBottomSMatrix = scale(mat4(1.0f), vec3(1.0f, 2.5f, 1.0f));
    mat4 KBottomMatrix = secondDigitOffsetMatrix * KBottomTMatrix * shearingMatrixB * KBottomSMatrix;

    //Top slash of K
    mat4 KTopTMatrix = translate(mat4(1.0f), vec3(0.5f, 3.75f, 0.0f));
    mat4 KTopSMatrix = scale(mat4(1.0f), vec3(1.0f, 2.5f, 1.0f));
    mat4 KTopMatrix = secondDigitOffsetMatrix * KTopTMatrix * shearingMatrixA * KTopSMatrix;

    //Number 4 matrices already defined

    //Matrices for number 2
     // 2 model (group 4)
    //bottom part of 2
    mat4 GroupFivebottom2Matrix = fourthDigitOffsetMatrix * bottom2TMatrix * bottom2SMatrix;

    //middle part of 2
    mat4 GroupFivemiddle2Matrix = fourthDigitOffsetMatrix * middle2TMatrix * middle2SMatrix;

    //top part of 2
    mat4 GroupFivetop2Matrix = fourthDigitOffsetMatrix * top2TMatrix * top2SMatrix;

    //left part of 2
    mat4 GroupFiveleft2Matrix = fourthDigitOffsetMatrix * left2TMatrix * left2SMatrix;

    //right part of 2
    mat4 GroupFiveright2Matrix = fourthDigitOffsetMatrix * right2TMatrix * right2SMatrix;


    //More definition (
    vec2 currentOrientation(0.0f, 0.0f); // current orientation of matrix
    float timeOnTexture = 0;
    int currentTexture = 1;

    // Entering Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // Each frame, reset color of each pixel to glClearColor
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw textured geometry
        glUseProgram(texturedShaderProgram);

        glActiveTexture(GL_TEXTURE0);
        GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
        glUniform1i(textureLocation, 0);                // Set our Texture sampler to user Texture Unit 0

        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        //For the swap of texture on screen
        timeOnTexture += dt;
        if (timeOnTexture > 10) {
            timeOnTexture = 0;
            currentTexture += 1;
            if (currentTexture == 6) {
                currentTexture = 1;
            }
        }

        switch (currentTexture) {
        case 1:
            glBindTexture(GL_TEXTURE_2D, tilesTextureID);
            break;
        case 2:
            glBindTexture(GL_TEXTURE_2D, cardboardTextureID);
            break;
        case 3:
            glBindTexture(GL_TEXTURE_2D, fabricTextureID);
            break;
        case 4:
            glBindTexture(GL_TEXTURE_2D, steelTextureID);
            break;
        case 5:
            glBindTexture(GL_TEXTURE_2D, cementTextureID);
            break;
        }



        //TO DO -- Save for last
        //Draw screen
        setWorldMatrix(texturedShaderProgram, screenMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // Draw ground
        glBindTexture(GL_TEXTURE_2D, tilesTextureID);
        mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(32.0f, -0.01f, 32.0f)) * scale(mat4(1.0f), vec3(64.0f, 0.02f, 64.0f));
        setWorldMatrix(texturedShaderProgram, groundWorldMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

        groundWorldMatrix = translate(mat4(1.0f), vec3(32.0f, -0.01f, -32.0f)) * scale(mat4(1.0f), vec3(64.0f, 0.02f, 64.0f));
        setWorldMatrix(texturedShaderProgram, groundWorldMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

        groundWorldMatrix = translate(mat4(1.0f), vec3(-32.0f, -0.01f, 32.0f)) * scale(mat4(1.0f), vec3(64.0f, 0.02f, 64.0f));
        setWorldMatrix(texturedShaderProgram, groundWorldMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

        groundWorldMatrix = translate(mat4(1.0f), vec3(-32.0f, -0.01f, -32.0f)) * scale(mat4(1.0f), vec3(64.0f, 0.02f, 64.0f));
        setWorldMatrix(texturedShaderProgram, groundWorldMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0


        //Draw stage
        glBindTexture(GL_TEXTURE_2D, fabricTextureID);

        //Draw Stage
        setWorldMatrix(texturedShaderProgram, mainStageMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        setWorldMatrix(texturedShaderProgram, leftStageMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        setWorldMatrix(texturedShaderProgram, rightStageMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);




        //Drawing the pillars 
        glBindTexture(GL_TEXTURE_2D, cementTextureID);
        //Bottom left
        setWorldMatrix(texturedShaderProgram, bottomLeftPillarMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Top left
        setWorldMatrix(texturedShaderProgram, topLeftPillarMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Main left 
        setWorldMatrix(texturedShaderProgram, mainLeftPillarMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Bottom right
        setWorldMatrix(texturedShaderProgram, bottomRightPillarMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Top right
        setWorldMatrix(texturedShaderProgram, topRightPillarMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Main right
        setWorldMatrix(texturedShaderProgram, mainRightPillarMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //Section for first group (position 1,center)---------------------------------------------------------------------------------------
        //Samuel Tardif ID : 40051573
        //Checking if our group needs to be modified
        GroupScaleMatrix = scale(mat4(1.0f), vec3(oneScale, oneScale, oneScale));
        GroupOriginRotationMatrix = rotate(mat4(1.0f), radians(oneRotation), vec3(0.0f, 1.0f, 0.0f));
        GroupTMatrix = translate(mat4(1.0f), vec3(oneX, oneY, oneZ));
        //Making our combined group matrix
        GroupMatrix = GroupTMatrix * onePostitionMatrix * GroupOriginRotationMatrix * GroupScaleMatrix;

        //Drawing our digits
        glBindTexture(GL_TEXTURE_2D, steelTextureID);

        //TO DO INSERT DRAWING OF GROUP 1
        //Section for digit 4 --------------------
        //Right part of 4 
        setWorldMatrix(texturedShaderProgram, GroupMatrix * fourRightMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Horizontal part of 4
        setWorldMatrix(texturedShaderProgram, GroupMatrix * fourHorizontalMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Left part of 4
        setWorldMatrix(texturedShaderProgram, GroupMatrix * fourLeftMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Section for digit 3 ----------------------
        //Right part of three
        setWorldMatrix(texturedShaderProgram, GroupMatrix * threeRightMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Top part of 3
        setWorldMatrix(texturedShaderProgram, GroupMatrix * threeTopMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Mid part of 3
        setWorldMatrix(texturedShaderProgram, GroupMatrix * threeMidMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Bottom part of 3
        setWorldMatrix(texturedShaderProgram, GroupMatrix * threeBottomMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Drawing our digits
        glBindTexture(GL_TEXTURE_2D, cardboardTextureID);

        //Section for letter S -------------------------
        //Right part of S
        setWorldMatrix(texturedShaderProgram, GroupMatrix * sRightMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Top part of S
        setWorldMatrix(texturedShaderProgram, GroupMatrix * sTopMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Mid part of S
        setWorldMatrix(texturedShaderProgram, GroupMatrix * sMidMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Bottom part of S
        setWorldMatrix(texturedShaderProgram, GroupMatrix * sBottomMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Left part of S
        setWorldMatrix(texturedShaderProgram, GroupMatrix * sLeftMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Section for letter L ----------------------------
        //Left part of L
        setWorldMatrix(texturedShaderProgram, GroupMatrix * lLeftMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Bottom part of L
        setWorldMatrix(texturedShaderProgram, GroupMatrix * lBottomMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);



        //Section for second group (position 2, behind left)---------------------------------------------------------------------------------------
        //Feike Qi ID : 40079084
        //Checking if our group needs to be modified
        GroupScaleMatrix = scale(mat4(1.0f), vec3(twoScale, twoScale, twoScale));
        GroupOriginRotationMatrix = rotate(mat4(1.0f), radians(twoRotation), vec3(0.0f, 1.0f, 0.0f));
        GroupTMatrix = translate(mat4(1.0f), vec3(twoX, twoY, twoZ));
        //Making our combined group matrix
        GroupMatrix = GroupTMatrix * twoPostitionMatrix * twoBaseRotationMatrix * GroupOriginRotationMatrix * GroupScaleMatrix;

        //Drawing our digits
        glBindTexture(GL_TEXTURE_2D, steelTextureID);

        //Section for first digit 4 (part of group 2)--------------------
        //Right part of 4 
        setWorldMatrix(texturedShaderProgram, GroupMatrix * four1RightMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Horizontal part of 4
        setWorldMatrix(texturedShaderProgram, GroupMatrix * four1HorizontalMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Left part of 4
        setWorldMatrix(texturedShaderProgram, GroupMatrix * four1LeftMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //Section for second digit 4 (part of group 2)--------------------
        //Right part of 4 
        setWorldMatrix(texturedShaderProgram, GroupMatrix * four2RightMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Horizontal part of 4
        setWorldMatrix(texturedShaderProgram, GroupMatrix * four2HorizontalMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Left part of 4
        setWorldMatrix(texturedShaderProgram, GroupMatrix * four2LeftMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Drawing our letters
        glBindTexture(GL_TEXTURE_2D, cardboardTextureID);

        //Section for F (part of group 2)--------------------
        setWorldMatrix(texturedShaderProgram, GroupMatrix * fVerticalMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Horizontal part of F
        setWorldMatrix(texturedShaderProgram, GroupMatrix * fTopMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Horizontal part2 of F
        setWorldMatrix(texturedShaderProgram, GroupMatrix * fMiddleMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Section for E (part of group 2)--------------------
        setWorldMatrix(texturedShaderProgram, GroupMatrix * eVerticalMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Horizontal part of E
        setWorldMatrix(texturedShaderProgram, GroupMatrix * eTopMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Horizontal part2 of E
        setWorldMatrix(texturedShaderProgram, GroupMatrix * eMiddleMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Horizontal part3 of E
        setWorldMatrix(texturedShaderProgram, GroupMatrix * eBottomMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //Section for third group (position 3, behind right)---------------------------------------------------------------------------------------
        //
        //Checking if our group needs to be modified
        GroupScaleMatrix = scale(mat4(1.0f), vec3(threeScale, threeScale, threeScale));
        GroupOriginRotationMatrix = rotate(mat4(1.0f), radians(threeRotation), vec3(0.0f, 1.0f, 0.0f));
        GroupTMatrix = translate(mat4(1.0f), vec3(threeX, threeY, threeZ));
        //Making our combined group matrix
        GroupMatrix = GroupTMatrix * threePostitionMatrix * threeBaseRotationMatrix * GroupOriginRotationMatrix * GroupScaleMatrix;

        //Drawing our digits
        //Lam Tran Student ID: 40088195

        //Drawing our digits
        //Drawing our digits
        glBindTexture(GL_TEXTURE_2D, steelTextureID);
        //Section for number 4 (Part of group 3)
        //Lam Tran Student ID: 40088195
        //Right part of 4 
        setWorldMatrix(texturedShaderProgram, GroupMatrix * fourRightMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Horizontal part of 4
        setWorldMatrix(texturedShaderProgram, GroupMatrix * fourHorizontalMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Left part of 4
        setWorldMatrix(texturedShaderProgram, GroupMatrix * fourLeftMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Section for number 5 (part of group 3)-------------------------
        //Right part of 5
        setWorldMatrix(texturedShaderProgram, GroupMatrix * FiveRightMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Top part of 5
        setWorldMatrix(texturedShaderProgram, GroupMatrix * FiveTopMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Mid part of 5
        setWorldMatrix(texturedShaderProgram, GroupMatrix * FiveMidMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Bottom part of 5
        setWorldMatrix(texturedShaderProgram, GroupMatrix * FiveBottomMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Left part of 5
        setWorldMatrix(texturedShaderProgram, GroupMatrix * FiveLeftMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Drawing our letters
        glBindTexture(GL_TEXTURE_2D, cardboardTextureID);

        //Section for letter L (part of group 2)----------------------------
        //Left part of L
        setWorldMatrix(texturedShaderProgram, GroupMatrix * LamLLeftMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Bottom part of L
        setWorldMatrix(texturedShaderProgram, GroupMatrix * LamLBottomMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Section for letter M
        //Draw left part of M
        setWorldMatrix(texturedShaderProgram, GroupMatrix * MLeftMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Draw the middle left of M
        setWorldMatrix(texturedShaderProgram, GroupMatrix * MMiddleLeftMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Draw middle right of M
        setWorldMatrix(texturedShaderProgram, GroupMatrix * MMiddleRightMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Draw right of M
        setWorldMatrix(texturedShaderProgram, GroupMatrix * MRightMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //Section for fourth group (position 4, front right)---------------------------------------------------------------------------------------
        //Name: Adam Richard ID: 27059329
        //Checking if our group needs to be modified
        GroupScaleMatrix = scale(mat4(1.0f), vec3(fourScale, fourScale, fourScale));
        GroupOriginRotationMatrix = rotate(mat4(1.0f), radians(fourRotation), vec3(0.0f, 1.0f, 0.0f));
        GroupTMatrix = translate(mat4(1.0f), vec3(fourX, fourY, fourZ));
        //Making our combined group matrix
        GroupMatrix = GroupTMatrix * fourPostitionMatrix * fourBaseRotationMatrix * GroupOriginRotationMatrix * GroupScaleMatrix;

        glBindTexture(GL_TEXTURE_2D, steelTextureID);

        //Drawing our digits
        //Section for digit 2 (group 4)
        //bottom part of 2
        setWorldMatrix(texturedShaderProgram, GroupMatrix * bottom2Matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //middle part of 2
        setWorldMatrix(texturedShaderProgram, GroupMatrix * middle2Matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //top part of 2
        setWorldMatrix(texturedShaderProgram, GroupMatrix * top2Matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //left part of 2
        setWorldMatrix(texturedShaderProgram, GroupMatrix * left2Matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //right part of 2
        setWorldMatrix(texturedShaderProgram, GroupMatrix * right2Matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Section for digit 9 (group 4)
        //middle part of 9
        setWorldMatrix(texturedShaderProgram, GroupMatrix * middle9Matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //top part of 9
        setWorldMatrix(texturedShaderProgram, GroupMatrix * top9Matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //left part of 9
        setWorldMatrix(texturedShaderProgram, GroupMatrix * left9Matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //right part of 9
        setWorldMatrix(texturedShaderProgram, GroupMatrix * right9Matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Drawing our letters
        glBindTexture(GL_TEXTURE_2D, cardboardTextureID);

        //Section for Letter A (group 4)
        //Draw Left part of A
        setWorldMatrix(texturedShaderProgram, GroupMatrix * ALeftMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Draw Right part of A
        setWorldMatrix(texturedShaderProgram, GroupMatrix * ARightMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Draw Middle part of A
        setWorldMatrix(texturedShaderProgram, GroupMatrix * AMiddleMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Section for Letter M (group 4)
        //Draw left part of M
        setWorldMatrix(texturedShaderProgram, GroupMatrix * MLeftMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Draw the middle left of M
        setWorldMatrix(texturedShaderProgram, GroupMatrix * MMiddleLeftMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Draw middle right of M
        setWorldMatrix(texturedShaderProgram, GroupMatrix * MMiddleRightMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Draw right of M
        setWorldMatrix(texturedShaderProgram, GroupMatrix * MRightMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //Section for fifth group (position 5, front left)---------------------------------------------------------------------------------------
        //
        //Checking if our group needs to be modified
        GroupScaleMatrix = scale(mat4(1.0f), vec3(fiveScale, fiveScale, fiveScale));
        GroupOriginRotationMatrix = rotate(mat4(1.0f), radians(fiveRotation), vec3(0.0f, 1.0f, 0.0f));
        GroupTMatrix = translate(mat4(1.0f), vec3(fiveX, fiveY, fiveZ));
        //Making our combined group matrix
        GroupMatrix = GroupTMatrix * fivePostitionMatrix * fiveBaseRotationMatrix * GroupOriginRotationMatrix * GroupScaleMatrix;

        //Drawing our digits
        //Draw left stick of D
        setWorldMatrix(texturedShaderProgram, GroupMatrix * DLeftMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Draw top stick of D
        setWorldMatrix(texturedShaderProgram, GroupMatrix * DTopMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Draw bottom stick of D
        setWorldMatrix(texturedShaderProgram, GroupMatrix * DBottomMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Draw right stick of D
        setWorldMatrix(texturedShaderProgram, GroupMatrix * DRightMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Draw left stick of K
        setWorldMatrix(texturedShaderProgram, GroupMatrix * KLeftMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Draw bottom slash of K
        setWorldMatrix(texturedShaderProgram, GroupMatrix * KBottomMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Draw top slash of K
        setWorldMatrix(texturedShaderProgram, GroupMatrix * KTopMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindTexture(GL_TEXTURE_2D, steelTextureID);

        //Section for number 4
        //Right part of 4 
        setWorldMatrix(texturedShaderProgram, GroupMatrix * four1RightMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Horizontal part of 4
        setWorldMatrix(texturedShaderProgram, GroupMatrix * four1HorizontalMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Left part of 4
        setWorldMatrix(texturedShaderProgram, GroupMatrix * four1LeftMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Section for digit 2 (group 5)
       //bottom part of 2
        setWorldMatrix(texturedShaderProgram, GroupMatrix * GroupFivebottom2Matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //middle part of 2
        setWorldMatrix(texturedShaderProgram, GroupMatrix * GroupFivemiddle2Matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //top part of 2
        setWorldMatrix(texturedShaderProgram, GroupMatrix * GroupFivetop2Matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //left part of 2
        setWorldMatrix(texturedShaderProgram, GroupMatrix * GroupFiveleft2Matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //right part of 2
        setWorldMatrix(texturedShaderProgram, GroupMatrix * GroupFiveright2Matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw colored geometry------------------------------------------------------------------------------------------------------
        glUseProgram(colorShaderProgram);
        glUniform3fv(colorLocation, 0, value_ptr(vec3(1.0, 0.0, 0.0)));

        //TO DO -- Save for last
        //Draw screen
        WorldMatrix = screenMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.0, 0.0)));
        glDrawArrays(GL_TRIANGLES, 0, 36);




        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();


        // Handle inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);



        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;


        // - Calculate mouse motion dx and dy
        // - Update camera horizontal and vertical angle
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);

        double dx = mousePosX - lastMousePosX;
        double dy = mousePosY - lastMousePosY;

        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        // Convert to spherical coordinates
        const float cameraAngularSpeed = 60.0f;
        cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

        // Clamp vertical angle to [-85, 85] degrees
        cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));

        float theta = radians(cameraHorizontalAngle);
        float phi = radians(cameraVerticalAngle);

        cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
        vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

        glm::normalize(cameraSideVector);


        // GVBN movement ------------------------------------------------------
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) // move camera to the left
        {
            cameraPosition -= cameraSideVector * dt * cameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) // move camera to the right
        {
            cameraPosition += cameraSideVector * dt * cameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) // move camera up
        {
            cameraPosition -= cameraLookAt * dt * cameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) // move camera down
        {
            cameraPosition += cameraLookAt * dt * cameraSpeed;
        }

        // Orientation controls--------------------------------------------------
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            currentOrientation.x += radians(1.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            currentOrientation.x -= radians(1.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            currentOrientation.y += radians(1.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            currentOrientation.y -= radians(1.0f);
        }
        // reset orientation
        if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
            cameraHorizontalAngle = 90.0f;
            cameraVerticalAngle = 0.0f;
            currentOrientation.y = 0;
            currentOrientation.x = 0;
            cameraPosition = vec3(0.0f, 5.0f, 20.0f);
            cameraLookAt = vec3(0.0f, 0.0f, 0.0f);
            fov = 70.0f;
            viewMatrix = lookAt(cameraPosition,  // eye
                cameraLookAt,  // center
                cameraUp); // up
        }


        //Preset Camera locations----------------------------------------------------------------------------
        //Preset for center
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            cameraHorizontalAngle = 90.0f;
            cameraVerticalAngle = 0.0f;
            currentOrientation.y = 0;
            currentOrientation.x = 0;
            //Snapping camera to center
            cameraPosition = glm::vec3(0.0f, 5.0f, 45.0f);
            cameraLookAt = glm::vec3(0.0f, 0.0f, -1.0f);

            viewMatrix = glm::lookAt(cameraPosition,  // eye,
                cameraLookAt,  // center
                glm::vec3(0.0f, 1.0f, 0.0f));// up

            viewMatrixLocation = glGetUniformLocation(colorShaderProgram, "viewMatrix");
            glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

            //Setting current position to 1
            position = 1;
        }
        //Preset for behind left
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            cameraHorizontalAngle = 121.0f;
            cameraVerticalAngle = 0.0f;
            currentOrientation.y = 0;
            currentOrientation.x = 0;
            cameraPosition = glm::vec3(-40 * sin(3.14159 / 4), 5.0f, -10 * cos(3.14159 / 4));
            cameraLookAt = glm::vec3(-0.6f, 0.0f, -1.0f);

            viewMatrix = glm::lookAt(cameraPosition,  // eye
                cameraLookAt,  // center
                glm::vec3(0.0f, 1.0f, 0.0f));// up

            viewMatrixLocation = glGetUniformLocation(colorShaderProgram, "viewMatrix");
            glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

            //Setting current position to 2
            position = 2;
        }

        //Preset for behind right
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            cameraHorizontalAngle = 35.5f;
            cameraVerticalAngle = 0.0f;
            currentOrientation.y = 0;
            currentOrientation.x = 0;
            cameraPosition = glm::vec3(10 * sin(3.14159 / 4), 5.0f, -30 * cos(3.14159 / 4));
            cameraLookAt = glm::vec3(1.4f, 0.0f, -1.0f);

            viewMatrix = glm::lookAt(cameraPosition,  // eye
                cameraLookAt,  // center
                glm::vec3(0.0f, 1.0f, 0.0f));// up

            viewMatrixLocation = glGetUniformLocation(colorShaderProgram, "viewMatrix");
            glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

            //Setting current position to 3
            position = 3;
        }

        //Preset for front right
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            cameraHorizontalAngle = -46.5f;
            cameraVerticalAngle = 0.0f;
            currentOrientation.y = 0;
            currentOrientation.x = 0;
            cameraPosition = glm::vec3(20 * sin(3.14159 / 4), 5.0f, 20 * cos(3.14159 / 4));
            cameraLookAt = glm::vec3(0.95f, 0.0f, 1.0f);

            viewMatrix = glm::lookAt(cameraPosition,  // eye
                cameraLookAt,  // center
                glm::vec3(0.0f, 1.0f, 0.0f));// up

            viewMatrixLocation = glGetUniformLocation(colorShaderProgram, "viewMatrix");
            glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

            //Setting current position to 4
            position = 4;
        }

        //Preset for front left
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            cameraHorizontalAngle = -135.5f;
            cameraVerticalAngle = 0.0f;
            currentOrientation.y = 0;
            currentOrientation.x = 0;
            cameraPosition = glm::vec3(-20 * sin(3.14159 / 4), 5.0f, 20 * cos(3.14159 / 4));
            cameraLookAt = glm::vec3(-1.02f, 0.0f, 1.0f);

            viewMatrix = glm::lookAt(cameraPosition,  // eye
                cameraLookAt,  // center
                glm::vec3(0.0f, 1.0f, 0.0f));// up

            viewMatrixLocation = glGetUniformLocation(colorShaderProgram, "viewMatrix");
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
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // Rotate counter-clockwise
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
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // Rotate clockwise
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
        //It is relative to the starting position's perspective
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) //T = negative Z axis (or back for default view)
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
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) // G - Positive Z axis
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
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //F = negative X axis (or back for default view)
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
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // G - Positive X axis
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
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //O = positive Y axis
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
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // L -Negative Y axis
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

        mat4 viewMatrix(1.0f);

        viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

        setViewMatrix(colorShaderProgram, viewMatrix);
        setViewMatrix(texturedShaderProgram, viewMatrix);

        // Orientation Matrix
        orientationMatrix = rotate(rotate(mat4(1.0f), currentOrientation.x, vec3(1.0f, 0.0f, 0.0f)), currentOrientation.y, vec3(0.0f, 1.0f, 0.0f));
        setOrientationMatrix(colorShaderProgram, orientationMatrix);
        setOrientationMatrix(texturedShaderProgram, orientationMatrix);

    }

    glfwTerminate();

    return 0;
}

const char* getVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        ""
        "uniform mat4 worldMatrix;"
        "uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
        "uniform mat4 projectionMatrix = mat4(1.0);"
        ""
        "out vec3 vertexColor;"
        "void main()"
        "{"
        "   vertexColor = aColor;"
        "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
        "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "}";
}

const char* getFragmentShaderSource()
{
    return
        "#version 330 core\n"
        "in vec3 vertexColor;"
        "out vec4 FragColor;"
        "void main()"
        "{"
        "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
        "}";
}

const char* getTexturedVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        "layout (location = 2) in vec2 aUV;"
        "layout (location = 3) in vec3 aNormal;"
        ""
        "uniform mat4 worldMatrix;"
        "uniform mat4 viewMatrix = mat4(1.0);"  // default value for view    matrix (identity)
        "uniform mat4 projectionMatrix = mat4(1.0);"
        ""
        "out vec3 vertexColor;"
        "out vec2 vertexUV;"
        "out vec3 vertexNormal;"
        "out vec3 FragPos;"
        ""
        "void main()"
        "{"
        "   vertexColor = aColor;"
        "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
        "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "   vertexUV = aUV;"
        "   FragPos = vec3(worldMatrix * vec4(aPos, 1.0));"
        "   vertexNormal = mat3(transpose(inverse(worldMatrix))) * vertexNormal;"
        "}";
}

const char* getTexturedFragmentShaderSource()
{
    return
        "#version 330 core\n"
        "in vec3 vertexColor;"
        "in vec2 vertexUV;"
        "in vec3 vertexNormal;"
        "in vec3 FragPos;"
        ""
        "uniform sampler2D textureSampler;"
        "uniform vec3 lightPos0 = vec3(0.0f, 30.0f, 0.0f);"
        
        ""
        "out vec4 FragColor;"
        "void main()"
        "{"
        //Ambient light
        "vec3 ambientLight = vec3(0.1f, 0.1f, 0.1f);"

        //Diffuse light
        "vec3 norm = normalize(vertexNormal);"
        "vec3 lightDir = normalize(lightPos0 - FragPos);"
        "float diff = max(dot(norm, lightDir), 0.0);"
        "vec3 diffuseColor = vec3(1.0f, 1.0f, 1.0f);"
        "vec3 diffuse = diff * diffuseColor;"

        "vec4 textureColor = texture( textureSampler, vertexUV );"
        "FragColor = textureColor * (vec4(ambientLight, 1.0f) + vec4(diffuse, 1.0f));"
        "}";
}

int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
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

GLuint loadTexture(const char* filename)
{
    // Step1 Create and bind textures
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    assert(textureId != 0);


    glBindTexture(GL_TEXTURE_2D, textureId);

    // Step2 Set filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Step3 Load Textures with dimension data
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
        return 0;
    }

    // Step4 Upload the texture to the PU
    GLenum format = 0;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
        0, format, GL_UNSIGNED_BYTE, data);

    // Step5 Free resources
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureId;
}

int createTexturedCubeVertexArrayObject()
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(TexturedColoredVertex),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(TexturedColoredVertex),
        (void*)(2 * sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and color)
    );
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3,                            // attribute 2 matches aUV in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(TexturedColoredVertex),
        (void*)(3 * sizeof(vec3))      // uv is offseted by 3 vec3 (comes after position and color and UV)
    );
    glEnableVertexAttribArray(3);

    return vertexArrayObject;
}
