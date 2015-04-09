#include "OBJModel.h"
#include "Renderer.h"

#include <GL/glew.h>
#include <iostream>

using namespace glm;

OBJModel::OBJModel(const char* path): Model(){
	mName = "OBJModel";
	loadOBJ(path);
}

OBJModel::OBJModel(OBJModel & other)
{
	mName = other.mName; // The model name is mainly for debugging
	mPosition = other.mPosition;
	mScaling = other.mScaling;
	mRotationAxis = other.mRotationAxis;
	mRotationAngleInDegrees = other.mRotationAngleInDegrees;

	// Makes the model follow a path defined by a set of waypoints
	mPath = other.mPath;
	mSpline = other.mSpline;
	mSplineParameterT = other.mSplineParameterT;
	mSpeed = other.mSpeed;
	mTargetWaypoint = other.mTargetWaypoint;


	//polygons for obj file
	polygons = other.polygons;
}
OBJModel::~OBJModel(){
}

void OBJModel::loadOBJ(const char* path){
	


	
	bool res = Renderer::LoadOBJ(path, polygons);

	for (int i = 0; i < polygons.size(); i++){
		polygons[i]->Initialize();
	}

	
	if (res == false)
		//WELL FUCK
		std::cout << "WELL SHIT\n";
}

void OBJModel::Update(float dt){
	Model::Update(dt);

}

void OBJModel::Draw()
{
	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);
	

	for (int i = 0; i < polygons.size(); i++)
	{
		glBindVertexArray(polygons[i]->VertexArrayID);
		
		polygons[i]->textureID = glGetUniformLocation(Renderer::GetShaderProgramID(), "myTextureSampler");

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, polygons[i]->texture);
		glUniform1i(polygons[i]->textureID, 0);

		//vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, polygons[i]->vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

		//normals
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, polygons[i]->normalbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		//UVs
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, polygons[i]->uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);
		
		glDrawArrays(GL_TRIANGLES, 0, polygons[i]->vertices.size());


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

	}

}


bool OBJModel::ParseLine(const std::vector<ci_string> &token)
{
	if (token.empty())
	{
		return true;
	}
	else
	{
		return Model::ParseLine(token);
	}
}
