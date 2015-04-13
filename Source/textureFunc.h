/*
- Sebouh Bardakjian
All texture/screenshot related tasks
*/

#pragma once
// Open a window with GLFW
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include  <Dist\x32\FreeImage.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EventManager.h"



class textureFunc{
public:

	// Read a shader source from a file
	// store the shader source in a std::vector<char>
	static void read_shader_src(const char *fname, std::vector<char> &buffer);

	// Compile a shader
	static GLuint load_and_compile_shader(const char *fname, GLenum shaderType);

	// Create a program from two shaders
	static GLuint create_program(const char *path_vert_shader, const char *path_frag_shader);

	// Render scene
	static void display(GLuint &vao);

	// Initialize the data to be rendered
	static void initialize(GLuint &vao);
	static void initialize2(GLuint &vao);

	// Load an image from the disk with FreeImage
	static void load_image(const char *fname);

private:
	static float lastFrameTime;
	static float frameTime;
	static int currentTextureNumber;

};