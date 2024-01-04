#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	//initialize cube Object
	cube1->addComponent(obj.loadOBJ("models/cube.txt"));
	Component* t1 = cube1->getComponent(Component::TRANSFORM);
	C_Transform* transformCube1 = dynamic_cast<C_Transform*>(t1);
	transformCube1->setPosition(Vector3(0, 0, 0));
	transformCube1->setScale(Vector3(1, 1, 1));
	transformCube1->setRotation(Vector3(0, 0, -90));	

	Component* m1 = cube1->getComponent(Component::MESH_RENDERER);
	C_MeshRenderer* rendererCube1 = dynamic_cast<C_MeshRenderer*>(m1);
	Shader* shader01 = new Shader_01;
	rendererCube1->shader = shader01;

	sceneObjects.push_back(cube1);
	renderObjects.push_back(cube1);

	//initialize camera object
	C_Camera* cameraComponent = new C_Camera();
	camera->addComponent(cameraComponent);
	Component* a = camera->getComponent(Component::TRANSFORM);
	C_Transform* transformCamera = dynamic_cast<C_Transform*>(a);
	transformCamera->setPosition(Vector3(0, 10, 0));
	transformCamera->setRotation(Vector3(270, 180, 0));
	cameraComponent->camDir = getDirectionVectorFromEulerAngles(transformCamera->getRotation());
	
	sceneObjects.push_back(camera);

	//initilaize light object
	C_Light* lightComponent = new C_Light();
	light->addComponent(lightComponent);
	Component* l = light->getComponent(Component::TRANSFORM);
	C_Transform* transformLight = dynamic_cast<C_Transform*>(l);
	transformLight->setPosition(Vector3(0, 0, 0));
	transformLight->setRotation(Vector3(270, 180, 45));
	lightComponent->lightDir = getDirectionVectorFromEulerAngles(transformLight->getRotation());
	sceneObjects.push_back(light);


	windowResized(ofGetWidth(), ofGetHeight());
	ofBackground(255);
}

//--------------------------------------------------------------
void ofApp::update(){
	
	//let cube rotate
	Component* t1 = cube1->getComponent(Component::TRANSFORM);
	C_Transform* transformCube1 = dynamic_cast<C_Transform*>(t1);
	transformCube1->setRotation(Vector3(transformCube1->getRotation().x + 1, 0, transformCube1->getRotation().z + 1));	
}

//--------------------------------------------------------------
void ofApp::draw() {
	//clear screen fo next frame
	ScreenBuffer = std::vector<std::vector<ofColor>>(xRes, std::vector<ofColor>(yRes));
	ScreenBuffer = std::vector<std::vector<ofColor>>(xRes, std::vector<ofColor>(yRes));
	zBuffer = std::vector<std::vector<int>>(xRes, std::vector<int>(yRes));

	//calculate and draw image to screen
	renderPipeline();
	drawTexture();
}

void ofApp::renderPipeline()
{
	transformToWorldCoordinates();
	transformToCameraCoordinates();
	transformToProjectionCoordinates();
	clipping();
	triangulate();
	transformToViewportCoordinates();
	rasterize();
}

//convert from object to world space
void ofApp::transformToWorldCoordinates()
{
	Component* l = light->getComponent(Component::LIGHT);
	C_Light* lightComponent = dynamic_cast<C_Light*>(l);
		
	for each (GameObject* o in renderObjects)
	{
		//fetch all the neccessary components for this stage
		Component* t = o->getComponent(Component::TRANSFORM);
		Component* r = o->getComponent(Component::MESH_RENDERER);

		C_Transform* transformComponent = dynamic_cast<C_Transform*>(t);
		C_MeshRenderer* meshRendererComponent = dynamic_cast<C_MeshRenderer*>(r);

		if (transformComponent && meshRendererComponent)
		{
			//save object space for later shader calculations
			meshRendererComponent->resetTempVertexBuffer();
			std::vector<Vertex*> vBuffer = meshRendererComponent->getVertexBuffer();

			meshRendererComponent->resetTempNormalBuffer();
			std::vector<Vector3*> nBuffer = meshRendererComponent->getNormalBuffer();

			int count = 0;
			for each (Vertex* v in vBuffer)
			{
				//transform each vertex to world space
				Vector3* point = v->getPosition();
				*point = m.translate(transformComponent->getPosition(), *point);
				*point = m.rotate(transformComponent->getRotation(), *point);
				*point = m.scale(transformComponent->getScale(), *point);

				//do the same for the normals
				Vector3* n = nBuffer[count];
				*n = m.rotate(transformComponent->getRotation(), *n);

				//call the assigned vertex shader
				v->Color = meshRendererComponent->shader->vertexShader(point, n, lightComponent->Color, lightComponent->intensity, lightComponent->lightDir);

				count++;
			}
		}
	}
}

//convert from world to camera space following same principle as transformToWorldCoordinates
void ofApp::transformToCameraCoordinates(){
	Component* t = camera->getComponent(Component::TRANSFORM);
	C_Transform* transformComponent = dynamic_cast<C_Transform*>(t);

	if (transformComponent)
	{
		for each (GameObject* o in renderObjects)
		{
			Component* r = o->getComponent(Component::MESH_RENDERER);
			C_MeshRenderer* meshRendererComponent = dynamic_cast<C_MeshRenderer*>(r);

			if (meshRendererComponent)
			{
				std::vector<Vertex*> vBuffer = meshRendererComponent->getVertexBuffer();
				for each (Vertex* v in vBuffer)
				{
					//transform each vertex to camera space
					Vector3* point = v->getPosition();
					*point = m.translate(transformComponent->getReversePosition(), *point);
					*point = m.rotate(transformComponent->getReverseRotation(), *point);
					*point = m.scale(transformComponent->getReverseScale(), *point);
				}
			}
		}
	}
}

//again same step, now to projection coordinates
void ofApp::transformToProjectionCoordinates(){
	Component* c = camera->getComponent(Component::CAMERA);
	C_Camera* cameraComponent = dynamic_cast<C_Camera*>(c);

	if (cameraComponent)
	{
		std::vector<std::vector<float>> projectionMatrix = cameraComponent->projectionMatrix();

		for each (GameObject* o in renderObjects)
		{
			Component* r = o->getComponent(Component::MESH_RENDERER);
			C_MeshRenderer* meshRendererComponent = dynamic_cast<C_MeshRenderer*>(r);

			if (meshRendererComponent)
			{
				std::vector<Vertex*> vBuffer = meshRendererComponent->getVertexBuffer();
				for each (Vertex* v in vBuffer)
				{
					//transform eachvertex to projection space, projecting them to values between -1 and 1
					Vector3* point = v->getPosition();
					std::vector<float> r = m.projectMatrixMultiply(projectionMatrix, *point);
					point->x = r[0] / r[3];
					point->y = r[1] / r[3];
				}
			}
		}
	}
}

//throw out triangles not in the cameras view frsutum to save performance later
void ofApp::clipping() {
	for each (GameObject* o in renderObjects)
	{
		Component* r = o->getComponent(Component::MESH_RENDERER);
		C_MeshRenderer* meshRendererComponent = dynamic_cast<C_MeshRenderer*>(r);

		if (meshRendererComponent)
		{
			meshRendererComponent->clipped = false;
			std::vector<Vertex*> vBuffer = meshRendererComponent->getVertexBuffer();
			for each (Vertex* v in vBuffer)
			{
				Vector3* point = v->getPosition();
				//cheack if it is in the cameras frsutum
				if (point->x > 1 || point->x < -1 || point->y < -1 || point->y > 1 )//|| point->z < -1 || point->z > 1)	
				{
					meshRendererComponent->clipped = true;
				}
			}
			
		}
	}
} 

//prepare for the rasterization by sorting thevertices in triangles
void ofApp::triangulate() {
	tris.clear();

	Component* c = camera->getComponent(Component::CAMERA);
	C_Camera* camComponent = dynamic_cast<C_Camera*>(c);

	for each (GameObject * o in renderObjects)
	{
		Component* r = o->getComponent(Component::MESH_RENDERER);
		C_MeshRenderer* meshRendererComponent = dynamic_cast<C_MeshRenderer*>(r);

		if (meshRendererComponent)
		{
			if (!meshRendererComponent->clipped)		//only draw triangles that are in the cameras view frsutum
			{
				std::vector<Vertex*> vBuffer = meshRendererComponent->getVertexBuffer();
				std::vector<int> iBuffer = meshRendererComponent->getIndexBuffer();
				std::vector<Vector3*> nBuffer = meshRendererComponent->getNormalBuffer();

				//sort vertices into triangles, saving their position and color
				for (int i = 0; i < iBuffer.size(); i += 3)
				{
					Triangle tri;
					tri.pos[0] = vBuffer[iBuffer[i]]->getPosition();
					tri.pos[1] = vBuffer[iBuffer[i + 1]]->getPosition();
					tri.pos[2] = vBuffer[iBuffer[i + 2]]->getPosition();

					tri.col[0] = vBuffer[iBuffer[i]]->Color;
					tri.col[1] = vBuffer[iBuffer[i + 1]]->Color;
					tri.col[2] = vBuffer[iBuffer[i + 2]]->Color;

					tri.meshRenderer = meshRendererComponent;

					//backface culling by simple dot product of camera and triangle normal
					tri.normal = *nBuffer[iBuffer[i]];
					Vector3 camDir = camComponent->camDir;
					float dot = tri.normal.x * -camDir.x + tri.normal.y * -camDir.z + tri.normal.z * -camDir.y; //error
					if (dot >= 0.1)
					{
						tris.push_back(tri);
					}
				}
			}
		}
	}
}

//bring triangle coordinates from 3d to 2d so they can be rendered to the screen
void ofApp::transformToViewportCoordinates() {
	
	std::vector<std::vector<float>> viewportMatrix
	{
		{0.5f * xRes , 0,0, 0.5f * xRes - 0.5f},
		{0, -0.5f * yRes  ,0 ,0.5f * yRes - 0.5f},
		{0,0, 1, 0},
		{0,0,0,1}
	};

	for each (GameObject* o in renderObjects)
	{
		Component* r = o->getComponent(Component::MESH_RENDERER);
		C_MeshRenderer* meshRendererComponent = dynamic_cast<C_MeshRenderer*>(r);

		if (meshRendererComponent)
		{
			if (!meshRendererComponent->clipped)
			{
				std::vector<Vertex*> vBuffer = meshRendererComponent->getVertexBuffer();
				for each (Vertex * v in vBuffer)
				{
					//transform each vertex to viewport coordinates
					Vector3* point = v->getPosition();
					*point = m.matrixVectorMultiply(viewportMatrix, *point);
				}
			}
		}
	}
}

//final step of converting the triangles to actual pixels on the screen
void ofApp::rasterize() {
	Component* c = camera->getComponent(Component::TRANSFORM);
	C_Transform* transformComponent = dynamic_cast<C_Transform*>(c);

	Vector3 pos = transformComponent->getPosition();
	Vector3 rot = transformComponent->getRotation();

	Vector3 dir = getDirectionVectorFromEulerAngles(rot);

	//loop over each triangle
	for each (Triangle t in tris) {
		// Calculate the screen-space bounding box of the triangle
		glm::vec2 minBounds;
		minBounds.x = glm::min(glm::min(t.pos[0]->x, t.pos[1]->x), t.pos[2]->x);
		minBounds.y = glm::min(glm::min(t.pos[0]->y, t.pos[1]->y), t.pos[2]->y);

		glm::vec2 maxBounds;
		maxBounds.x = glm::max(glm::max(t.pos[0]->x, t.pos[1]->x), t.pos[2]->x);
		maxBounds.y = glm::max(glm::max(t.pos[0]->y, t.pos[1]->y), t.pos[2]->y);

		// Iterate only over the pixels inside the bounding box
		for (int i = std::max(0, static_cast<int>(minBounds.x)); i < std::min(xRes, static_cast<int>(maxBounds.x)); i++) {
			for (int j = std::max(0, static_cast<int>(minBounds.y)); j < std::min(yRes, static_cast<int>(maxBounds.y)); j++) {
				glm::vec2 p(i, j);
				glm::vec2 t1;
				glm::vec2 t2;
				glm::vec2 t3;

				t1.x = t.pos[0]->x;
				t1.y = t.pos[0]->y;

				t2.x = t.pos[1]->x;
				t2.y = t.pos[1]->y;

				t3.x = t.pos[2]->x;
				t3.y = t.pos[2]->y;

				//check for each pixel if it overlaps the triangle
				if (pointInTriangle(p, t1, t2, t3))
				{
					//calculate z and color value of each pixel from the corresponding vertices using barycentric ccordinates
					Vector3	bar = interpolateZCoordinate(p, t.pos[0], t.pos[1], t.pos[2]);
					float z_p = bar.x * t.pos[0]->z + bar.y * t.pos[1]->z + bar.z * t.pos[2]->z;
					
					Vector3 v1 = t.col[0];
					Vector3 v2 = t.col[1];
					Vector3 v3 = t.col[2];

					Vector3 vertColor = v1.multiplyFloat(bar.x).addVector(v2.multiplyFloat(bar.y)).addVector(v3.multiplyFloat(bar.z));
					if (vertColor.x > 255) vertColor.x = 255;
					if (vertColor.y > 255) vertColor.y = 255;					
					if (vertColor.z > 255) vertColor.z = 255;

					//check with zBuffer if pixel is seen
					if (z_p > zBuffer[i][j] || zBuffer[i][j] == 0)
					{
						Vector3 col = t.meshRenderer->shader->pixelShader(); 				//call pixelShader
						col.addVector(vertColor);											//combine with vertexColors
						//overwrite the buffers with the pixel values
						ScreenBuffer[i][j] = ofColor(col.x, col.y, col.z);
						zBuffer[i][j] = z_p;
					}
				}
			}
		}
	}
}

//finally draw the calculated textre, represented as a 2d array to the screen using openframeworks drawing tools
//that part is really inefficient and slows the whole program down, as this is not something really intended for real time 3D rendering
//but it was the tool I had at hand and it works fluent for primitive objects nevertheless
void ofApp::drawTexture()
{
	ofMesh mesh;
	mesh.clear();
	mesh.setMode(OF_PRIMITIVE_POINTS);

	//loop over each pixel and draw the correspondning color from the array/texture
#pragma omp parallel for
	for (int i = 0; i < xRes; i++) {
		for (int j = 0; j < yRes; j++) {
			ofColor color = ScreenBuffer[i][j];
			if (color != ofColor(255, 255, 255))
			{
				mesh.addVertex(ofVec3f(i, j, 0));
				mesh.addColor(color);
			}
		}
	}
	mesh.draw();
}

//----------------------------------------------------------------------------------------------
//end of render pipeline and helping functions

//calculation of the barycentric coordinates
Vector3 ofApp::interpolateZCoordinate(const glm::vec2& p, const Vector3* v0, const Vector3* v1, const Vector3* v2) {
	float areaABC = ((v1->y - v2->y) * (v0->x - v2->x) + (v2->x - v0->x) * (v1->y - v2->y)) + (v0->x - v1->x) * (v2->y - v0->y);
	float w0 = ((p.x - v2->x) * (v0->y - v2->y) - (v0->x - v2->x) * (p.y - v2->y)); // areaABC;
	float w1 = ((p.x - v0->x) * (v1->y - v0->y) - (v1->x - v0->x) * (p.y - v0->y)); // areaABC;
	float w2 = ((p.x - v1->x) * (v2->y - v1->y) - (v2->x - v1->x) * (p.y - v1->y)); // areaABC;

	float w = sqrt(pow(w0, 2) + pow(w1, 2) + pow(w2, 2));
	w0 /= w;
	w1 /= w;
	w2 /= w;
	Vector3 r = Vector3(w0 * w0, w1 * w1, w2 * w2);

	/*
	// different, bit more exact approach for doing the normalization
	Vector3 r1 = normalize(w0, w1, w2, 0.000001);
	Vector3 r2 = Vector3(r.x*r.x, r.y*r.y, r.z*r.z);
	*/

	return r;
}

//the other mentioned normalized approach using	the kahan summing algorithm
Vector3 ofApp::normalize(float a, float b, float c, float epsilon, int maxIterations) {
	double length;
	double da = static_cast<double>(a);
	double db = static_cast<double>(b);
	double dc = static_cast<double>(c);
	for (int i = 0; i < maxIterations; ++i) {
		double n[] = { da * da, db * db, dc * dc , 0.0 };
		double lengthSquared = 0.0;
		double over = 0.0;
		for (int i = 0; i < 4; i++)
		{
			double y = n[i] - over;
			double t = lengthSquared + y;
			over = (t - lengthSquared) - y;
			lengthSquared = t;
		}
		length = sqrt(lengthSquared);
		if (length > 0) {
			da /= length;
			db /= length;
			dc /= length;
		}
		if (std::abs(length - 1.0) <= epsilon) {
			return Vector3(static_cast<float>(da), static_cast<float>(db), static_cast<float>(dc));
		}
	}
	// If epsilon is not met after maxIterations, return the current state
	return Vector3(static_cast<float>(da), static_cast<float>(db), static_cast<float>(dc));
}

//check if a point lies in a triangle
bool ofApp::pointInTriangle(const glm::vec2 & point, const glm::vec2 & v0, const glm::vec2 & v1, const glm::vec2 & v2)
{
		float d1 = glm::sign((point.x - v2.x) * (v0.y - v2.y) - (v0.x - v2.x) * (point.y - v2.y));
		float d2 = glm::sign((point.x - v0.x) * (v1.y - v0.y) - (v1.x - v0.x) * (point.y - v0.y));
		float d3 = glm::sign((point.x - v1.x) * (v2.y - v1.y) - (v2.x - v1.x) * (point.y - v1.y));
			
		bool hasNegative = (d1 < 0) || (d2 < 0) || (d3 < 0);
		bool hasPositive = (d1 > 0) || (d2 > 0) || (d3 > 0);

		return !(hasNegative && hasPositive);
}

//get a direction vector from the euler angels used in the transformation step
Vector3 ofApp::getDirectionVectorFromEulerAngles(Vector3 rot) {
	glm::vec3 direction(0.0f, 0.0f, 1.0f);

	glm::mat4 rotationMatrix(1.0f); 
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rot.z), glm::vec3(0.0f, 1.0f, 0.0f)); 
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rot.y), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rot.x), glm::vec3(0.0f, 0.0f, 1.0f));

	direction = glm::vec3(rotationMatrix * glm::vec4(direction, 0.0f));
	Vector3 result;
	float length = sqrt(pow(direction.x, 2) + pow(direction.y, 2) + pow(direction.z, 2));
	result.x = direction.x/length;
	result.y = direction.y/length;
	result.z = direction.z/length;

	return result;
}

//if the window is resized during runtime, change the buffer sizes and resolution variables used e.g for the viewport transformation
void ofApp::windowResized(int w, int h){
	xRes = w;
	yRes = h;

	zBuffer = std::vector<std::vector<int>>(xRes, std::vector<int >(yRes));
	ScreenBuffer = std::vector<std::vector<ofColor>>(xRes, std::vector<ofColor>(yRes));

	Component* c = camera->getComponent(Component::CAMERA);
	C_Camera* cam = dynamic_cast<C_Camera*>(c);

	cam->adjustResolution(w, h);

}
