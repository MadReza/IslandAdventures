//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//
#pragma comment( lib, "Glu32.Lib" )

#include "EventManager.h"
#include "Renderer.h"
#include "World.h"

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;


// Time
double EventManager::sLastFrameTime = glfwGetTime();
float  EventManager::sFrameTime = 0.0f;

// Mouse
double EventManager::sLastMousePositionX = 0.0f;
float  EventManager::sMouseDeltaX = 0.0f;
double EventManager::sLastMousePositionY = 0.0f;
float  EventManager::sMouseDeltaY = 0.0f;

// Window
GLFWwindow* EventManager::spWindow = nullptr;
bool EventManager::fullscreen = true;
int EventManager::m_WindowWidth = 0;
int EventManager::m_WindowHeight = 0;

// Event management
bool EventManager::gameStarted = false;
bool EventManager::mainMenu = true;
bool EventManager::paused = true;
bool EventManager::screenshots = false;
bool EventManager::options = false;
int EventManager::keyPressed = -1;
int EventManager::selected = 0;

// Screenshot
int EventManager::nShot = 0;


void EventManager::Initialize()
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		exit(-1);
	}
		
	glfwWindowHint(GLFW_SAMPLES, 4);
    
#if defined(PLATFORM_OSX)
    // OS X would use legacy opengl by default, and wouldn't be able to load shaders
    // This is the proper way to setup GLFW to use modern OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
#else
    // Allow older laptops to run the framework, even though, our framework
    // is compatible with OpenGL 3.3 (which removed many deprecated features)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
    
    
	// Open a window and create its OpenGL context
	glfwWindowHint(GLFW_RESIZABLE, 1);


	
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	EventManager::m_WindowWidth = mode->width - 10;
	EventManager::m_WindowHeight = mode->height - 75;
	

	spWindow = glfwCreateWindow(EventManager::m_WindowWidth, EventManager::m_WindowHeight, "COMP371 - Assignment Framework",  nullptr /*glfwGetPrimaryMonitor() /* For fullscreen, nullptr otherwise */, nullptr);
	glfwSetWindowPos(spWindow, 5, 5);
	glfwSetWindowSizeCallback(spWindow, window_size_callback);
	if (spWindow == nullptr)
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		exit(-1);
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(spWindow, GLFW_STICKY_KEYS, GL_TRUE);

	// Initial mouse position
	glfwPollEvents();
	double x, y;
	glfwGetCursorPos(spWindow, &x, &y);

	sLastMousePositionX = x;
	sLastMousePositionY = y;

	// Initial time
	sLastFrameTime = glfwGetTime();

}

void EventManager::Shutdown()
{
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	spWindow = nullptr;
}

void EventManager::Update()
{
	// Update inputs / events
	glfwPollEvents();

	// Update mouse position
	double x, y;
	glfwGetCursorPos(spWindow, &x, &y);
	sMouseDeltaX = static_cast<float>( x - sLastMousePositionX );
	sMouseDeltaY = static_cast<float>( y - sLastMousePositionY );
	sLastMousePositionX = x;
	sLastMousePositionY = y;

	// Update frame time
	double currentTime = glfwGetTime();
	sFrameTime = static_cast<float>(currentTime - sLastFrameTime);
	sLastFrameTime = currentTime;

	EventManager::selected = 0;
	if (EventManager::paused == true){
		if (EventManager::mainMenu == true){
			// Check hovered menu item
			if (x >= 370 / 1024.0 * EventManager::m_WindowWidth && x <= 630 / 1024.0 * EventManager::m_WindowWidth &&
				y >= 200 / 768.0 * EventManager::m_WindowHeight && y <= 270 / 768.0 * EventManager::m_WindowHeight){
				EventManager::selected = 1;
			}
			else if (x >= 350 / 1024.0 * EventManager::m_WindowWidth && x <= 650 / 1024.0 * EventManager::m_WindowWidth &&
				y >= 271 / 768.0 * EventManager::m_WindowHeight && y <= 335 / 768.0 * EventManager::m_WindowHeight){
				EventManager::selected = 2;
			}
			else if (x >= 280 / 1024.0 * EventManager::m_WindowWidth && x <= 730 / 1024.0 * EventManager::m_WindowWidth &&
				y >= 336 / 768.0 * EventManager::m_WindowHeight && y <= 400 / 768.0 * EventManager::m_WindowHeight){
				EventManager::selected = 3;
			}
			else if (x >= 400 / 1024.0 * EventManager::m_WindowWidth && x <= 600 / 1024.0 * EventManager::m_WindowWidth &&
				y >= 401 / 768.0 * EventManager::m_WindowHeight && y <= 465 / 768.0 * EventManager::m_WindowHeight){
				EventManager::selected = 4;
			}
			else {
				EventManager::selected = 0;
			}
		}
		else if (EventManager::options == true){
			if (x >= 145 / 1024.0 * EventManager::m_WindowWidth && x <= 900 / 1024.0 * EventManager::m_WindowWidth &&
				y >= 200 / 768.0 * EventManager::m_WindowHeight && y <= 270 / 768.0 * EventManager::m_WindowHeight){
				EventManager::selected = 1;
			}
			else if (x >= 350 / 1024.0 * EventManager::m_WindowWidth && x <= 650 / 1024.0 * EventManager::m_WindowWidth &&
				y >= 271 / 768.0 * EventManager::m_WindowHeight && y <= 335 / 768.0 * EventManager::m_WindowHeight){
				EventManager::selected = 2;
			}
			else if (x >= 400 / 1024.0 * EventManager::m_WindowWidth && x <= 600 / 1024.0 * EventManager::m_WindowWidth &&
				y >= 401 / 768.0 * EventManager::m_WindowHeight && y <= 465 / 768.0 * EventManager::m_WindowHeight){
				EventManager::selected = 3;
			}
		}
		else if (EventManager::screenshots == true){
			if (x >= 400 / 1024.0 * EventManager::m_WindowWidth && x <= 600 / 1024.0 * EventManager::m_WindowWidth &&
				y >= 655 / 768.0 * EventManager::m_WindowHeight && y <= 715 / 768.0 * EventManager::m_WindowHeight){
				EventManager::selected = 3;
			}
		}
	}




	
	// FULLSCREEN
	/*
	// Toggle fullscreen flag.
	//fullscreen = !fullscreen;

	//glfwWindowShouldClose(EventManager::GetWindow());
	glfwDestroyWindow(EventManager::GetWindow());

	//glfwWindowHint(GLFW_RESIZABLE, 0);
	//glfwSetWindowSize(spWindow, 1600, 1600);
	GLFWwindow* spWindowCopy = glfwCreateWindow(1024, 768, "COMP371 - Assignment Framework", nullptr/* glfwGetPrimaryMonitor()  For fullscreen, nullptr otherwise , nullptr);
		
		
	//glfwDestroyWindow(spWindow);
	spWindow = spWindowCopy;

	//glfwMakeContextCurrent(spWindow);

	//glfwSwapInterval(1);

	Renderer::Initialize();
	World* world = World::GetInstance();
	world->LoadScene("../Scenes/BSplineScene.scene");
	*/
	


}

float EventManager::GetFrameTime()
{
	return sFrameTime;
}

bool EventManager::ExitRequested()
{
	return glfwGetKey(spWindow, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(spWindow);
}

GLFWwindow* EventManager::GetWindow()
{
	return spWindow;
}

float EventManager::GetMouseMotionX()
{
	return sMouseDeltaX;
}

float EventManager::GetMouseMotionY()
{
	return sMouseDeltaY;
}

void EventManager::EnableMouseCursor()
{
	glfwSetInputMode(spWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void EventManager::DisableMouseCursor()
{
	glfwSetInputMode(spWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void EventManager::SaveTGA(void)
{

	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	char cFileName[64];
	FILE *fScreenshot;

	int nSize = EventManager::m_WindowWidth * EventManager::m_WindowHeight * 3;

	GLubyte *pixels = new GLubyte[nSize];
	if (pixels == NULL) return;

	while (EventManager::nShot < 35)
	{
		sprintf(cFileName, "screenshot_%d.tga", EventManager::nShot);

		fScreenshot = fopen(cFileName, "rb");

		if (fScreenshot == NULL) break;
		else fclose(fScreenshot);

		++EventManager::nShot;

		if (EventManager::nShot > 35)
		{
			// Too many pics taken
			return;
		}
	}

	fScreenshot = fopen(cFileName, "wb");

	glReadPixels(0, 0, EventManager::m_WindowWidth, EventManager::m_WindowHeight, GL_BGR, GL_UNSIGNED_BYTE, pixels);

	unsigned char TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	unsigned char header[6] = { EventManager::m_WindowWidth % 256, EventManager::m_WindowWidth / 256,
		EventManager::m_WindowHeight % 256, EventManager::m_WindowHeight / 256, 24, 0 };

	fwrite(TGAheader, sizeof(unsigned char), 12, fScreenshot);

	fwrite(header, sizeof(unsigned char), 6, fScreenshot);

	fwrite(pixels, sizeof(GLubyte), nSize, fScreenshot);

	fclose(fScreenshot);
	delete[] pixels;

	return;
}


void EventManager::changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45, ratio, 0.1f, 1000.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void EventManager::window_size_callback(GLFWwindow* window, int width, int height)
{
	int minWidth = 800;
	int minHeight = 600;

	if (width >= minWidth && height >= minHeight){
		EventManager::m_WindowWidth = width;
		EventManager::m_WindowHeight = height;
		EventManager::changeSize(width, height);
	}
	else {
		glfwSetWindowSize(spWindow, minWidth, minHeight);
		EventManager::m_WindowWidth = minWidth;
		EventManager::m_WindowHeight = minHeight;
		EventManager::changeSize(minWidth, minHeight);
	}
}

void EventManager::SwitchWindowSize(){
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	EventManager::m_WindowWidth = mode->width - 10;
	EventManager::m_WindowHeight = mode->height - 75;

	// Windowed fullscreen option
	if (EventManager::fullscreen){
		glfwSetWindowSize(spWindow, EventManager::m_WindowWidth / 1.5, EventManager::m_WindowHeight / 1.5);
		EventManager::changeSize(EventManager::m_WindowWidth, EventManager::m_WindowHeight);
		glfwSetWindowPos(spWindow, 10, 35);
		EventManager::fullscreen = false;
	}
	else { // Windowed small screen option
		glfwSetWindowSize(spWindow, EventManager::m_WindowWidth, EventManager::m_WindowHeight);
		EventManager::changeSize(EventManager::m_WindowWidth, EventManager::m_WindowHeight);
		glfwSetWindowPos(spWindow, 5, 5);
		EventManager::fullscreen = true;
	}
}