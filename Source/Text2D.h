#ifndef TEXT2D_HPP
#define TEXT2D_HPP

void initText2D(const char * texturePath);
void printText2D(const char * text, int x, int y, int size, glm::vec4 color = glm::vec4(1, 0, 1, 0));
void cleanupText2D();

#endif