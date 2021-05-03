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
#include <OIFace.hpp>
#include <OINullDFaceTracker.hpp>
#include "Cylinder.h"


using namespace glm;
using namespace std;



GLuint loadTexture(const char* filename);

const char* getVertexShaderSource();

const char* getFragmentShaderSource();

const char* getTexturedVertexShaderSource();

const char* getTexturedFragmentShaderSource();

const char* getShadowVertexShaderSource();

const char* getShadowFragmentShaderSource();

int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource);

struct TexturedColoredVertex
{
	TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv, vec3 _normal)
		: position(_position), color(_color), uv(_uv), normal(_normal) {}

	vec3 position;
	vec3 color;
	vec2 uv;
	vec3 normal;

	TexturedColoredVertex() {
		this->position = vec3(0.0f, 0.0f, 0.0f);
		this->color = vec3(0.0f, 0.0f, 0.0f);;
		this->uv = vec2(0.0f, 0.0f);;
		this->normal = vec3(0.0f, 0.0f, 0.0f);;
	}
};

void updateVertexArray(openiss::OIFace* face, TexturedColoredVertex texturedFaceVertexArray[]) {

	int circleIncrements = 8;
	float radius = 0.05f;
	float angleStep = 360.0f / circleIncrements;

	for (int i = 0; i < 72; i++) {

		float x = face->facialLandmarks->at(0).at(i).x;
		float y = face->facialLandmarks->at(0).at(i).y;

		for (int j = 0; j < circleIncrements; j++) {
			//Middle point
			texturedFaceVertexArray[24 * i + 3 * j].position = vec3(x, y, 0.25f);
			texturedFaceVertexArray[24 * i + 3 * j].color = vec3(1.0f, 0.0f, 0.0f);
			texturedFaceVertexArray[24 * i + 3 * j].uv = vec2(0.0f, 0.0f);
			texturedFaceVertexArray[24 * i + 3 * j].normal = vec3(0.0f, 0.0f, 1.0f);
			//First edge point
			texturedFaceVertexArray[24 * i + 3 * j + 1].position = vec3(x + radius * cos(radians(j * angleStep)), y + radius * sin(radians(j * angleStep)), 0.25f);
			texturedFaceVertexArray[24 * i + 3 * j + 1].color = vec3(1.0f, 0.0f, 0.0f);
			texturedFaceVertexArray[24 * i + 3 * j + 1].uv = vec2(cos(radians(j * angleStep)), sin(radians(j * angleStep)));
			texturedFaceVertexArray[24 * i + 3 * j + 1].normal = vec3(0.0f, 0.0f, 1.0f);
			//First edge point
			texturedFaceVertexArray[24 * i + 3 * j + 2].position = vec3(x + radius * cos(radians((j + 1) * angleStep)), y + radius * sin(radians((j + 1) * angleStep)), 0.25f);
			texturedFaceVertexArray[24 * i + 3 * j + 2].color = vec3(1.0f, 0.0f, 0.0f);
			texturedFaceVertexArray[24 * i + 3 * j + 2].uv = vec2(cos(radians((j + 1) * angleStep)), sin(radians((j + 1) * angleStep)));
			texturedFaceVertexArray[24 * i + 3 * j + 2].normal = vec3(0.0f, 0.0f, 1.0f);
		}
	}
	//Generate cylinders for links
	

};

// Textured Cube model
TexturedColoredVertex texturedCubeVertexArray[] = {
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

template <class T>
void SetUniform1Value(GLuint shader_id, const char* uniform_name, T uniform_value)
{
	glUseProgram(shader_id);
	glUniform1i(glGetUniformLocation(shader_id, uniform_name), uniform_value);
	glUseProgram(0);
}


void SetUniformMat4(GLuint shader_id, const char* uniform_name, mat4 uniform_value)
{
	glUseProgram(shader_id);
	glUniformMatrix4fv(glGetUniformLocation(shader_id, uniform_name), 1, GL_FALSE, &uniform_value[0][0]);
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
	GLFWwindow* window = glfwCreateWindow(800, 600, "Comp371 Assignment", NULL, NULL);
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
	GLuint tilesTextureID = loadTexture("assets/textures/tiles2.jpg");
	GLuint steelTextureID = loadTexture("assets/textures/steel.jpg");
	GLuint cardboardTextureID = loadTexture("assets/textures/cardboard2.jpg");
	GLuint fabricTextureID = loadTexture("assets/textures/fabric2.jpg");
	GLuint group1TextureID = loadTexture("assets/textures/group1resize.jpg");
	GLuint group2TextureID = loadTexture("assets/textures/group2resize.jpg");
	GLuint group3TextureID = loadTexture("assets/textures/group3resize.jpg");
	GLuint group4TextureID = loadTexture("assets/textures/group4resize.jpg");
	GLuint group5TextureID = loadTexture("assets/textures/group5resize.jpg");

	// Black background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Compile and link shaders here ...
	int colorShaderProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());
	int texturedShaderProgram = compileAndLinkShaders(getTexturedVertexShaderSource(), getTexturedFragmentShaderSource());
	int shadowShaderProgram = compileAndLinkShaders(getShadowVertexShaderSource(), getShadowFragmentShaderSource());

	// SHADER PROGRAM LOCATIONS
	GLuint worldMatrixLocation = glGetUniformLocation(colorShaderProgram, "worldMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(colorShaderProgram, "viewMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(colorShaderProgram, "projectionMatrix");
	GLuint colorLocation = glGetUniformLocation(colorShaderProgram, "vertexColor");
	GLuint isTexturedLocation = glGetUniformLocation(colorShaderProgram, "isTextured");

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


	// Set initial view matrix
	mat4 viewMatrix = lookAt(cameraPosition,  // eye
		cameraPosition + cameraLookAt,  // center
		cameraUp); // up

	// Set View and Projection matrices on both shaders
	setViewMatrix(colorShaderProgram, viewMatrix);
	setViewMatrix(texturedShaderProgram, viewMatrix);

	setProjectionMatrix(colorShaderProgram, projectionMatrix);
	setProjectionMatrix(texturedShaderProgram, projectionMatrix);


	// For frame time
	float lastFrameTime = glfwGetTime();
	int lastMouseLeftState = GLFW_RELEASE;
	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

	// Other OpenGL states to set once
	// Enable Backface culling
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);


	// Define and upload geometry to the GPU here ...
	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObjectCube;
	glGenBuffers(1, &vertexBufferObjectCube);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectCube);
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

	glVertexAttribPointer(3,                            // attribute 3 matches aNormal in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedColoredVertex),
		(void*)(2 * sizeof(vec3) + sizeof(vec2))      // uv is offseted by 2 vec3 and one vec2 (comes after position and color and UV)
	);
	glEnableVertexAttribArray(3);


	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//Project part 3

	const int numberOfPointsPerFace = 72;
	const int circlePrecision = 8;

	const static int texturedFaceArraySize = 3 * numberOfPointsPerFace * circlePrecision;

	//Creating OINullDFaceTracker
	openiss::OINullDFaceTracker OINFT = openiss::OINullDFaceTracker();

	//Getting our first face
	openiss::OIFace* face = OINFT.getNextFace();
	

	//TO DO ----------------------------------------------
	//Put parameters for array size
	//num of faces
	//res of spheres
	//res of cylinders

	//Creating our vertex array for faces
	static TexturedColoredVertex texturedFaceVertexArray[texturedFaceArraySize];

	//Generating pointers to arrays of vertex

	updateVertexArray(face, texturedFaceVertexArray);
	


	std::cerr << "First texturedVertex : x :" << texturedFaceVertexArray[0].position.x << " y: " << texturedFaceVertexArray[0].position.y << " z: " << texturedFaceVertexArray[0].position.z
		<< " color : r: " << texturedFaceVertexArray[0].color.x << " g: " << texturedFaceVertexArray[0].color.y << " b: " << texturedFaceVertexArray[0].position.z << std::endl;
	std::cerr << "Last texturedVertex : x :" << texturedFaceVertexArray[1727].position.x << " y: " << texturedFaceVertexArray[1727].position.y << " z: " << texturedFaceVertexArray[1727].position.z
		<< " color : r: " << texturedFaceVertexArray[112].color.x << " g: " << texturedFaceVertexArray[112].color.y << " b: " << texturedFaceVertexArray[112].position.z << std::endl;

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObjectFace;
	glGenBuffers(1, &vertexBufferObjectFace);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectFace);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturedFaceVertexArray), texturedFaceVertexArray, GL_DYNAMIC_DRAW);
	

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		sizeof(TexturedColoredVertex), // stride - each vertex contain 3 vec3 (position, color, normal) and a vec 2 (uv)
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

	glVertexAttribPointer(3,                            // attribute 3 matches aNormal in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedColoredVertex),
		(void*)(2 * sizeof(vec3) + sizeof(vec2))      // uv is offseted by 2 vec3 and one vec2 (comes after position and color and UV)
	);
	glEnableVertexAttribArray(3);

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// Initialize Matrices
	mat4 model = mat4(1.0f); // identity matrix
	mat4 WorldMatrix = mat4(1.0f); // Matrix used to create objects
	mat4 orientationMatrix = mat4(1.0f); // initialize orientation matrix

	//Stage matrices
	// Stage
	//Main cube
	mat4 mainStageMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -50.0f)) * scale(mat4(1.0f), vec3(50.0f, 2.0f, 10.0f));

	//Right cube
	mat4 rightStageMatrix = translate(mat4(1.0f), vec3(20.0f, 0.0f, -40.0f)) * scale(mat4(1.0f), vec3(10.0f, 2.0f, 10.0f));

	//Left cube
	mat4 leftStageMatrix = translate(mat4(1.0f), vec3(-20.0f, 0.0f, -40.0f)) * scale(mat4(1.0f), vec3(10.0f, 2.0f, 10.0f));;

	// Screen
	mat4 screenMatrix = translate(mat4(1.0f), vec3(0.0f, 7.0f, -54.0f)) * scale(mat4(1.0f), vec3(20.0f, 10.0f, 0.05f));

	//Left pillar top
	mat4 topLeftPillarMatrix = translate(mat4(1.0f), vec3(-10.0f, 12.5f, -54.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));;

	//Left pillar bottom
	mat4 bottomLeftPillarMatrix = translate(mat4(1.0f), vec3(-10.0f, 1.5f, -54.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));;

	//Main left pillar
	mat4 mainLeftPillarMatrix = translate(mat4(1.0f), vec3(-10.0f, 7.0f, -54.0f)) * scale(mat4(1.0f), vec3(0.5f, 10.0f, 0.5f));

	//Right pillar top
	mat4 topRightPillarMatrix = translate(mat4(1.0f), vec3(10.0f, 12.5f, -54.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));

	//Right pillar bottom
	mat4 bottomRightPillarMatrix = translate(mat4(1.0f), vec3(10.0f, 1.5f, -54.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));

	//Main right pillar
	mat4 mainRightPillarMatrix = translate(mat4(1.0f), vec3(10.0f, 7.0f, -54.0f)) * scale(mat4(1.0f), vec3(0.5f, 10.0f, 0.5f));

	//More definition (
	vec2 currentOrientation(0.0f, 0.0f); // current orientation of matrix
	float timeOnTexture = 0;
	int currentTexture = 1;
	GLuint isTextured = 1;
	float timeOnFace = 0;

	float lightAngleOuter = 30.0;
	float lightAngleInner = 20.0;
	// Set light cutoff angles on scene shader
	SetUniform1Value(texturedShaderProgram, "light_cutoff_inner", cos(radians(lightAngleInner)));
	SetUniform1Value(texturedShaderProgram, "light_cutoff_outer", cos(radians(lightAngleOuter)));


	// Entering Main Loop---------------------------------------------------------------------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw textured geometry
		glUseProgram(texturedShaderProgram);

		glActiveTexture(GL_TEXTURE0);
		GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
		glUniform1i(textureLocation, 0);                // Set our Texture sampler to user Texture Unit 0

		//Set the light position to uniform variable lightPos0
		glUniform3fv(glGetUniformLocation(texturedShaderProgram, "lightPos0"), 1, &lightPos0[0]);

		//Set the camera position to uniform variable viewPos
		glUniform3fv(glGetUniformLocation(texturedShaderProgram, "viewPos"), 1, &cameraPosition[0]);

		//Toggle for texture
		isTexturedLocation = glGetUniformLocation(texturedShaderProgram, "isTextured");
		glUniform1i(isTexturedLocation, isTextured);

		vec3 lightFocus(0.0, 0.0, -1.0);      // the point in 3D space the light "looks" at
		vec3 lightDirection = normalize(lightFocus - lightPos0);

		float lightNearPlane = 1.0f;
		float lightFarPlane = 180.0f;

		mat4 lightProjectionMatrix = frustum(-1.0f, 1.0f, -1.0f, 1.0f, lightNearPlane, lightFarPlane);
		//perspective(20.0f, (float)DEPTH_MAP_TEXTURE_SIZE / (float)DEPTH_MAP_TEXTURE_SIZE, lightNearPlane, lightFarPlane);
		mat4 lightViewMatrix = lookAt(lightPos0, lightFocus, vec3(0.0f, 1.0f, 0.0f));
		mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;

		//Set light space matrix on shadow shader and scene shader
		SetUniformMat4(shadowShaderProgram, "light_view_proj_matrix", lightSpaceMatrix);
		SetUniformMat4(texturedShaderProgram, "light_view_proj_matrix", lightSpaceMatrix);

		// Set light far and near planes on scene shader
		SetUniform1Value(texturedShaderProgram, "light_near_plane", lightNearPlane);
		SetUniform1Value(texturedShaderProgram, "light_far_plane", lightFarPlane);

		//Set model matrix for shadow shader
		SetUniformMat4(shadowShaderProgram, "model_matrix", WorldMatrix);

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
			glBindTexture(GL_TEXTURE_2D, group1TextureID);
			break;
		case 2:
			glBindTexture(GL_TEXTURE_2D, group2TextureID);
			break;
		case 3:
			glBindTexture(GL_TEXTURE_2D, group3TextureID);
			break;
		case 4:
			glBindTexture(GL_TEXTURE_2D, group4TextureID);
			break;
		case 5:
			glBindTexture(GL_TEXTURE_2D, group5TextureID);
			break;
		}

		//Drawing cubes------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectCube);
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

		glVertexAttribPointer(3,                            // attribute 3 matches aNormal in Vertex Shader
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(TexturedColoredVertex),
			(void*)(2 * sizeof(vec3) + sizeof(vec2))      // uv is offseted by 2 vec3 and one vec2 (comes after position and color and UV)
		);
		glEnableVertexAttribArray(3);

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

		//Drawing face------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		setWorldMatrix(texturedShaderProgram, model);
		int currentFace = 0;
		int num = 0;
		//The face will switch every 5 seconds, reset to the first face after 35 seconds
		timeOnFace += dt;
		if (timeOnFace > 5){
			face = OINFT.getNextFace();
			updateVertexArray(face, texturedFaceVertexArray);
			timeOnFace = 0;
		}	
		
		//Bind the new vertex array to the buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectFace);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texturedFaceVertexArray), texturedFaceVertexArray, GL_DYNAMIC_DRAW);
		//Enable the attribute
		glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
			3,                   // size
			GL_FLOAT,            // type
			GL_FALSE,            // normalized?
			sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
			(void*)0             // array buffer offset
		); glBindTexture(GL_TEXTURE_2D, tilesTextureID);
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

		glVertexAttribPointer(3,                            // attribute 3 matches aNormal in Vertex Shader
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(TexturedColoredVertex),
			(void*)(2 * sizeof(vec3) + sizeof(vec2))      // uv is offseted by 2 vec3 and one vec2 (comes after position and color and UV)
		);
		glEnableVertexAttribArray(3);

		glDrawArrays(GL_TRIANGLES, 0, texturedFaceArraySize);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);


		// Cylinder Test 
		


		// Draw colored geometry------------------------------------------------------------------------------------------------------
		glUseProgram(colorShaderProgram);
		glUniform3fv(colorLocation, 0, value_ptr(vec3(1.0, 0.0, 0.0)));

		// End Frame
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Handle inputs
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

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

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
		{
			cameraPosition -= cameraSideVector * dt * cameraSpeed * 3.0f;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move camera to the right
		{
			cameraPosition += cameraSideVector * dt * cameraSpeed * 3.0f;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move camera up
		{
			cameraPosition -= cameraLookAt * dt * cameraSpeed * 3.0f;
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move camera down
		{
			cameraPosition += cameraLookAt * dt * cameraSpeed * 3.0f;
		}

		// Could get rid of
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) //Key to toggle Textures on
		{
			isTextured = 1;
		}
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) //Key to toggle Textures off
		{
			isTextured = 0;
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
		"uniform mat4 viewMatrix;"      // default value for view matrix (identity)
		"uniform mat4 projectionMatrix;"
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
		"uniform mat4 light_view_proj_matrix;"
		""
		"out vec3 vertexColor;"
		"out vec2 vertexUV;"
		"out vec3 vertexNormal;"
		"out vec3 FragPos;"
		"out vec3 fragment_normal;"
		"out vec3 fragment_position;"
		"out vec4 fragment_position_light_space;"
		""
		"void main()"
		"{"
		"   vertexColor = aColor;"
		"   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
		"   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
		"   vertexUV = aUV;"
		"   FragPos = vec3(worldMatrix * vec4(aPos, 1.0));"
		"   vertexNormal = mat3(transpose(inverse(worldMatrix))) * aNormal;"
		"fragment_normal = mat3(modelViewProjection) * aNormal;"
		"fragment_position = vec3(modelViewProjection * vec4(aPos, 1.0));"
		"fragment_position_light_space = light_view_proj_matrix * vec4(fragment_position, 1.0);"
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
		"uniform sampler2D shadow_map;"
		"uniform vec3 lightPos0;"
		"uniform vec3 viewPos;"
		"uniform int isTextured = 1;"

		"uniform float light_cutoff_outer;"
		"uniform float light_cutoff_inner;"
		"uniform float light_near_plane;"
		"uniform float light_far_plane;"

		""
		"out vec4 FragColor;"
		//"float shadow_scalar() {"
		//// this function returns 1.0 when the surface receives light, and 0.0 when it is in a shadow
		//// perform perspective divide
		//"vec3 normalized_device_coordinates = fragment_position_light_space.xyz / fragment_position_light_space.w;"
		//// transform to [0,1] range
		//"normalized_device_coordinates = normalized_device_coordinates * 0.5 + 0.5;"
		//// get closest depth value from light's perspective (using [0,1] range fragment_position_light_space as coords)
		//"float closest_depth = texture(shadow_map, normalized_device_coordinates.xy).r;"
		//// get depth of current fragment from light's perspective
		//"float current_depth = normalized_device_coordinates.z;"
		//// check whether current frag pos is in shadow
		//"float bias = 0;  // bias applied in depth map: see shadow_vertex.glsl"
		//"return ((current_depth - bias) < closest_depth) ? 1.0 : 0.0;"
		//"}"

		//"float spotlight_scalar() {"
		//"float theta = dot(normalize(fragment_position - light_position), light_direction);"
		//"if (theta > light_cutoff_inner) {"
		//"return 1.0;"
		//"}"
		//"else if (theta > light_cutoff_outer) {"
		//"return (1.0 - cos(PI * (theta - light_cutoff_outer) / (light_cutoff_inner - light_cutoff_outer))) / 2.0;"
		//"}"
		//"else {"
		//"return 0.0;"
		//"}"
		//"}"
		"void main()"
		"{"
		//"float scalar = shadow_scalar() * spotlight_scalar();"
		//Ambient light
		"float ambientStrength = 0.1;"
		"vec3 ambientLight = ambientStrength * vec3(1.0f, 1.0f, 1.0f);"
		//Diffuse light
		"vec3 norm = normalize(vertexNormal);"
		"vec3 lightDir = normalize(lightPos0 - FragPos);"
		"float diff = max(dot(norm, lightDir), 0.0);"
		"vec3 diffuseColor = vec3(1.0f, 1.0f, 1.0f);"
		"vec3 diffuse = diff * diffuseColor;"
		//Diffuse light

		"float specularStrength = 0.5f;"

		"vec3 viewDir = normalize(viewPos - FragPos);"
		"vec3 reflectDir = reflect(-lightDir, norm);"
		"float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);"
		"vec3 specularColor = vec3(1.0f, 1.0f, 1.0f);"
		"vec3 specular = specularStrength * spec * specularColor;"
		""
		"vec4 textureColor = texture( textureSampler, vertexUV );"
		"if(isTextured==1){"
		"FragColor = textureColor * (vec4(ambientLight, 1.0f) + vec4(diffuse, 1.0f) + vec4(specular, 1.0f));"
		"}"
		"else{"
		"FragColor = vec4(vertexColor,1.0f) * (vec4(ambientLight, 1.0f) + vec4(diffuse, 1.0f) + vec4(specular, 1.0f));"
		"}"
		"}";
}
const char* getShadowVertexShaderSource()
{
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 position;"
		""
		"uniform mat4 light_view_proj_matrix;"
		"uniform mat4 model_matrix;"
		""
		"void main()"
		"{"
		"mat4 scale_bias_matrix = mat4(vec4(0.5, 0.0, 0.0, 0.0),"
		"vec4(0.0, 0.5, 0.0, 0.0),"
		"vec4(0.0, 0.0, 0.5, 0.0),"
		"vec4(0.5, 0.5, 0.5, 1.0));"
		"gl_Position = light_view_proj_matrix * model_matrix * vec4(position, 1.0);"
		"}";

}

const char* getShadowFragmentShaderSource()
{
	return
		"#version 330 core\n"
		"out vec4 FragColor;"
		""
		"in vec4 gl_FragCoord;"
		""
		"void main()"
		"{"
		"gl_FragDepth = gl_FragCoord.z;"
		"FragColor = vec4(vec3(gl_FragCoord.z), 1.0f);"
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

