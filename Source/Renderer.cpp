//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "Renderer.h"


#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include "Renderer.h"
#include "EventManager.h"

#include <GLFW/glfw3.h>


#if defined(PLATFORM_OSX)
#define fscanf_s fscanf
#endif


std::vector<unsigned int> Renderer::sShaderProgramID;
unsigned int Renderer::sCurrentShader;

GLFWwindow* Renderer::spWindow = nullptr;


void Renderer::Initialize()
{
	spWindow = EventManager::GetWindow();
	glfwMakeContextCurrent(spWindow);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		
		getchar();
		exit(-1);
	}

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	// Enable depth test
    glEnable(GL_DEPTH_TEST);
    
	// Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS); 

	// Line width
	glLineWidth(2);

	// Loading Shaders
#if defined(PLATFORM_OSX)
    std::string shaderPathPrefix = "Shaders/";
#else
    std::string shaderPathPrefix = "../Source/Shaders/";
#endif

	sShaderProgramID.push_back(
                LoadShaders(shaderPathPrefix + "PhongLighting.vertexshader",
                            shaderPathPrefix + "PhongLighting.fragmentshader")
                               );
	sShaderProgramID.push_back(
                LoadShaders(shaderPathPrefix + "PathLines.vertexshader",
                            shaderPathPrefix + "PathLines.fragmentshader")
                               );
	sShaderProgramID.push_back(
                LoadShaders(shaderPathPrefix + "PhongLighting.vertexshader",
                            shaderPathPrefix + "BlueColor.fragmentshader")
                               );
	sShaderProgramID.push_back(
				LoadShaders(shaderPathPrefix + "Textured.vertexshader",
							shaderPathPrefix + "Textured.fragmentshader")
							);
	sShaderProgramID.push_back(
				LoadShaders(shaderPathPrefix + "TextVertexShader.vertexshader",
							shaderPathPrefix + "TextVertexShader.fragmentshader")
							);

	sCurrentShader = 0;

}

void Renderer::Shutdown()
{
		// Shaders
	for (vector<unsigned int>::iterator it = sShaderProgramID.begin(); it < sShaderProgramID.end(); ++it)
	{
		glDeleteProgram(*it);
	}
	sShaderProgramID.clear();


	// Managed by EventManager
	spWindow = nullptr;
}

void Renderer::BeginFrame()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Renderer::EndFrame()
{
	// Swap buffers
	glfwSwapBuffers(spWindow);
}

void Renderer::SetShader(ShaderType type)
{
	if (type < sShaderProgramID.size())
	{
		sCurrentShader = type;
	}
}

//
// The following code is taken from
// www.opengl-tutorial.org
//
GLuint Renderer::LoadShaders(std::string vertex_shader_path,std::string fragment_shader_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_shader_path.c_str());
		getchar();
		exit(-1);
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_shader_path.c_str());
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_shader_path.c_str());
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

//greatly modified OBJ loader from what was here before
//Loads up the .obj file and parses the .mtl first, saving a vector of Material objects
//Then goes through the .obj file and seperates it into seperate Polygon,
//associating it with the appropriate Material
//
//returns a vector of these loaded polygons

bool Renderer::LoadOBJ(const char * path, std::vector<OBJPolygon*> & polygons)
{
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::vector<Material*> materials;
	Material* currentMat;
	bool nextPoly = false;




	FILE * file;
    
#if defined(PLATFORM_OSX)
    file = fopen(path, "r");
#else
    fopen_s(&file, path, "r");
#endif
    
	if (file == nullptr){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
		if (res == EOF){
			//push last polygon
			OBJPolygon* poly = new OBJPolygon();

			for (unsigned int i = 0; i<vertexIndices.size(); i++){

				// Get the indices of its attributes
				unsigned int vertexIndex = vertexIndices[i];
				unsigned int uvIndex = uvIndices[i];
				unsigned int normalIndex = normalIndices[i];

				// Get the attributes thanks to the index
				glm::vec3 vertex = temp_vertices[vertexIndex - 1];
				glm::vec2 uv = temp_uvs[uvIndex - 1];
				glm::vec3 normal = temp_normals[normalIndex - 1];

				// Put the attributes in polygon
				poly->vertices.push_back(vertex);
				poly->uvs.push_back(uv);
				poly->normals.push_back(normal);


			}

			//set material
			poly->material = currentMat;

			//push polygon
			polygons.push_back(poly);

			
			break; // EOF = End Of File. Quit the loop.
		}

		// else : parse lineHeader
		
		
		if (strcmp(lineHeader, "mtllib") == 0){ //load .mtl file
			//get the path for the .mtl file
			char c[128];
			fscanf(file, "%s\n", c);
			char mtlPath[128];
			strcpy(mtlPath, path);
			strcat(mtlPath, "/../");
			strcat(mtlPath, c);
			char * path2 = mtlPath;
			
			LoadMTL(path2, materials);
			
		}else
		if (strcmp(lineHeader, "usemtl") == 0){//switch material
			char c[128];
			fscanf(file, "%s\n", c);
			
			//find material
			for (int i = 0; i < materials.size(); i++)
				if (strcmp(c, materials[i]->getName()) == 0){
					currentMat = materials[i];
					break;
				}
					

		}else
		if ( strcmp( lineHeader, "v" ) == 0 ){
			//v is the first attriubte encountered in a new polygon
			if (nextPoly){
				OBJPolygon* poly = new OBJPolygon();
			
				for (unsigned int i = 0; i<vertexIndices.size(); i++){

					// Get the indices of its attributes
					unsigned int vertexIndex = vertexIndices[i];
					unsigned int uvIndex = uvIndices[i];
					unsigned int normalIndex = normalIndices[i];

					// Get the attributes thanks to the index
					glm::vec3 vertex = temp_vertices[vertexIndex - 1];
					glm::vec2 uv = temp_uvs[uvIndex - 1];
					glm::vec3 normal = temp_normals[normalIndex - 1];

					// Put the attributes in polygon
					poly->vertices.push_back(vertex);
					poly->uvs.push_back(uv);
					poly->normals.push_back(normal);

				}

				//reset variables
				std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
				std::vector<glm::vec3> temp_vertices;
				std::vector<glm::vec2> temp_uvs;
				std::vector<glm::vec3> temp_normals;
				nextPoly = false;

				//set material
				poly->material = currentMat;

				//push polygon
				polygons.push_back(poly);
			
			}
			
			
			
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y );
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "g") == 0){
			nextPoly = true;
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);

		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		
		// Put the attributes in buffers
		//out_vertices.push_back(vertex);
		//out_uvs     .push_back(uv);
		//out_normals .push_back(normal);
	
	}

	return true;
}

bool Renderer::LoadMTL(const char* path, std::vector<Material*> & materials){
	FILE * file;

#if defined(PLATFORM_OSX)
	file = fopen(path, "r");
#else
	fopen_s(&file, path, "r");
#endif

	if (file == nullptr){
		printf("Can't open file. Damn :/");
		getchar();
		return false;
	}

	//temp variable for reading
	vec3 ka;
	vec3 kd;
	vec3 ks;
	char* name = new char();
	char* texturePath = new char();

	int currentMat = -1;

	while (true){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		if (strcmp(lineHeader, "newmtl") == 0){
			//Create new material
			currentMat++;
			name = new char();
			materials.push_back(new Material());

			//retreive name
			fscanf(file, "%s\n", name);
			materials[currentMat]->setName(name);
		}
		else if (strcmp(lineHeader, "Ka") == 0){
			//retreive ambiant level
			fscanf_s(file, "%f %f %f\n", &ka.r, &ka.b, &ka.g);

			//store
			materials[currentMat]->setKa(ka);
		}
		else if (strcmp(lineHeader, "Kd") == 0){
			//retreive diffuse level
			fscanf_s(file, "%f %f %f\n", &kd.r, &kd.g, &kd.b);

			//store
			materials[currentMat]->setKd(kd);
		}
		else if (strcmp(lineHeader, "Ks") == 0){
			//retrieve specular level
			fscanf_s(file, "%f %f %f\n", &ks.r, &ks.g, &ks.b);

			//store
			materials[currentMat]->setKs(ks);
		}
		else if (strcmp(lineHeader, "map_Kd") == 0){
			//retreive texture location
			char c[128];
			fscanf(file, "%s\n", c);
			char d[128] = "../Models/";
			strcat(d, c);
			texturePath = new char[sizeof(d) + 1];

			memcpy(texturePath, &d, sizeof(d) + 1);

			//store
			materials[currentMat]->setTexturePath(texturePath);
		}
		else{
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	return true;
}
