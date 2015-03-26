//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "World.h"
#include "Renderer.h"
#include "ParsingHelper.h"

#include "StaticCamera.h"
#include "BSplineCamera.h"
#include "ThirdPersonCamera.h"
#include "BSplineCameraThirdPerson.h"
#include "DebugCamera.h"

#include "CubeModel.h"
#include "SphereModel.h"
#include "Path.h"
#include "BSpline.h"
#include "OBJModel.h"

#include <GLFW/glfw3.h>
#include "EventManager.h"
#include "Text2D.h"

using namespace std;
using namespace glm;

World* World::instance;

World::World()
{
    instance = this;
	keyPressed = -1;
	score = 0;
}

World::~World()
{
	// Models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		delete *it;
	}

	mModel.clear();

    // Paths
    for (vector<Path*>::iterator it = mPath.begin(); it < mPath.end(); ++it)
	{
		delete *it;
	}
	mPath.clear();

    // Splines
    for (vector<BSpline*>::iterator it = mSpline.begin(); it < mSpline.end(); ++it)
	{
		delete *it;
	}
	mSpline.clear();

	// Camera
	for (vector<Camera*>::iterator it = mCamera.begin(); it < mCamera.end(); ++it)
	{
		delete *it;
	}
	mCamera.clear();
}

World* World::GetInstance()
{
    return instance;
}

void World::Update(float dt)
{
	//TODO: Move Camera Manager outside and clean up code.
	//2 part key event: Press & release, On release action happens to make certain we don't repeat keypress multiple times.
	if (keyPressed != -1 && glfwGetKey(EventManager::GetWindow(), keyPressed) == GLFW_RELEASE 
		&& glfwGetMouseButton(EventManager::GetWindow(), keyPressed) == GLFW_RELEASE)
	{
		switch (keyPressed)
		{
		case GLFW_KEY_1:
			mCurrentCamera = 0;
			std::cout << "Camera Changed to: " << mCurrentCamera << std::endl;
			keyPressed = -1; //Reset KeyPressed.
			break;
		case GLFW_KEY_2:
			if (mCamera.size() > 1)
			{
				mCurrentCamera = 1;
				std::cout << "Camera Changed to: " << mCurrentCamera << std::endl;
			}
			keyPressed = -1; //Reset KeyPressed.
			break;
		case GLFW_KEY_3:
			if (mCamera.size() > 2)
			{
				mCurrentCamera = 2;
				std::cout << "Camera Changed to: " << mCurrentCamera << std::endl;
			}
			keyPressed = -1; //Reset KeyPressed.
			break;
		case GLFW_KEY_4:
			// Spline camera
			if (mCamera.size() > 3 && mSpline.size() > 0)
			{
				mCurrentCamera = 3;
				std::cout << "Camera Changed to: " << mCurrentCamera << std::endl;
			}
			keyPressed = -1; //Reset KeyPressed.
			break;
		case GLFW_KEY_5:
			// Spline camera
			if (mCamera.size() > 4 && mModel.size() > 0)
			{
				mCurrentCamera = 4;
				std::cout << "Camera Changed to: " << mCurrentCamera << ": Third Person Model Camera" << std::endl;
			}
			keyPressed = -1; //Reset KeyPressed.
			break;
		case GLFW_KEY_GRAVE_ACCENT:
			if (mCamera.size() > 5)
			{
				mCurrentCamera = 5;
				std::cout << "Camera Changed to: " << mCurrentCamera << ": Spline Movement Camera" << std::endl;
			}
			keyPressed = -1; //Reset KeyPressed.
			break;
		case GLFW_KEY_0:
			Renderer::SetShader(SHADER_SOLID_COLOR);
			std::cout << "Shader Changed: SOLID_COLOR" << std::endl;
			keyPressed = -1; 
			break;
		case GLFW_KEY_9:
			Renderer::SetShader(SHADER_BLUE);
			std::cout << "Shader Changed: SHADER_BLUE" << std::endl;
			keyPressed = -1; 
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
		case GLFW_MOUSE_BUTTON_RIGHT:
			++score;
			keyPressed = -1;
			break;
		default:
			break;
		}
	}

	// User Inputs
	// 0 1 2 to change the Camera
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_1) == GLFW_PRESS)
	{
		keyPressed = GLFW_KEY_1;
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_2) == GLFW_PRESS)
	{
		keyPressed = GLFW_KEY_2;
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_3) == GLFW_PRESS)
	{
		keyPressed = GLFW_KEY_3;
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_4) == GLFW_PRESS)
	{
		keyPressed = GLFW_KEY_4;
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_5) == GLFW_PRESS)
	{
		keyPressed = GLFW_KEY_5;
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)
	{	
		keyPressed = GLFW_KEY_GRAVE_ACCENT;
	}

	// Take picture using left or right mouse
	if (glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		keyPressed = GLFW_MOUSE_BUTTON_LEFT;
	}
	else if (glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		keyPressed = GLFW_MOUSE_BUTTON_RIGHT;
	}
	

	// 0-9 to change the shader
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_0) == GLFW_PRESS)
	{
		keyPressed = GLFW_KEY_0;
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_9) == GLFW_PRESS)
	{
		keyPressed = GLFW_KEY_9;
	}

	// Update current Camera
	mCamera[mCurrentCamera]->Update(dt);

	// Update models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		(*it)->Update(dt);
	}
}

void World::Draw()
{
	Renderer::BeginFrame();
	
	// Set shader to use
	glUseProgram(Renderer::GetShaderProgramID());

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform"); 

	// Send the view projection constants to the shader
	mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	// Code for lighting - get just the View Matrix
	GLuint ViewMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewTransform");
	mat4 V = mCamera[mCurrentCamera]->GetViewMatrix();
	glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, &V[0][0]);

	// Code for lighting - set the light
	GLuint LightID = glGetUniformLocation(Renderer::GetShaderProgramID(), "LightPosition_worldspace");
	glm::vec3 lightPos = glm::vec3(40,4,4);
    glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
	
	// Draw models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		// Draw model

		//check if it's a textured object
		if (dynamic_cast<OBJModel*>((*it)) != 0){
			setUpTextureShader();
			(*it)->Draw();
			setUpLightingShader();
		}else
			(*it)->Draw();
	}

	// Draw Path Lines
	
	// Set Shader for path lines
	unsigned int prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_PATH_LINES);
	glUseProgram(Renderer::GetShaderProgramID());

	// Send the view projection constants to the shader
	VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	for (vector<Path*>::iterator it = mPath.begin(); it < mPath.end(); ++it)
	{
		// Draw model
		(*it)->Draw();
	}

    // Draw B-Spline Lines (using the same shader for Path Lines)
    for (vector<BSpline*>::iterator it = mSpline.begin(); it < mSpline.end(); ++it)
	{
		// Draw model
		(*it)->Draw();
	}




	// DRAW UI 2D TEXTS 
	// text, x, y, size
	char text[256];
	sprintf(text, "Score: %d", score);
	printText2D(text, 10, 550, 30);

	sprintf(text, "COMP 371");
	printText2D(text, 620, 10, 20);

	if (score >= 15){
		sprintf(text, "You Win!");
		printText2D(text, 200, 300, 50);
	}

	// Restore previous shader
	Renderer::SetShader((ShaderType) prevShader);

	Renderer::EndFrame();
	
}

void World::LoadScene(const char * scene_path)
{
	// Using case-insensitive strings and streams for easier parsing
	ci_ifstream input;
	input.open(scene_path, ios::in);

	// Invalid file
	if(input.fail() )
	{	 
		fprintf(stderr, "Error loading file: %s\n", scene_path);
		getchar();
		exit(-1);
	}

	ci_string item;
	while( std::getline( input, item, '[' ) )   
	{
        ci_istringstream iss( item );

		ci_string result;
		if( std::getline( iss, result, ']') )
		{
			if( result == "cube" )
			{
				// Box attributes
				CubeModel* cube = new CubeModel();
				cube->Load(iss);
				mModel.push_back(cube);
			}
            else if( result == "sphere" )
            {
                SphereModel* sphere = new SphereModel();
                sphere->Load(iss);
                mModel.push_back(sphere);
            }
            else if( result == "path" )
			{
				Path* path = new Path();
				path->Load(iss);
                mPath.push_back(path);
			}
            else if( result == "spline" )
			{
				BSpline* path = new BSpline();
				path->Load(iss);
                mSpline.push_back(path);
			}
			else if ( result.empty() == false && result[0] == '#')
			{
				// this is a comment line
			}
			else
			{
				fprintf(stderr, "Error loading scene file... !");
				getchar();
				exit(-1);
			}
	    }
	}
	input.close();

	// Set PATH vertex buffers
	for (vector<Path*>::iterator it = mPath.begin(); it < mPath.end(); ++it)
	{
		// Draw model
		(*it)->CreateVertexBuffer();
	}

    // Set B-SPLINE vertex buffers
    for (vector<BSpline*>::iterator it = mSpline.begin(); it < mSpline.end(); ++it)
	{
		// Draw model
		(*it)->CreateVertexBuffer();
	}

	//LOAD DAT OBJ MODEL YO
	OBJModel* pika = new OBJModel("../Models/Pikachu.obj");
	pika->SetPosition(vec3(1,1,1));
	mModel.push_back(pika);

    LoadCameras();
}

void World::LoadCameras()
{
    // Setup Camera
    mCamera.push_back(new StaticCamera(vec3(3.0f, 5.0f, 5.0f),  vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
    mCamera.push_back(new StaticCamera(vec3(3.0f, 74.5f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
    mCamera.push_back(new StaticCamera(vec3(0.5f,  0.5f, 5.0f), vec3(0.0f, 0.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
    
    // Cube Character controlled with Third Person Camera
    CubeModel* character = new CubeModel();
    character->SetPosition(vec3(0.0f, 0.5f, 0.0f));
    mCamera.push_back(new ThirdPersonCamera(character));
    mModel.push_back(character);
    
    // BSpline Camera
    BSpline* spline = FindSpline("\"RollerCoaster\"");
    if (spline == nullptr)
    {
        spline = FindSplineByIndex(0);
    }
    
    if (spline != nullptr)
    {
		mCamera.push_back(new BSplineCameraThirdPerson(spline, 5.0f));
    }

	//debug camera
	mCamera.push_back(new DebugCamera(vec3(0.0f, 2.0f, 0.0f)));
    
    mCurrentCamera = 0;
}

Path* World::FindPath(ci_string pathName)
{
    for(std::vector<Path*>::iterator it = mPath.begin(); it < mPath.end(); ++it)
    {
        if((*it)->GetName() == pathName)
        {
            return *it;
        }
    }
    return nullptr;
}

BSpline* World::FindSpline(ci_string pathName)
{
    for(std::vector<BSpline*>::iterator it = mSpline.begin(); it < mSpline.end(); ++it)
    {
        if((*it)->GetName() == pathName)
        {
            return *it;
        }
    }
    return nullptr;
}

BSpline* World::FindSplineByIndex(unsigned int index)
{
    return mSpline.size() > 0 ? mSpline[index % mSpline.size()] : nullptr;
}

Model* World::FindModelByIndex(unsigned int index)
{
    return mModel.size() > 0 ? mModel[index % mModel.size()] : nullptr;
}

void World::setUpLightingShader(){
	Renderer::SetShader(SHADER_SOLID_COLOR);
	
	glUseProgram(Renderer::GetShaderProgramID());

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");

	// Send the view projection constants to the shader
	mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	// Code for lighting - get just the View Matrix
	GLuint ViewMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewTransform");
	mat4 V = mCamera[mCurrentCamera]->GetViewMatrix();
	glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, &V[0][0]);

	// Code for lighting - set the light
	GLuint LightID = glGetUniformLocation(Renderer::GetShaderProgramID(), "LightPosition_worldspace");
	glm::vec3 lightPos = glm::vec3(40, 4, 4);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
}

void World::setUpTextureShader(){
	Renderer::SetShader(SHADER_TEXTURE);
	
	glUseProgram(Renderer::GetShaderProgramID());

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");

	// Send the view projection constants to the shader
	mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	// Code for lighting - get just the View Matrix
	GLuint ViewMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewTransform");
	mat4 V = mCamera[mCurrentCamera]->GetViewMatrix();
	glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, &V[0][0]);

	// Code for lighting - set the light
	GLuint LightID = glGetUniformLocation(Renderer::GetShaderProgramID(), "LightPosition_worldspace");
	glm::vec3 lightPos = glm::vec3(40, 4, 4);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
}