#pragma once

#ifndef ofApp_H
#define ofApp_H

#include "ofMain.h"

#include "Component.h"
#include "C_Transform.h"
#include "C_Camera.h"
#include "C_MeshRenderer.h"
#include "C_Light.h"

#include "Vector3.h"
#include "GameObject.h"
#include "MatrixMath.h"
#include "glm/glm.hpp"
#include "OBJManager.h"
#include "Triangle.h"

#include "Shader.h"
#include "Shader_01.h"
#include "Shader_02.h"
#include "Shader_03.h"


#endif

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		//render pipeline functions
		void renderPipeline();
		void transformToWorldCoordinates();
		void transformToCameraCoordinates();
		void transformToProjectionCoordinates();
		void clipping();
		void transformToViewportCoordinates();
		void rasterize();
		void triangulate();
		void drawTexture();

		//helper functions
		bool pointInTriangle(const glm::vec2& point, const glm::vec2& v0, const glm::vec2& v1, const glm::vec2& v2);
		Vector3 interpolateZCoordinate(const glm::vec2& p, const Vector3* v0, const Vector3* v1, const Vector3* v2);
		Vector3 normalize(float a, float b, float c, float epsilon, int maxIterations = 10000);
		void windowResized(int w, int h);
		Vector3 getDirectionVectorFromEulerAngles(Vector3 rot);

		//screen resolution
		int xRes;
		int yRes;

		//buffer
		std::vector<std::vector<int>> zBuffer;
		std::vector<std::vector<ofColor>> ScreenBuffer;

		//different lists for rendering		
		std::vector<GameObject*> sceneObjects;
		std::vector<GameObject*> renderObjects;

		std::vector<Vertex*> renderVertices;
		std::vector<Triangle> tris;

		//outside tools		
		MatrixMath m;
		OBJManager obj;

		//create game Objects
		GameObject* camera = new GameObject();
		GameObject* cube1 = new GameObject();
		GameObject* light = new GameObject();

};
