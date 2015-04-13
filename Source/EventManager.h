//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once
#include "Renderer.h"
#include "World.h"
#include <GLFW/glfw3.h>

struct GLFWwindow;

class EventManager
{
public:
	static void Initialize();
	static void Shutdown();

	static void Update();

	static float GetFrameTime();
	static bool ExitRequested();

	static GLFWwindow* GetWindow();
	static void SwitchWindowSize();
	static bool fullscreen;

	static float GetMouseMotionX();
	static float GetMouseMotionY();

	static void EnableMouseCursor();
	static void DisableMouseCursor();
	static void window_size_callback(GLFWwindow* window, int width, int height);

	static void SaveTGA(void);

	static bool gameStarted;
	static bool mainMenu;
	static bool paused;
	static bool screenshots;
	static bool options;
	static int keyPressed;
	static int selected;
	
	static int nShot;
	static int m_WindowWidth;
	static int m_WindowHeight;
	static void changeSize(int w, int h);
	static unsigned int nextPowerOf2(unsigned int x);

	static GLuint textureId;

private:
	// Time
	static double sLastFrameTime;
	static float sFrameTime;

	// Mouse
	static double sLastMousePositionX;
	static float  sMouseDeltaX;
	static double sLastMousePositionY;
	static float  sMouseDeltaY;

	// Window
	static GLFWwindow* spWindow;

	
	
};
