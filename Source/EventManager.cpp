//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

/*
- Sebouh Bardakjian
Worked on all event managment, window handling, keypress, menu related stuff, and screenshot capturing
*/

#pragma comment( lib, "Glu32.Lib" )

#include "EventManager.h"
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

// Textures
GLuint EventManager::textureId = 0;

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
			if (x >= 410 / 1024.0 * EventManager::m_WindowWidth && x <= 610 / 1024.0 * EventManager::m_WindowWidth &&
				y >= 655 / 768.0 * EventManager::m_WindowHeight && y <= 715 / 768.0 * EventManager::m_WindowHeight){
				EventManager::selected = 3;
			}
		}
	}

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

	int x, y;
	x = EventManager::m_WindowWidth / 4;
	y = EventManager::m_WindowHeight / 4;

	int nSize = (EventManager::m_WindowWidth-x*2) * (EventManager::m_WindowHeight-y*2) * 3;
	int nSizeTexture = nextPowerOf2(EventManager::m_WindowWidth - x * 2) * nextPowerOf2(EventManager::m_WindowHeight - y * 2) * 3;

	GLubyte *pixels = new GLubyte[nSize];
	GLubyte *pixelsForTexture = new GLubyte[nSizeTexture];

	memset(pixelsForTexture, 0, nSizeTexture);

	if (pixels == NULL) return;

	//while (EventManager::nShot < 32)
	//{
		sprintf(cFileName, "screenshot_%d.tga", EventManager::nShot);
		
		fScreenshot = fopen(cFileName, "rb");

		//if (fScreenshot == NULL) break;
		//else fclose(fScreenshot);

		++EventManager::nShot;

		//if (EventManager::nShot > 32)
		//{
			// Too many pics taken
			//cout << ""
		//	return;
	//}
	fScreenshot = fopen(cFileName, "wb");

	glReadPixels(x, y, (EventManager::m_WindowWidth - x * 2), (EventManager::m_WindowHeight - y * 2), GL_BGR, GL_UNSIGNED_BYTE, pixels);

	// copy pixels into pixelsTexture
	GLubyte* src = pixels;
	GLubyte* dst = pixelsForTexture;

	for (int i = 0; i < (EventManager::m_WindowHeight - y * 2); ++i)
	{
		memcpy(dst, src, (EventManager::m_WindowWidth - x * 2) * 3 * sizeof(GLubyte));
		src += 3 * (EventManager::m_WindowWidth - x * 2);
		dst += 3 * nextPowerOf2(EventManager::m_WindowWidth - x * 2);
	}


	// Upload texture to gpu, keep track of ID

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(
		GL_TEXTURE_2D, 0,           /* target, level */
		GL_RGB8,                    /* internal format */
		nextPowerOf2(EventManager::m_WindowWidth - x * 2), nextPowerOf2(EventManager::m_WindowHeight - y * 2), 0,           /* width, height, border */
		GL_BGR, GL_UNSIGNED_BYTE,   /* external format, type */
		pixelsForTexture                      /* pixels */
		);


	unsigned char TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	unsigned char header[6] = { (EventManager::m_WindowWidth - x * 2) % 256, (EventManager::m_WindowWidth - x * 2) / 256,
		(EventManager::m_WindowHeight - y * 2) % 256, (EventManager::m_WindowHeight - y * 2) / 256, 24, 0 };

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
		glfwSetWindowSize(spWindow, EventManager::m_WindowWidth / 2, EventManager::m_WindowHeight / 2);
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

unsigned int EventManager::nextPowerOf2(unsigned int n)
{
	unsigned count = 0;

	/* First n in the below condition is for the case where n is 0*/
	if (n && !(n&(n - 1)))
		return n;

	while (n != 0)
	{
		n >>= 1;
		count += 1;
	}

	return 1 << count;
}
