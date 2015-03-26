#include "OBJPolygon.h"
#include "Renderer.h"
#include <iostream>

OBJPolygon::OBJPolygon(){}

OBJPolygon::~OBJPolygon(){

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteTextures(1, &textureID);
	glDeleteVertexArrays(1, &VertexArrayID);

	delete material;
}

void OBJPolygon::Initialize(){
	//this assumes all data has already been properly loaded
	glGenVertexArrays(1, &VertexArrayID);
	//glBindVertexArray(VertexArrayID);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	texture = make_texture(material->getTexturePath());
}

//copied directly from
//https://github.com/jckarter/hello-gl/blob/master/hello-gl.c
GLuint OBJPolygon::make_texture(const char *filename)
{
	int width, height;
	void *pixels = read_tga(filename, &width, &height);
	GLuint texture;

	if (!pixels)
		return 0;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(
		GL_TEXTURE_2D, 0,           /* target, level */
		GL_RGB8,                    /* internal format */
		width, height, 0,           /* width, height, border */
		GL_BGR, GL_UNSIGNED_BYTE,   /* external format, type */
		pixels                      /* pixels */
		);
	free(pixels);
	return texture;
}

//copied directly from
//https://github.com/jckarter/hello-gl/blob/master/util.c
void * OBJPolygon::read_tga(const char *filename, int *width, int *height)
{
	struct tga_header {
		char  id_length;
		char  color_map_type;
		char  data_type_code;
		unsigned char  color_map_origin[2];
		unsigned char  color_map_length[2];
		char  color_map_depth;
		unsigned char  x_origin[2];
		unsigned char  y_origin[2];
		unsigned char  width[2];
		unsigned char  height[2];
		char  bits_per_pixel;
		char  image_descriptor;
	} header;
	int i, color_map_size, pixels_size;
	FILE *f;
	size_t read;
	void *pixels;

	f = fopen(filename, "rb");

	if (!f) {
		fprintf(stderr, "Unable to open %s for reading\n", filename);
		return NULL;
	}

	read = fread(&header, 1, sizeof(header), f);

	if (read != sizeof(header)) {
		fprintf(stderr, "%s has incomplete tga header\n", filename);
		fclose(f);
		return NULL;
	}
	if (header.data_type_code != 2) {
		fprintf(stderr, "%s is not an uncompressed RGB tga file\n", filename);
		fclose(f);
		return NULL;
	}
	if (header.bits_per_pixel != 24) {
		fprintf(stderr, "%s is not a 24-bit uncompressed RGB tga file\n", filename);
		fclose(f);
		return NULL;
	}

	for (i = 0; i < header.id_length; ++i)
		if (getc(f) == EOF) {
		fprintf(stderr, "%s has incomplete id string\n", filename);
		fclose(f);
		return NULL;
		}

	color_map_size = le_short(header.color_map_length) * (header.color_map_depth / 8);
	for (i = 0; i < color_map_size; ++i)
		if (getc(f) == EOF) {
		fprintf(stderr, "%s has incomplete color map\n", filename);
		fclose(f);
		return NULL;
		}

	*width = le_short(header.width); *height = le_short(header.height);
	pixels_size = *width * *height * (header.bits_per_pixel / 8);
	pixels = malloc(pixels_size);

	read = fread(pixels, 1, pixels_size, f);
	fclose(f);

	if (read != pixels_size) {
		fprintf(stderr, "%s has incomplete image\n", filename);
		free(pixels);
		return NULL;
	}

	return pixels;
}

short OBJPolygon::le_short(unsigned char *bytes)
{
	return bytes[0] | ((char)bytes[1] << 8);
}