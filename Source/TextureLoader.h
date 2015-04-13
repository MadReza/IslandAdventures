/*
- Sebouh Bardakjian
.DDS texture loader for font
*/

#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath);


#endif