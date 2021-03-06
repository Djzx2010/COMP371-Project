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
#include <shaderloader.h>
#include <OIFace.hpp>
#include <OINullDFaceTracker.hpp>
#include <math.h>

using namespace glm;
using namespace std;

GLuint loadTexture(const char* filename);

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

//Creates cylinders and updates the vertex array
void createLink(openiss::Point2f point1, openiss::Point2f point2, int startIndex, TexturedColoredVertex texturedFaceVertexArray[]) {

	//
	int numberOfSlices = 20;

	//radius
	float radius = 0.025;

	//Angle step
	float angleStep = 360.0f / numberOfSlices;

	//Calculations for transformations
	//Calculating vector p2-p1
	vec3 linkVector = vec3(point2.x - point1.x, point2.y - point1.y, 0);

	//Our base cylinder has height of 1, therefore we must scale it by the lenght of linkVector
	float scale = sqrt(pow(linkVector.x,2) + pow(linkVector.y,2));

	//Our translation = midpoint of point 1 and point 2
	float xTranslation = (point2.x + point1.x) / 2;
	float yTranslation = (point2.y + point1.y) / 2;

	//Our rotation is acos of y/scale
	float rotation =  -acos(linkVector.y / scale);

	//We need to flip it if x < 0
	if (linkVector.x < 0) {
		rotation = -rotation;
	}

	//Main loop to create slices
	for (int currentSlice = 0; currentSlice < numberOfSlices; currentSlice++) {

		float angle = radians(angleStep * currentSlice);

		//Getting coordinates for 6 points of slice
		//Point 1 (top of cylinder)
		vec3 p1 = vec3(0.0f, 0.5f, 0.0f);

		//Point 2 (top of cylinder, on the edge)
		vec3 p2 = vec3(radius * sin(angle), 0.5f, radius * cos(angle));

		//Point 3 (top of cylinder, on the edge with anglle incremented)
		vec3 p3 = vec3(radius * sin(angle + angleStep), 0.5f, radius * cos(angle + angleStep));

		//Point 4 (bottom of cylinder, on the edge)
		vec3 p4 = vec3(radius * sin(angle), -0.5f, radius * cos(angle));

		//Point 5 (bottom of cylinder, on the edge with anglle incremented)
		vec3 p5 = vec3(radius * sin(angle + angleStep), -0.5f, radius * cos(angle + angleStep));

		//Point 6 (bottom of cylinder)
		vec3 p6 = vec3(0.0f, -0.5f, 0.0f);

		//Creating a vector so its easier to loop
		vector<vec3> pointsVector;

		//Pushing in all our points
		pointsVector.push_back(p1);
		pointsVector.push_back(p2);
		pointsVector.push_back(p3);
		pointsVector.push_back(p4);
		pointsVector.push_back(p5);
		pointsVector.push_back(p6);

		//appllying our transformation
		for (int i = 0; i < pointsVector.size(); i++) {

			//Scaling
			pointsVector.at(i) = vec3(pointsVector.at(i).x, pointsVector.at(i).y * scale, pointsVector.at(i).z);

			//Apllying our rotation
			pointsVector.at(i) = vec3(pointsVector.at(i).x * cos(rotation) - pointsVector.at(i).y * sin(rotation), pointsVector.at(i).x * sin(rotation) + pointsVector.at(i).y * cos(rotation), pointsVector.at(i).z);

			//Translation
			pointsVector.at(i) = vec3(pointsVector.at(i).x + xTranslation, pointsVector.at(i).y + yTranslation, pointsVector.at(i).z);
		}

		//Adding our triangles to the array

		//Triangle 1 (1-2-3) index 0-1-1
		//Calculating normal
		vec3 normal = normalize(cross(pointsVector.at(1) - pointsVector.at(0), pointsVector.at(2) - pointsVector.at(0)));

		//First point
		int index = startIndex + 3 * 4 * currentSlice;
		texturedFaceVertexArray[index].position = pointsVector.at(0);
		texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
		texturedFaceVertexArray[index].uv = vec2(0.5f, 0.5f);
		texturedFaceVertexArray[index].normal = normal;

		//Second point
		index += 1;
		texturedFaceVertexArray[index].position = pointsVector.at(1);
		texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
		texturedFaceVertexArray[index].uv = vec2(0.5f + 0.5 * sin(angle), 0.5f + 0.5 * cos(angle));
		texturedFaceVertexArray[index].normal = normal;

		//Third point
		index += 1;
		texturedFaceVertexArray[index].position = pointsVector.at(2);
		texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
		texturedFaceVertexArray[index].uv = vec2(0.5f + 0.5 * sin(angle + angleStep), 0.5f + 0.5 * cos(angle + angleStep));
		texturedFaceVertexArray[index].normal = normal;

		//Triangle 2 (2-4-5) index 1-3-4
		//Calculating normal
		normal = normalize(cross(pointsVector.at(3) - pointsVector.at(1), pointsVector.at(4) - pointsVector.at(1)));

		//First point
		index += 1;
		texturedFaceVertexArray[index].position = pointsVector.at(1);
		texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
		texturedFaceVertexArray[index].uv = vec2(0.0f, 1.0f); //Top left
		texturedFaceVertexArray[index].normal = normal;

		//Second point
		index += 1;
		texturedFaceVertexArray[index].position = pointsVector.at(3);
		texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
		texturedFaceVertexArray[index].uv = vec2(0.0f, 0.0f); //Bottom left
		texturedFaceVertexArray[index].normal = normal;

		//Third point
		index += 1;
		texturedFaceVertexArray[index].position = pointsVector.at(4);
		texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
		texturedFaceVertexArray[index].uv = vec2(1.0f, 0.0f); //Bottom right
		texturedFaceVertexArray[index].normal = normal;

		//Triangle 3 (2-5-3) index 1-4-2
		//Calculating normal
		normal = normalize(cross(pointsVector.at(4) - pointsVector.at(1), pointsVector.at(2) - pointsVector.at(1)));

		//First point
		index += 1;
		texturedFaceVertexArray[index].position = pointsVector.at(1);
		texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
		texturedFaceVertexArray[index].uv = vec2(0.0f, 1.0f); //Top left
		texturedFaceVertexArray[index].normal = normal;

		//Second point
		index += 1;
		texturedFaceVertexArray[index].position = pointsVector.at(4);
		texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
		texturedFaceVertexArray[index].uv = vec2(0.0f, 1.0f); //Bottom right
		texturedFaceVertexArray[index].normal = normal;

		//Third point
		index += 1;
		texturedFaceVertexArray[index].position = pointsVector.at(2);
		texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
		texturedFaceVertexArray[index].uv = vec2(1.0f, 1.0f); //Top right
		texturedFaceVertexArray[index].normal = normal;

		//Triangle 3 (4-6-5) index 3-5-4
		//Calculating normal
		normal = normalize(cross(pointsVector.at(5) - pointsVector.at(3), pointsVector.at(4) - pointsVector.at(3)));

		//First point
		index += 1;
		texturedFaceVertexArray[index].position = pointsVector.at(3);
		texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
		texturedFaceVertexArray[index].uv = vec2(0.5f + 0.5 * sin(angle), 0.5f + 0.5 * cos(angle)); //Top left
		texturedFaceVertexArray[index].normal = normal;

		//Second point
		index += 1;
		texturedFaceVertexArray[index].position = pointsVector.at(5);
		texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
		texturedFaceVertexArray[index].uv = vec2(0.5f, 0.5f); //Bottom right
		texturedFaceVertexArray[index].normal = normal;

		//Third point
		index += 1;
		texturedFaceVertexArray[index].position = pointsVector.at(4);
		texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
		texturedFaceVertexArray[index].uv = vec2(0.5f + 0.5 * sin(angle + angleStep), 0.5f + 0.5 * cos(angle + angleStep)); //Top right
		texturedFaceVertexArray[index].normal = normal;
	}
}

void updateVertexArray(openiss::OIFace* face, TexturedColoredVertex texturedFaceVertexArray[]) {

	//Setting up our parameters for circle
	float radius = 0.05f;
	int sphereStackTotal = 10;
	int sphereSectorTotal = 20;
	float thetaAngleStep = 360.0f / sphereSectorTotal;
	float phiAngleStep = 180.0f / sphereStackTotal;

	//Number of points for circles
	int firstCylinderIndex = sphereStackTotal * sphereSectorTotal * 72 * 3 * 2;

	//For evcery face point
	for (int facePoint = 0; facePoint < 72; facePoint++) {

		float x = face->facialLandmarks->at(0).at(facePoint).x;
		float y = face->facialLandmarks->at(0).at(facePoint).y;

		vec3 offsetVector = vec3(x, y, 0.0f);

		//For every stack
		for (int stackCount = 0; stackCount < sphereStackTotal; stackCount++) {

			//Calculating phi anngle
			float phi = (90.0f - (phiAngleStep * stackCount));

			//For every sector
			for (int sectorCount = 0; sectorCount < sphereSectorTotal; sectorCount++) {

				float theta = (thetaAngleStep * sectorCount);

				//Calculating 4 corner points of our sector
				//First point
				vec3 p1 = vec3(radius * cos(radians(phi)) * cos(radians(theta)), radius * sin(radians(phi)), radius * cos(radians(phi)) * sin(radians(theta)));

				//Second point, increment theta
				vec3 p2 = vec3(radius * cos(radians(phi)) * cos(radians(theta + thetaAngleStep)), radius * sin(radians(phi)), radius * cos(radians(phi)) * sin(radians(theta + thetaAngleStep)));

				//Third point, increment phi
				vec3 p3 = vec3(radius * cos(radians(phi + phiAngleStep)) * cos(radians(theta)), radius * sin(radians(phi + phiAngleStep)), radius * cos(radians(phi + phiAngleStep)) * sin(radians(theta)));

				//Fourth point, increment both
				vec3 p4 = vec3(radius * cos(radians(phi + phiAngleStep)) * cos(radians(theta + thetaAngleStep)), radius * sin(radians(phi + phiAngleStep)), radius * cos(radians(phi + phiAngleStep)) * sin(radians(theta + thetaAngleStep)));

				//Now we will at thos points to the array in proper order to make two traingles for the sector,
				//incrementing the values by the x and y of the face points to match the specific point in the face

				//First triangle (points 1-3-4)

				//Calculating normal
				vec3 normal = normalize(cross(p3 - p1, p4 - p1));

				//First point
				int index = 6 * sectorCount + 6 * sphereSectorTotal * stackCount + 6 * sphereSectorTotal * sphereStackTotal * facePoint;
				texturedFaceVertexArray[index].position = p1+ offsetVector;
				texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
				texturedFaceVertexArray[index].uv = vec2(0.0f, 1.0f); //Top left corner
				texturedFaceVertexArray[index].normal = normal;
				//Second point
				index += 1;
				texturedFaceVertexArray[index].position = p3+ offsetVector;
				texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
				texturedFaceVertexArray[index].uv = vec2(0.0f, 0.0f); //Bottom left corner
				texturedFaceVertexArray[index].normal = normal;
				//Third point
				index += 1;
				texturedFaceVertexArray[index].position = p4+ offsetVector;
				texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
				texturedFaceVertexArray[index].uv = vec2(1.0f, 0.0f); //Bottom right corner
				texturedFaceVertexArray[index].normal = normal;

				//Second triangle (1-4-2)

				//Calculating normal
				normal = normalize(cross(p4 - p1, p2 - p1));

				//First point
				index += 1;
				texturedFaceVertexArray[index].position = p1 + offsetVector;
				texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
				texturedFaceVertexArray[index].uv = vec2(0.0f, 1.0f); //Top left corner
				texturedFaceVertexArray[index].normal = normal;
				//Second point
				index += 1;
				texturedFaceVertexArray[index].position = p4 + offsetVector;
				texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
				texturedFaceVertexArray[index].uv = vec2(1.0f, 0.0f); //Bottom right corner
				texturedFaceVertexArray[index].normal = normal;
				//Third point
				index += 1;
				texturedFaceVertexArray[index].position = p2 + offsetVector;
				texturedFaceVertexArray[index].color = vec3(1.0f, 0.0f, 0.0f);
				texturedFaceVertexArray[index].uv = vec2(1.0f, 1.0f); //Top right corner
				texturedFaceVertexArray[index].normal = normal;

			}
		}
	}
	//Generate cylinders for links
	//Number of points per cylinder
	int numberOfSlices = 20;
	int indexPerCylinder = numberOfSlices * 4 * 3;
	int currentIndexOffset = firstCylinderIndex;

	//Edge of the face links
	for (int i = 0; i < 18; i++) {
		createLink(face->facialLandmarks->at(0).at(i), face->facialLandmarks->at(0).at(i + 1), currentIndexOffset, texturedFaceVertexArray);
		currentIndexOffset += indexPerCylinder;
	}

	//For first eyebrow
	for (int i = 20; i < 27; i++) {
		createLink(face->facialLandmarks->at(0).at(i), face->facialLandmarks->at(0).at(i + 1), currentIndexOffset, texturedFaceVertexArray);
		currentIndexOffset += indexPerCylinder;
	}
	createLink(face->facialLandmarks->at(0).at(27), face->facialLandmarks->at(0).at(20), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;

	//For second eyebrow
	for (int i = 28; i < 35; i++) {
		createLink(face->facialLandmarks->at(0).at(i), face->facialLandmarks->at(0).at(i + 1), currentIndexOffset, texturedFaceVertexArray);
		currentIndexOffset += indexPerCylinder;
	}
	createLink(face->facialLandmarks->at(0).at(35), face->facialLandmarks->at(0).at(28), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;

	//First eye
	createLink(face->facialLandmarks->at(0).at(37), face->facialLandmarks->at(0).at(38), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	createLink(face->facialLandmarks->at(0).at(38), face->facialLandmarks->at(0).at(41), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	createLink(face->facialLandmarks->at(0).at(41), face->facialLandmarks->at(0).at(40), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	createLink(face->facialLandmarks->at(0).at(40), face->facialLandmarks->at(0).at(37), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;

	//Second eye
	createLink(face->facialLandmarks->at(0).at(45), face->facialLandmarks->at(0).at(46), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	createLink(face->facialLandmarks->at(0).at(46), face->facialLandmarks->at(0).at(49), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	createLink(face->facialLandmarks->at(0).at(49), face->facialLandmarks->at(0).at(48), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	createLink(face->facialLandmarks->at(0).at(48), face->facialLandmarks->at(0).at(45), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;

	//Nose
	for (int i = 52; i < 54; i++) {
		createLink(face->facialLandmarks->at(0).at(i), face->facialLandmarks->at(0).at(i + 1), currentIndexOffset, texturedFaceVertexArray);
		currentIndexOffset += indexPerCylinder;
	}
	createLink(face->facialLandmarks->at(0).at(52), face->facialLandmarks->at(0).at(55), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	for (int i = 55; i < 58; i++) {
		createLink(face->facialLandmarks->at(0).at(i), face->facialLandmarks->at(0).at(i + 1), currentIndexOffset, texturedFaceVertexArray);
		currentIndexOffset += indexPerCylinder;
	}

	//Mouth
	createLink(face->facialLandmarks->at(0).at(59), face->facialLandmarks->at(0).at(61), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	createLink(face->facialLandmarks->at(0).at(61), face->facialLandmarks->at(0).at(67), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	createLink(face->facialLandmarks->at(0).at(67), face->facialLandmarks->at(0).at(69), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	createLink(face->facialLandmarks->at(0).at(69), face->facialLandmarks->at(0).at(68), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	createLink(face->facialLandmarks->at(0).at(68), face->facialLandmarks->at(0).at(64), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	createLink(face->facialLandmarks->at(0).at(64), face->facialLandmarks->at(0).at(60), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	//Second row
	createLink(face->facialLandmarks->at(0).at(59), face->facialLandmarks->at(0).at(62), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	createLink(face->facialLandmarks->at(0).at(62), face->facialLandmarks->at(0).at(70), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	createLink(face->facialLandmarks->at(0).at(70), face->facialLandmarks->at(0).at(65), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	createLink(face->facialLandmarks->at(0).at(65), face->facialLandmarks->at(0).at(60), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	//Third row
	createLink(face->facialLandmarks->at(0).at(59), face->facialLandmarks->at(0).at(63), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	createLink(face->facialLandmarks->at(0).at(63), face->facialLandmarks->at(0).at(71), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	createLink(face->facialLandmarks->at(0).at(71), face->facialLandmarks->at(0).at(66), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
	createLink(face->facialLandmarks->at(0).at(66), face->facialLandmarks->at(0).at(60), currentIndexOffset, texturedFaceVertexArray);
	currentIndexOffset += indexPerCylinder;
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
	GLuint redTextureID = loadTexture("assets/textures/red.jpg");

	// Black background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Compile and link shaders here ...
	string shaderPathPrefix = "assets/shaders/";
	GLuint colorShaderProgram = loadSHADER(shaderPathPrefix + "shader_vertex.glsl", shaderPathPrefix + "shader_fragment.glsl");
	GLuint texturedShaderProgram = loadSHADER(shaderPathPrefix + "textured_vertex.glsl", shaderPathPrefix + "textured_fragment.glsl");
	GLuint shadowShaderProgram = loadSHADER(shaderPathPrefix + "shadow_vertex.glsl", shaderPathPrefix + "shadow_fragment.glsl");

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
	glEnable(GL_DEPTH_TEST);


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
	const int sphereStackCount = 10;
	const int sphereSectorCount = 20;
	const int numberOfLinks = 62;
	const int numberOfSlicesCylinder = 20;

	//Face array size				Pints per triangle	Points per face  Triangle per sector * number of sector per stack * number of stack
	const static int texturedFaceArraySize = 3 * numberOfPointsPerFace *  2 * sphereSectorCount * sphereStackCount
		//Size of cylinders           Number of triangle per slice * points per triangle
		+ numberOfLinks*numberOfSlicesCylinder*4*3;

	//Creating OINullDFaceTracker
	openiss::OINullDFaceTracker OINFT = openiss::OINullDFaceTracker();

	//Getting our first face
	openiss::OIFace* face = OINFT.getNextFace();

	//Creating our vertex array for faces
	static TexturedColoredVertex texturedFaceVertexArray[texturedFaceArraySize];

	//Generating pointers to arrays of vertex

	updateVertexArray(face, texturedFaceVertexArray);

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

	//Initializing matrices and variables to interact with the faceObject
	//These variables are used to modify the above matrices to change the digits according to user input
	float Rotation = 0.0f;
	float Scale = 1;
	float ShearX = 0;
	float ShearZ = 0;
	float faceX = 0;
	float faceZ = 0;
	float faceY = 0;

	//These matrices willl modify our face object
	mat4 rotationMatrix = model;
	mat4 scaleMatrix = model;
	mat4 translationMatrix = model;
	mat4 shearMatrix = model;


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

		//Setting our matrix
		mat4 faceMatrix = translate(mat4(1.0f), vec3(0.0f, 1.0f, -50.0f));


		//Checking if our group needs to be modified
		rotationMatrix = rotate(mat4(1.0f), radians(Rotation), vec3(0.0f, 1.0f, 0.0f));
		scaleMatrix = scale(mat4(1.0f), vec3(Scale, Scale, Scale));
		shearMatrix = { 1.0,0.0,0.0,0.0,
							ShearX,1.0,ShearZ,0.0,
							0.0,0.0,1.0,0.0,
							0.0,0.0,0.0,1.0 };
		translationMatrix = translate(mat4(1.0f), vec3(faceX, faceY, faceZ));
		//Making our combined group matrix
		faceMatrix = faceMatrix * translationMatrix  * shearMatrix * rotationMatrix * scaleMatrix;

		setWorldMatrix(texturedShaderProgram, faceMatrix);
		int currentFace = 0;
		int num = 0;
		//The face will switch every 5 seconds, reset to the first face after 35 seconds
		timeOnFace += dt;
		if (timeOnFace > 2){
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

		//Binding texture
		glBindTexture(GL_TEXTURE_2D, redTextureID);

		//Drawing face
		glDrawArrays(GL_TRIANGLES, 0, texturedFaceArraySize);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);

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


		// GVBN model movement ------------------------------------------------------
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) // move face to the left
		{
			faceX += -0.1;
		}

		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) // move face to the right
		{
			faceX += 0.1;;
		}

		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) // move face back
		{
			faceZ += -0.1;
		}

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) // move face forward
		{
			faceZ += 0.1;
		}

		//UJ move model up and down
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) // move model up
		{
			faceY += 0.1;
		}

		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) // move model down
		{
			faceY += -0.1;;
		}

		//IK scale model up and down and down
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) // scale face up
		{
			Scale += +0.1;
		}

		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) // scale face down
		{
			Scale += -0.1;;
		}

		//OL rotate face Y axis
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) // rotate counter clockwise
		{
			Rotation += 5;
		}

		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // rotate clockwise
		{
			Rotation += -5;;
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

		//Move camera with WASD
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
		{
			cameraPosition -= cameraSideVector * dt * cameraSpeed * 3.0f;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move camera to the right
		{
			cameraPosition += cameraSideVector * dt * cameraSpeed * 3.0f;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move camera forward
		{
			cameraPosition -= cameraLookAt * dt * cameraSpeed * 3.0f;
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move camera back
		{
			cameraPosition += cameraLookAt * dt * cameraSpeed * 3.0f;
		}

		// Toggle textures (Could get rid of)
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
