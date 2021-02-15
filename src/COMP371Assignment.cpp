#include <iostream>


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
	float cameraSpeed = 1.0f;
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

    // Initialize Matrix
    mat4 model = mat4(1.0f); // identity matrix
    mat4 axisWorldMatrix; // axis matrix
    mat4 gridWorldMatrix; // grid matrix
    mat4 orientationMatrix = mat4(1.0f); // initialize orientation matrix
    mat4 scaleMatrix = mat4(1.0f); // scale matrix

    vec2 currentOrientation(0.0f, 0.0f); // current orientation of matrix
    vec3 currentScale(1.0f, 1.0f, 1.0f); // currentScale applied to scale Matrix


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

        // Cube drawn here
        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

        // Coordinates Matrix
        // X-axis
        axisWorldMatrix = translate(model, vec3(3.5f, 0.0f, 0.0f)) * scale(model, vec3(7.0f, 0.1f, 0.1f));
        setWorldMatrix(shaderProgram, axisWorldMatrix);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 0.0, 0.0)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Y-axis
        axisWorldMatrix = translate(model, vec3(0.0f, 3.5f, 0.0f)) * scale(model, vec3(0.1f, 7.0f, 0.1f));
        setWorldMatrix(shaderProgram, axisWorldMatrix);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0, 1.0, 0.0)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Z-axis
        axisWorldMatrix = translate(model, vec3(0.0f, 0.0f, 3.5f)) * scale(model, vec3(0.1f, 0.1f, 7.0f));
        setWorldMatrix(shaderProgram, axisWorldMatrix);
        glUniform3fv(colorLocation, 1, value_ptr(vec3(0.0, 0.0, 1.0)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw Grid 128x128 - TO BE WORKED ON, currently tests the scale function with U AND J
        int gridsize = 50;
       for (int i = 0; i < gridsize; ++i)
        {
            for (int j = 0; j < gridsize; ++j)
            {
                gridWorldMatrix = translate(model, vec3(0.0f, 0.0f, -gridsize/2 + j)) * scale(model, currentScale); // currentScale is there for testing purposes
                glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &gridWorldMatrix[0][0]);
                glUniform3fv(colorLocation, 1, value_ptr(vec3(1.0, 1.0, 1.0)));
                glDrawArrays(GL_TRIANGLES, 0, 36);
                
            }
        }
	//Define the Scale Matrix for scaling up/ down
        mat4 ScaleMatrix = scale(mat4(1.0f), currentScale);

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

        //ShearingMatrices
        mat4 shearingMatrixA = { 1,0,0,0,
                               0.3,1,0,0,
                                 0,0,1,0,
                                 0,0,0,1 };

        mat4 shearingMatrixB = { 1,0,0,0,
                              -0.3,1,0,0,
                                 0,0,1,0,
                                 0,0,0,1 };

        // Draw A
        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

        mat4 middle = translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.5f, 1.0f, 1.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &middle[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        mat4 left = translate(mat4(1.0f), vec3(-1.0f, 2.0f, 0.0f)) * shearingMatrixA * scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &left[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        mat4 right = translate(mat4(1.0f), vec3(1.0f, 2.0f, 0.0f)) * shearingMatrixB * scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &right[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Draw M
        mat4 first = translate(mat4(1.0f), vec3(5.0f, 2.0f, 0.0f)) * shearingMatrixA * scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &first[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        mat4 second = translate(mat4(1.0f), vec3(6.5f, 2.0f, 0.0f)) * shearingMatrixB * scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &second[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        mat4 third = translate(mat4(1.0f), vec3(8.0f, 2.0f, 0.0f)) * shearingMatrixA * scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &third[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        mat4 fourth = translate(mat4(1.0f), vec3(9.5f, 2.0f, 0.0f)) * shearingMatrixB * scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &fourth[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Orientation Matrix
        orientationMatrix = rotate(rotate(mat4(1.0f), currentOrientation.x, vec3(1.0f, 0.0f, 0.0f)), currentOrientation.y, vec3(0.0f, 1.0f, 0.0f));
        setOrientationMatrix(shaderProgram, orientationMatrix);

        // Camera tilt, pan and zoom
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);

        double dx = 0;
        double dy = 0;
        
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

        // WASD movement TO BE CHANGED
        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;
	    
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
        {
            cameraPosition -= cameraSideVector * currentCameraSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move camera to the right
        {
            cameraPosition += cameraSideVector * currentCameraSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move camera up
        {
            cameraPosition -= cameraLookAt * currentCameraSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move camera down
        {
            cameraPosition += cameraLookAt * currentCameraSpeed * dt;
        }
 
        // Orientation controls
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

        // Scale controls
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) // Scale up
        {
            currentScale.x += 0.1f;
            currentScale.y += 0.1f;
            currentScale.z += 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) // Scale down
        {
            if (currentScale.x <= 0 && currentScale.y <= 0 && currentScale.z <= 0) {
                currentScale.x = 0.0f;
                currentScale.y = 0.0f;
                currentScale.z = 0.0f;
            }
            currentScale.x -= 0.1f;
            currentScale.y -= 0.1f;
            currentScale.z -= 0.1f;
        }

        // Render types
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) 
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
