/*
- Sebouh Bardakjian
All texture/ screenshot related tasks
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
#include "textureFunc.h"

using namespace std;

float textureFunc::frameTime = 0;
float textureFunc::lastFrameTime = 0;
int textureFunc::currentTextureNumber = 0;

// Render scene
void textureFunc::display(GLuint &vao) {
	//glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vao);

	//glDrawArrays(GL_TRIANGLES, 0, 12);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//glEnable(GL_PROGRAM_POINT_SIZE);
	//glPointSize(10.0f);

	// Swap front and back buffers
	//glfwSwapBuffers(EventManager::GetWindow());
}

void textureFunc::initialize(GLuint &vao) {
	// Use a Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 1 square (made by 2 triangles) to be rendered
	GLfloat vertices_position[12] = {
		-0.8, -0.6, 0,
		0.8, -0.6, 0,
		0.8, 0.8, 0,
		-0.8, 0.8, 0,
	};

	GLfloat texture_coord[8] = {
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
	};

	GLuint indices[6] = {
		0, 1, 2,
		2, 3, 0,
	};

	// Initialize the model, view and projection matrices
	glm::mat4 Model, View, Projection;

	// Set the projection matrix
	Projection = glm::ortho(-4.0f / 3.0f, 4.0f / 3.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	// Translation
	//Model = glm::translate(Model, glm::vec3(-0.5f, 0.2f, 0.0f));

	// Rotation around Oz with 45 degrees
	//Model = glm::rotate(Model, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));



	// Create a Vector Buffer Object that will store the vertices on video memory
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Allocate space for vertex positions and texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_position) + sizeof(texture_coord), NULL, GL_STATIC_DRAW);

	// Transfer the vertex positions:
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_position), vertices_position);

	// Transfer the texture coordinates:
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices_position), sizeof(texture_coord), texture_coord);

	// Create an Element Array Buffer that will store the indices array:
	GLuint eab;
	glGenBuffers(1, &eab);

	// Transfer the data from indices to eab
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Create a texture
	GLuint texture;
	glGenTextures(1, &texture);

	// Specify that we work with a 2D texture
	glBindTexture(GL_TEXTURE_2D, texture);



	frameTime = glfwGetTime();
	if (frameTime > lastFrameTime + 3){
		lastFrameTime = frameTime;
		currentTextureNumber++;
		if (currentTextureNumber > EventManager::nShot - 1){
			currentTextureNumber = 0;
		}
	}

	const char* name = "screenshot_";
	const char* extension = ".tga";
	string number = std::to_string(currentTextureNumber);
	const char *cstr = number.c_str();

	char* name_with_extension;
	name_with_extension = (char* ) malloc(strlen(name) + strlen(cstr) + 1 + 4); /* make space for the new string (should check the return value ...) */
	strcpy(name_with_extension, name); /* copy name into the new var */
	strcat(name_with_extension, cstr); /* add file number */
	strcat(name_with_extension, extension); /* add the extension */

	load_image(name_with_extension);

	GLuint shaderProgram = create_program("../Source/Shaders/2DTexture.vertexshader", "../Source/Shaders/2Dtexture.fragmentshader");

	// Get the location of the attributes that enters in the vertex shader
	GLint position_attribute = glGetAttribLocation(shaderProgram, "position");

	// Specify how the data for position can be accessed
	glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Enable the attribute
	glEnableVertexAttribArray(position_attribute);

	// Texture coord attribute
	GLint texture_coord_attribute = glGetAttribLocation(shaderProgram, "texture_coord");
	glVertexAttribPointer(texture_coord_attribute, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(vertices_position));
	glEnableVertexAttribArray(texture_coord_attribute);

	// Transfer the transformation matrices to the shader program
	GLint model = glGetUniformLocation(shaderProgram, "Model");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Model));

	GLint view = glGetUniformLocation(shaderProgram, "View");
	glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(View));

	GLint projection = glGetUniformLocation(shaderProgram, "Projection");
	glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(Projection));

}

void textureFunc::initialize2(GLuint &vao) {
	// Use a Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 1 square (made by 2 triangles) to be rendered
	GLfloat vertices_position[12] = {
		0.75, 0.5, 0,
		1.3, 0.5, 0,
		1.3, 1, 0,
		0.75, 1, 0,
	};

	GLfloat texture_coord[8] = {
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
	};

	GLuint indices[6] = {
		0, 1, 2,
		2, 3, 0,
	};

	// Initialize the model, view and projection matrices
	glm::mat4 Model, View, Projection;

	// Set the projection matrix
	Projection = glm::ortho(-4.0f / 3.0f, 4.0f / 3.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	// Translation
	//Model = glm::translate(Model, glm::vec3(-0.5f, 0.2f, 0.0f));

	// Rotation around Oz with 45 degrees
	//Model = glm::rotate(Model, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));



	// Create a Vector Buffer Object that will store the vertices on video memory
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Allocate space for vertex positions and texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_position) + sizeof(texture_coord), NULL, GL_STATIC_DRAW);

	// Transfer the vertex positions:
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_position), vertices_position);

	// Transfer the texture coordinates:
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices_position), sizeof(texture_coord), texture_coord);

	// Create an Element Array Buffer that will store the indices array:
	GLuint eab;
	glGenBuffers(1, &eab);

	// Transfer the data from indices to eab
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Create a texture
	GLuint texture;
	glGenTextures(1, &texture);

	// Specify that we work with a 2D texture
	glBindTexture(GL_TEXTURE_2D, texture);

	const char* name = "screenshot_";
	const char* extension = ".tga";
	string number = std::to_string(EventManager::nShot - 1);
	const char *cstr = number.c_str();

	char* name_with_extension;
	name_with_extension = (char*)malloc(strlen(name) + strlen(cstr) + 1 + 4); /* make space for the new string (should check the return value ...) */
	strcpy(name_with_extension, name); /* copy name into the new var */
	strcat(name_with_extension, cstr); /* add file number */
	strcat(name_with_extension, extension); /* add the extension */

	load_image(name_with_extension);

	GLuint shaderProgram = create_program("../Source/Shaders/2DTexture.vertexshader", "../Source/Shaders/2Dtexture.fragmentshader");

	// Get the location of the attributes that enters in the vertex shader
	GLint position_attribute = glGetAttribLocation(shaderProgram, "position");

	// Specify how the data for position can be accessed
	glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Enable the attribute
	glEnableVertexAttribArray(position_attribute);

	// Texture coord attribute
	GLint texture_coord_attribute = glGetAttribLocation(shaderProgram, "texture_coord");
	glVertexAttribPointer(texture_coord_attribute, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(vertices_position));
	glEnableVertexAttribArray(texture_coord_attribute);

	// Transfer the transformation matrices to the shader program
	GLint model = glGetUniformLocation(shaderProgram, "Model");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Model));

	GLint view = glGetUniformLocation(shaderProgram, "View");
	glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(View));

	GLint projection = glGetUniformLocation(shaderProgram, "Projection");
	glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(Projection));

}

void textureFunc::load_image(const char *fname) {

	// active only for static linking
#ifdef FREEIMAGE_LIB
	FreeImage_Initialise();
#endif

	FIBITMAP *bitmap;
	// Get the format of the image file
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(fname, 0);

	// If the format can't be determined, try to guess the format from the file name
	if (fif == FIF_UNKNOWN) {
		fif = FreeImage_GetFIFFromFilename(fname);
	}

	// Load the data in bitmap if possible
	if (fif != FIF_UNKNOWN && FreeImage_FIFSupportsReading(fif)) {
		bitmap = FreeImage_Load(fif, fname);
	}
	else {
		bitmap = NULL;
	}

	// PROCESS IMAGE if bitmap was successfully initialized
	if (bitmap) {
		unsigned int w = FreeImage_GetWidth(bitmap);
		unsigned int h = FreeImage_GetHeight(bitmap);
		unsigned pixel_size = FreeImage_GetBPP(bitmap);

		// Get a pointer to the pixel data
		BYTE *data = (BYTE*)FreeImage_GetBits(bitmap);

		// Process only RGB and RGBA images
		if (pixel_size == 24) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, (GLvoid*)data);
		}
		else if (pixel_size == 32) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)data);
		}
		else {
			std::cerr << "pixel size = " << pixel_size << " don't know how to process this case. I'm out!" << std::endl;
			exit(-1);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cerr << "Unable to load the image file " << fname << " I'm out!" << std::endl;
		exit(-1);
	}

	// Clean bitmap;
	FreeImage_Unload(bitmap);

	// active only for static linking
#ifdef FREEIMAGE_LIB
	FreeImage_DeInitialise();
#endif	
}




// Read a shader source from a file
// store the shader source in a std::vector<char>
void textureFunc::read_shader_src(const char *fname, std::vector<char> &buffer) {
	std::ifstream in;
	in.open(fname, std::ios::binary);

	if (in.is_open()) {
		// Get the number of bytes stored in this file
		in.seekg(0, std::ios::end);
		size_t length = (size_t)in.tellg();

		// Go to start of the file
		in.seekg(0, std::ios::beg);

		// Read the content of the file in a buffer
		buffer.resize(length + 1);
		in.read(&buffer[0], length);
		in.close();
		// Add a valid C - string end
		buffer[length] = '\0';
	}
	else {
		std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
		exit(-1);
	}
}

// Compile a shader
GLuint textureFunc::load_and_compile_shader(const char *fname, GLenum shaderType) {
	// Load a shader from an external file
	std::vector<char> buffer;
	read_shader_src(fname, buffer);
	const char *src = &buffer[0];

	// Compile the shader
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	// Check the result of the compilation
	GLint test;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
	if (!test) {
		std::cerr << "Shader compilation failed with this message:" << std::endl;
		std::vector<char> compilation_log(512);
		glGetShaderInfoLog(shader, compilation_log.size(), NULL, &compilation_log[0]);
		std::cerr << &compilation_log[0] << std::endl;
		glfwTerminate();
		exit(-1);
	}
	return shader;
}

// Create a program from two shaders
GLuint textureFunc::create_program(const char *path_vert_shader, const char *path_frag_shader) {
	// Load and compile the vertex and fragment shaders
	GLuint vertexShader = load_and_compile_shader(path_vert_shader, GL_VERTEX_SHADER);
	GLuint fragmentShader = load_and_compile_shader(path_frag_shader, GL_FRAGMENT_SHADER);

	// Attach the above shader to a program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Flag the shaders for deletion
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Link and use the program
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	return shaderProgram;
}
