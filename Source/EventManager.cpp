//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

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
int EventManager::m_WindowWidth = 1024;
int EventManager::m_WindowHeight = 768;

// Event management
bool EventManager::gameStarted = false;
bool EventManager::paused = true;
int EventManager::keyPressed = -1;

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
	glfwWindowHint(GLFW_RESIZABLE, 0);
	spWindow = glfwCreateWindow(1024, 768, "COMP371 - Assignment Framework",   glfwGetPrimaryMonitor() /* For fullscreen, nullptr otherwise */, nullptr);

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
	char cFileName[64];
	FILE *fScreenshot;

	int nSize = 768 * 1024 * 3;


	GLubyte *pixels = new GLubyte[nSize];
	if (pixels == NULL) return;


	//int nShot = 0;

	while (EventManager::nShot < 35)
	{
		sprintf(cFileName, "screenshot_%d.tga", EventManager::nShot);

		fScreenshot = fopen(cFileName, "rb");

		if (fScreenshot == NULL) break;
		else fclose(fScreenshot);

		++EventManager::nShot;

		if (EventManager::nShot > 35)
		{
			// MessageBox(m_WindowHandle,"Screenshot limit of 64 reached. Remove some shots if you want to take more.",  m_AppTitle,MB_OK);
			return;
		}
	}

	fScreenshot = fopen(cFileName, "wb");

	glReadPixels(0, 0, EventManager::m_WindowWidth, EventManager::m_WindowHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	//convert to BGR format    
	unsigned char temp;
	int i = 0;

	while (i < nSize)
	{
		temp = pixels[i];       //grab blue
		pixels[i] = pixels[i + 2];//assign red to blue
		pixels[i + 2] = temp;     //assign blue to red
		i += 3;     //skip to next blue byte
	}

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