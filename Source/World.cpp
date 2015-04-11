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
#include "PokemonGenerator.h"

#include <GLFW/glfw3.h>
#include "EventManager.h"
#include "Text2D.h"

using namespace std;
using namespace glm;

World* World::instance;

World::World()
{
    instance = this;
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

int mPrevCamera = 6;

void World::Update(float dt)
{
	if (EventManager::paused == false){ // Not paused
		glfwSetInputMode(EventManager::GetWindow(), GLFW_STICKY_KEYS, GL_TRUE);

		//TODO: Move Camera Manager outside and clean up code.
		//2 part key event: Press & release, On release action happens to make certain we don't repeat keypress multiple times.
		if (EventManager::keyPressed != -1 && glfwGetKey(EventManager::GetWindow(), EventManager::keyPressed) == GLFW_RELEASE
			&& glfwGetMouseButton(EventManager::GetWindow(), EventManager::keyPressed) == GLFW_RELEASE)
		{
			switch (EventManager::keyPressed)
			{
			case GLFW_KEY_1:
				mCurrentCamera = 0;
				std::cout << "Camera Changed to: " << mCurrentCamera << std::endl;
				EventManager::keyPressed = -1; //Reset KeyPressed.
				break;
			case GLFW_KEY_2:
				if (mCamera.size() > 1)
				{
					mCurrentCamera = 1;
					std::cout << "Camera Changed to: " << mCurrentCamera << std::endl;
				}
				EventManager::keyPressed = -1; //Reset KeyPressed.
				break;
			case GLFW_KEY_3:
				if (mCamera.size() > 2)
				{
					mCurrentCamera = 2;
					std::cout << "Camera Changed to: " << mCurrentCamera << std::endl;
				}
				EventManager::keyPressed = -1; //Reset KeyPressed.
				break;
			case GLFW_KEY_4:
				// Character
				if (mCamera.size() > 3)
				{
					mCurrentCamera = 3;
					std::cout << "Camera Changed to: " << mCurrentCamera << std::endl;
				}
				EventManager::keyPressed = -1; //Reset KeyPressed.
				break;
			case GLFW_KEY_5:
				// Spline camera
				if (mCamera.size() > 4 && mModel.size() > 0)
				{
					mCurrentCamera = 4;
					std::cout << "Camera Changed to: " << mCurrentCamera << ": Third Person Model Camera" << std::endl;
				}
				EventManager::keyPressed = -1; //Reset KeyPressed.
				break;
			case GLFW_KEY_GRAVE_ACCENT:
				if (mCamera.size() > 5)
				{
					mCurrentCamera = 5;
					std::cout << "Camera Changed to: " << mCurrentCamera << ": Spline Movement Camera" << std::endl;
				}
				EventManager::keyPressed = -1; //Reset KeyPressed.
				break;
				/*
			case GLFW_KEY_0:
				Renderer::SetShader(SHADER_LIGHTING);
				std::cout << "Shader Changed: SOLID_COLOR" << std::endl;
				EventManager::keyPressed = -1;
				break;
			case GLFW_KEY_9:
				Renderer::SetShader(SHADER_BLUE);
				std::cout << "Shader Changed: SHADER_BLUE" << std::endl;
				EventManager::keyPressed = -1;
				break;
				*/
			case GLFW_MOUSE_BUTTON_LEFT:
				if (mCurrentCamera == 4){
					score += 15;
					EventManager::SaveTGA();
				}
				EventManager::keyPressed = -1;
				break;
			/*case GLFW_MOUSE_BUTTON_RIGHT:
				if (mCurrentCamera == 4){
					score += 15;
					EventManager::SaveTGA();
				}
				EventManager::keyPressed = -1;
				break;*/
			case GLFW_KEY_X:
				mPrevCamera = mCurrentCamera;
				EventManager::paused = true;
				EventManager::mainMenu = true;
				EventManager::keyPressed = -1;
				mCurrentCamera = 6;
				break;
			default:
				break;
			}
		}

		// User Inputs
		// 0 1 2 to change the Camera
		if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_1) == GLFW_PRESS)
		{
			EventManager::keyPressed = GLFW_KEY_1;
		}
		else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_2) == GLFW_PRESS)
		{
			EventManager::keyPressed = GLFW_KEY_2;
		}
		else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_3) == GLFW_PRESS)
		{
			EventManager::keyPressed = GLFW_KEY_3;
		}
		else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_4) == GLFW_PRESS)
		{
			EventManager::keyPressed = GLFW_KEY_4;
		}
		else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_5) == GLFW_PRESS)
		{
			EventManager::keyPressed = GLFW_KEY_5;
		}
		else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)
		{
			EventManager::keyPressed = GLFW_KEY_GRAVE_ACCENT;
		}

		//  left and right mouse
		if (glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			EventManager::keyPressed = GLFW_MOUSE_BUTTON_LEFT;
		}
		else if (glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			EventManager::keyPressed = GLFW_MOUSE_BUTTON_RIGHT;
		}

		// Pause key
		if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_X) == GLFW_PRESS)
		{
			EventManager::keyPressed = GLFW_KEY_X;
		}


		// 0-9 to change the shader
		if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_0) == GLFW_PRESS)
		{
			EventManager::keyPressed = GLFW_KEY_0;
		}
		else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_9) == GLFW_PRESS)
		{
			EventManager::keyPressed = GLFW_KEY_9;
		}

		// Update current Camera
		mCamera[mCurrentCamera]->Update(dt);

		// Update models
		for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
		{
			(*it)->Update(dt);
		}
	}
	// PAUSED 
	else {
		// Update current Camera
		mCamera[mCurrentCamera]->Update(dt);

		// Update models
		for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
		{
			(*it)->Update(dt);
		}


		glfwSetInputMode(EventManager::GetWindow(), GLFW_STICKY_KEYS, GL_FALSE);
		EventManager::EnableMouseCursor();

		
		if (EventManager::mainMenu == true){ // MAIN MENU
			// Check for mouse input on menu
			if (EventManager::keyPressed != -1 && glfwGetMouseButton(EventManager::GetWindow(), EventManager::keyPressed) == GLFW_RELEASE){

				if (EventManager::selected == 1 && EventManager::keyPressed == GLFW_MOUSE_BUTTON_LEFT){
					EventManager::gameStarted = true;
					EventManager::paused = false;
					EventManager::mainMenu = false;
					mCurrentCamera = 4;
					EventManager::keyPressed = -1;
				}
				else if (EventManager::selected == 2 && EventManager::keyPressed == GLFW_MOUSE_BUTTON_LEFT){
					EventManager::options = true;
					EventManager::mainMenu = false;
					EventManager::keyPressed = -1;
				}
				else if (EventManager::selected == 3 && EventManager::keyPressed == GLFW_MOUSE_BUTTON_LEFT){
					EventManager::screenshots = true;
					EventManager::mainMenu = false;
					EventManager::keyPressed = -1;
				}
				else if (EventManager::selected == 4 && EventManager::keyPressed == GLFW_MOUSE_BUTTON_LEFT){
					cout << "QUIT" << endl;
					std::exit(1);
				}
				else{
					EventManager::keyPressed = -1;
				}
			}
		}
		else if (EventManager::options == true){ // OPTIONS MENU
			// Check for mouse input on menu
			if (EventManager::keyPressed != -1 && glfwGetMouseButton(EventManager::GetWindow(), EventManager::keyPressed) == GLFW_RELEASE){

				if (EventManager::selected == 1 && EventManager::keyPressed == GLFW_MOUSE_BUTTON_LEFT){
					if (EventManager::fullscreen == false){
						EventManager::SwitchWindowSize();
					}
					EventManager::keyPressed = -1;
				}
				else if (EventManager::selected == 2 && EventManager::keyPressed == GLFW_MOUSE_BUTTON_LEFT){
					if (EventManager::fullscreen == true){
						EventManager::SwitchWindowSize();
					}
					EventManager::keyPressed = -1;
				}
				else if (EventManager::selected == 3 && EventManager::keyPressed == GLFW_MOUSE_BUTTON_LEFT){
					EventManager::options = false;
					EventManager::mainMenu = true;
					EventManager::keyPressed = -1;
				}
				else{
					EventManager::keyPressed = -1;
				}
			}
		}
		else if (EventManager::screenshots == true){ // SCREENSHOTS MENU
			// Check for mouse input on menu
			if (EventManager::keyPressed != -1 && glfwGetMouseButton(EventManager::GetWindow(), EventManager::keyPressed) == GLFW_RELEASE){

				if (EventManager::selected == 1 && EventManager::keyPressed == GLFW_MOUSE_BUTTON_LEFT){
					EventManager::keyPressed = -1;
				}
				else if (EventManager::selected == 2 && EventManager::keyPressed == GLFW_MOUSE_BUTTON_LEFT){
					EventManager::keyPressed = -1;
				}
				else if (EventManager::selected == 3 && EventManager::keyPressed == GLFW_MOUSE_BUTTON_LEFT){
					EventManager::screenshots = false;
					EventManager::mainMenu = true;
					EventManager::keyPressed = -1;
				}
				else{
					EventManager::keyPressed = -1;
				}
			}
		}
		
		// Mouse click for pause menu
		if (glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			EventManager::keyPressed = GLFW_MOUSE_BUTTON_LEFT;
		}


	} // End of pause





	



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

	// SET THE LIGHT
	setUpLightingShader();

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

	Renderer::SetShader(SHADER_TEXT);
	glUseProgram(Renderer::GetShaderProgramID());
	
	if (EventManager::paused == false && mCurrentCamera == 4){
		// Draw crosshair
		GLuint ColorID = glGetUniformLocation(Renderer::GetShaderProgramID(), "FontColor");
		vec4 color = vec4(1, 1, 1, 0);
		glUniform4f(ColorID, color.x, color.y, color.z, color.w);


		glLineWidth(4.0);
		glBegin(GL_LINES);
		// Bottom left
		glVertex2f(200, 200);
		glVertex2f(200, 150);
		glVertex2f(200, 150);
		glVertex2f(250, 150);

		// Bottom right
		glVertex2f(550, 150);
		glVertex2f(600, 150);
		glVertex2f(600, 150);
		glVertex2f(600, 200);

		// Top right
		glVertex2f(600, 400);
		glVertex2f(600, 450);
		glVertex2f(600, 450);
		glVertex2f(550, 450);

		// Top left
		glVertex2f(250, 450);
		glVertex2f(200, 450);
		glVertex2f(200, 450);
		glVertex2f(200, 400);

		if (EventManager::keyPressed != GLFW_MOUSE_BUTTON_LEFT){
			// Middle crosshair
			glVertex2f(375, 300);
			glVertex2f(385, 300);

			glVertex2f(400, 280);
			glVertex2f(400, 267);

			glVertex2f(415, 300);
			glVertex2f(425, 300);

			glVertex2f(400, 320);
			glVertex2f(400, 333);
		}

		glEnd();
	}


	// DRAW UI 2D TEXTS  (text, x, y, size, color)
	char text[256];
	if (EventManager::gameStarted == true){
		if (EventManager::screenshots == false){
			sprintf(text, "Score: %d", score);
			printText2D(text, 10, 550, 30);
		}

		sprintf(text, "PAUSE: X");
		printText2D(text, 10, 10, 14, vec4(1, 0, 0, 0));
	}

	sprintf(text, "COMP 371");
	printText2D(text, 620, 10, 20);

	if (score >= 100 && EventManager::paused == false){
		sprintf(text, "You Win!");
		printText2D(text, 200, 300, 50);
	}

	// Draw MAIN MENU - PAUSE MENU
	if (EventManager::paused == true){
		if (EventManager::gameStarted == false && EventManager::mainMenu == true){
			DrawMainMenu();
		}
		else if (EventManager::gameStarted == true && EventManager::mainMenu == true){
			DrawPauseMenu();
		}
		else if (EventManager::options == true){
			DrawOptionsMenu();
		}
		else if (EventManager::screenshots == true){
			DrawScreenshotsMenu();
		}
	}

	// Restore previous shader
	Renderer::SetShader((ShaderType) prevShader);

	Renderer::EndFrame();
}

void World::DrawMainMenu(){
	char text[256];
	sprintf(text, "CREDITS:");
	printText2D(text, 10, 190, 25, vec4(-1, -1, -1, 0));
	sprintf(text, "ALEX NEWMAN");
	printText2D(text, 10, 160, 25, vec4(-1, -1, -1, 0));
	sprintf(text, "REZA MADABADI");
	printText2D(text, 10, 130, 25, vec4(-1, -1, -1, 0));
	sprintf(text, "ZHONGHAN ZHOU");
	printText2D(text, 10, 100, 25, vec4(-1, -1, -1, 0));
	sprintf(text, "TAWFEEQ JAWHAR");
	printText2D(text, 10, 70, 25, vec4(-1, -1, -1, 0));
	sprintf(text, "REINA VILLANUEVA");
	printText2D(text, 10, 40, 25, vec4(-1, -1, -1, 0));
	sprintf(text, "SEBOUH BARDAKJIAN");
	printText2D(text, 10, 10, 25, vec4(-1, -1, -1, 0));

	if (EventManager::selected == 1){
		sprintf(text, "START");
		printText2D(text, 310, 400, 32, vec4(1, 0, 0, 0));
	}
	else if (EventManager::selected == 2){
		sprintf(text, "OPTIONS");
		printText2D(text, 280, 350, 32, vec4(1, 0, 0, 0));
	}
	else if (EventManager::selected == 3){
		sprintf(text, "SCREENSHOTS");
		printText2D(text, 218, 300, 32, vec4(1, 0, 0, 0));
	}
	else if (EventManager::selected == 4){
		sprintf(text, "QUIT");
		printText2D(text, 327, 250, 32, vec4(1, 0, 0, 0));
	}

	sprintf(text, "START");
	printText2D(text, 315, 400, 30);
	sprintf(text, "OPTIONS");
	printText2D(text, 285, 350, 30);
	sprintf(text, "SCREENSHOTS");
	printText2D(text, 230, 300, 30);
	sprintf(text, "QUIT");
	printText2D(text, 330, 250, 30);
}

void World::DrawPauseMenu(){
	char text[256];

	if (EventManager::selected == 1){
		sprintf(text, "RESUME");
		printText2D(text, 295, 400, 32, vec4(1, 0, 0, 0));
	}
	else if (EventManager::selected == 2){
		sprintf(text, "OPTIONS");
		printText2D(text, 280, 350, 32, vec4(1, 0, 0, 0));
	}
	else if (EventManager::selected == 3){
		sprintf(text, "SCREENSHOTS");
		printText2D(text, 218, 300, 32, vec4(1, 0, 0, 0));
	}
	else if (EventManager::selected == 4){
		sprintf(text, "QUIT");
		printText2D(text, 327, 250, 32, vec4(1, 0, 0, 0));
	}

	sprintf(text, "RESUME");
	printText2D(text, 300, 400, 30);
	sprintf(text, "OPTIONS");
	printText2D(text, 285, 350, 30);
	sprintf(text, "SCREENSHOTS");
	printText2D(text, 230, 300, 30);
	sprintf(text, "QUIT");
	printText2D(text, 330, 250, 30);
}

void World::DrawOptionsMenu(){
	char text[256];

	if (EventManager::selected == 1){
		sprintf(text, "FULLSCREEN WINDOWED");
		printText2D(text, 100, 400, 32, vec4(1, 0, 0, 0));
	}
	else if (EventManager::selected == 2){
		sprintf(text, "WINDOWED");
		printText2D(text, 268, 350, 32, vec4(1, 0, 0, 0));
	}
	else if (EventManager::selected == 3){
		sprintf(text, "BACK");
		printText2D(text, 327, 250, 32, vec4(1, 0, 0, 0));
	}

	if (EventManager::fullscreen == true){
		sprintf(text, "FULLSCREEN WINDOWED");
		printText2D(text, 120, 400, 30, vec4(0.3, 1, 1, 0));
		sprintf(text, "WINDOWED");
		printText2D(text, 275, 350, 30);
	}
	else {
		sprintf(text, "FULLSCREEN WINDOWED");
		printText2D(text, 120, 400, 30);
		sprintf(text, "WINDOWED");
		printText2D(text, 275, 350, 30, vec4(0.3, 1, 1, 0));
	}
	
	sprintf(text, "BACK");
	printText2D(text, 330, 250, 30);
}

void World::DrawScreenshotsMenu(){
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.2, 0.2, 0.2, 1);
	char text[256];

	if (EventManager::selected == 3){
		sprintf(text, "BACK");
		printText2D(text, 337, 50, 32, vec4(1, 0, 0, 0));
	}
	sprintf(text, "BACK");
	printText2D(text, 340, 50, 30);


	// Draw screenshots
	/*
	GLuint ColorID = glGetUniformLocation(Renderer::GetShaderProgramID(), "FontColor");
	vec4 color = vec4(1, 1, 1, 0);
	glUniform4f(ColorID, color.x, color.y, color.z, color.w);
	*/

	unsigned int prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_TEXT);
	glUseProgram(Renderer::GetShaderProgramID());


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, EventManager::textureId);

	for (int i = 50; i < 751; i = i + 167.5){
		for (int j = 100; j < 551; j = j + 105){
			glBegin(GL_QUADS);

			glTexCoord2f(0, 0);
			glVertex2f(i, j);
			glTexCoord2f(1, 0);
			glVertex2f(i + 167.5, j);
			glTexCoord2f(1, 1);
			glVertex2f(i + 167.5, j + 105);
			glTexCoord2f(0, 1);
			glVertex2f(i, j + 105);
			
			glEnd();

			j += 10;
		}
		i += 10;
	}


	glDisable(GL_TEXTURE_2D);

	// Restore previous shader
	Renderer::SetShader((ShaderType)prevShader);



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
	vector<OBJModel*> pokemon = PokemonGenerator::GeneratePokemon();
	for (int i = 0; i < pokemon.size(); i++)
		mModel.push_back(pokemon[i]);
	srand(20);

	OBJModel* grass = new OBJModel("../Models/Grass_02.obj");
	for (int i = 0; i < 50; i++){

		OBJModel* newgrass = new OBJModel(*grass);
		newgrass->SetPosition(vec3(rand() % 150 - 75, 0, rand() % 150 - 75));
		newgrass->SetScaling(vec3(1, 1, 1));
		mModel.push_back(newgrass);
	}

	
	bool daytime = true;
	const char* groundPath;
	const char* skyboxPath;
	
	if (daytime) {
		groundPath = "../Models/cube.obj";
		skyboxPath = "../Models/ds.obj";
	}
	else {
		groundPath = "../Models/cube2.obj";
		skyboxPath = "../Models/ns.obj";
	}

	OBJModel* ground = new OBJModel(groundPath);
	ground->SetPosition(vec3(0, -2.5, 0));
	ground->SetScaling(vec3(150, 5, 150));
	mModel.push_back(ground);

	OBJModel* skybox = new OBJModel(skyboxPath);
	skybox->SetPosition(vec3(0, 60, 0));
	skybox->SetScaling(vec3(80, 80, 80));
	mModel.push_back(skybox);
	
	
    LoadCameras();
}

void World::LoadCameras()
{
    // Setup Camera
    mCamera.push_back(new StaticCamera(vec3(3.0f, 5.0f, 5.0f),  vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
    mCamera.push_back(new StaticCamera(vec3(3.0f, 74.5f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
    mCamera.push_back(new StaticCamera(vec3(0.5f,  0.5f, 5.0f), vec3(0.0f, 0.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
    
    // Cube Character controlled with Third Person Camera
	OBJModel* character = new OBJModel("../Models/Calem.obj");
	character->SetPosition(vec3(0.0f, 0.0f, 0.0f));
	character->SetScaling(vec3(1.25, 1.25, 1.25));
    mCamera.push_back(new ThirdPersonCamera(character));
    mModel.push_back(character);
    
    // BSpline Camera
	BSpline* spline = new BSpline();
	spline->SetPosition(vec3(0.0, 2.5, 0.0));
	spline->AddControlPoint(vec3(20.0, 0.0, - 50.0));
	spline->AddControlPoint(vec3(40.0, 0.0, - 20.0));
	spline->AddControlPoint(vec3(60.0, 0.0, - 5.0));
	spline->AddControlPoint(vec3(80.0, 0.0, 20.0));
	spline->AddControlPoint(vec3(45.0, 0.0, 20.0));
	spline->AddControlPoint(vec3(25.0, 0.0, 10.0));
	spline->AddControlPoint(vec3(10.0, 0.0, 40.0));
	spline->AddControlPoint(vec3(-50.0, 0.0, 60.0));
	spline->AddControlPoint(vec3(-75.0, 0.0, 40.0));
	spline->AddControlPoint(vec3(-50.0, 0.0, 20.0));
	spline->AddControlPoint(vec3(-75.0, 0.0 ,- 20.0));
	spline->AddControlPoint(vec3(-60.0, 0.0, - 60.0));
	spline->AddControlPoint(vec3(-20.0, 0.0, - 60.0));
	
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
    
	// BSpline MainMeny Camera
	BSpline* splineMainMenu = new BSpline();
	splineMainMenu->SetPosition(vec3(0.0, 30, 0.0));
	splineMainMenu->AddControlPoint(vec3(20.0, 10.0, -10.0));
	splineMainMenu->AddControlPoint(vec3(40.0, - 20.0, 20.0));
	splineMainMenu->AddControlPoint(vec3(-60.0, - 10.0, - 5.0));
	splineMainMenu->AddControlPoint(vec3(20.0, 20.0, 20.0));
	splineMainMenu->AddControlPoint(vec3(-20.0, - 20.0, 40.0));
	splineMainMenu->AddControlPoint(vec3(45.0,40.0, 20.0));
	splineMainMenu->AddControlPoint(vec3(25.0, 0.0, 10.0));
	splineMainMenu->AddControlPoint(vec3(10.0, - 20.0, 40.0));
	splineMainMenu->AddControlPoint(vec3(-50.0, 30.0, 10.0));
	splineMainMenu->AddControlPoint(vec3(-50.0, 0.0, 20.0));
	splineMainMenu->AddControlPoint(vec3(-20.0, 30.0, - 20.0));
	splineMainMenu->AddControlPoint(vec3(30.0, - 10.0, 30.0));
	splineMainMenu->AddControlPoint(vec3(-20.0, 40.0, - 60.0));
		

	if (splineMainMenu == nullptr)
	{
		splineMainMenu = FindSplineByIndex(0);
	}

	if (splineMainMenu != nullptr)
	{
		mCamera.push_back(new BSplineCamera(splineMainMenu, 10.0f));
	}

	//Starting camera (change to the roller coaster for introduction credits)
	mCurrentCamera = 6;

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
	Renderer::SetShader(SHADER_LIGHTING);

	glUseProgram(Renderer::GetShaderProgramID());

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");

	// Send the view projection constants to the shader
	mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	// Code for lighting - just get the View Matrix
	GLuint ViewMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewTransform");
	mat4 V = mCamera[mCurrentCamera]->GetViewMatrix();
	glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, &V[0][0]);

	// Light Position
	GLuint LightPositionID = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldLightPosition");
	vec4 lightPos = vec4(5, 0, 0, 0);		// directional light
	//vec4 lightPos = vec4(5, 0, 0, 1);		// point light
	glUniform4f(LightPositionID, lightPos.x, lightPos.y, lightPos.z, lightPos.w);

	// Light Color
	GLuint LightColorID = glGetUniformLocation(Renderer::GetShaderProgramID(), "LightColor");
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	glUniform3f(LightColorID, lightColor.x, lightColor.y, lightColor.z);

	// Light Attenuation
	GLuint LightAttenuationID = glGetUniformLocation(Renderer::GetShaderProgramID(), "lightAttenuation");
	const float lightKc = 0.0f;
	const float lightKl = 0.0f;
	const float lightKq = 1.0f;
	glUniform3f(LightAttenuationID, lightKc, lightKl, lightKq);

	// Material Coefficients
	GLuint MaterialID = glGetUniformLocation(Renderer::GetShaderProgramID(), "MaterialCoefficients");
	const float ka = 0.2f;
	const float kd = 0.8f;
	const float ks = 0.2f;
	const float n = 150.0f;
	glUniform4f(MaterialID, ka, kd, ks, n);
}

void World::setUpTextureShader(){
	Renderer::SetShader(SHADER_TEXTURE);

	glUseProgram(Renderer::GetShaderProgramID());

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");

	// Send the view projection constants to the shader
	mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	// Code for lighting - just get the View Matrix
	GLuint ViewMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewTransform");
	mat4 V = mCamera[mCurrentCamera]->GetViewMatrix();
	glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, &V[0][0]);

	// Light Position
	GLuint LightPositionID = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldLightPosition");
	vec4 lightPos = vec4(5, 0, 0, 0);		// directional light
	//vec4 lightPos = vec4(5, 0, 0, 1);		// point light
	glUniform4f(LightPositionID, lightPos.x, lightPos.y, lightPos.z, lightPos.w);

	// Light Color
	GLuint LightColorID = glGetUniformLocation(Renderer::GetShaderProgramID(), "LightColor");
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	glUniform3f(LightColorID, lightColor.x, lightColor.y, lightColor.z);

	// Light Attenuation
	GLuint LightAttenuationID = glGetUniformLocation(Renderer::GetShaderProgramID(), "lightAttenuation");
	const float lightKc = 0.0f;
	const float lightKl = 0.0f;
	const float lightKq = 1.0f;
	glUniform3f(LightAttenuationID, lightKc, lightKl, lightKq);

	// Material Coefficients
	GLuint MaterialID = glGetUniformLocation(Renderer::GetShaderProgramID(), "MaterialCoefficients");
	const float ka = 0.2f;
	const float kd = 0.8f;
	const float ks = 0.2f;
	const float n = 150.0f;
	glUniform4f(MaterialID, ka, kd, ks, n);
}