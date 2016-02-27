#ifndef SHADER_UTILS_H_INCLUDED
#define SHADER_UTILS_H_INCLUDED
#include <GL/glew.h>

char* file_read(const char*);
void print_log(GLuint);
GLuint create_shader(const char*, GLenum);

GLuint createShader(const char*, GLenum);
GLuint createShaderProgram(const char *inVertexShaderFile,
		const char *inFragmentShaderFile,
		GLuint &outVertexShaderID, GLuint &outFragmentShaderID);

////////////////////////////////////////////////////////////////////////////////
#endif // SHADER_UTILS_H
