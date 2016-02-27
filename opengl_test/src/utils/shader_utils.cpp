#include "shader_utils.h"
#include <GL/glew.h>
#include <stdio.h> /*Used for fprintf*/
#include <stdlib.h>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
//Store all the file's contents in memory, useful to pass shaders source code
// to OpenGL
//
//Problem:
//We should close the input file before the return NULL; statements but this
// would lead to a lot of repetition (DRY)-you could solve this by using goto
// or by abusing switch/for/while + break or by building an if else mess better
// solution: let the user handle the File: char* file_read(const FILE* input)
//
char* file_read(const char* filename) {
	FILE* input = fopen(filename, "rb");
	if(input == NULL) return NULL;

	if(fseek(input, 0, SEEK_END) == -1) return NULL;
	long size = ftell(input);
	if(size == -1) return NULL;
	if(fseek(input, 0, SEEK_SET) == -1) return NULL;

	/*if using c-compiler: dont cast malloc's return value*/
	char *content = (char*) malloc( (size_t) size +1  );
	if(content == NULL) return NULL;

	fread(content, 1, (size_t)size, input);
	if(ferror(input)) {
		free(content);
		return NULL;
	}

	fclose(input);
	content[size] = '\0';
	return content;
}

////////////////////////////////////////////////////////////////////////////////
// Display compilation errors from the OpenGL shader compiler
void print_log(GLuint object) {
	GLint log_length = 0;
	if(glIsShader(object)) {
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	} else if(glIsProgram(object)) {
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	} else {
		std::cout << "printlog: Not a shader or a program\n" << std::endl;
		return;
	}

	char* log = (char*)malloc(log_length);

	if(glIsShader(object)) {
		glGetShaderInfoLog(object, log_length, NULL, log);
	} else if(glIsProgram(object)) {
		glGetProgramInfoLog(object, log_length, NULL, log);
	}

	std::cout << log << std::endl;
	free(log);
}

////////////////////////////////////////////////////////////////////////////////
// Compile the shader from file 'filename', with error handling
GLuint createShader(const char* inFilename, GLenum inType) {
	const GLchar* theSource = file_read(inFilename);
	if (theSource == NULL) {
		std::cout << "Error opening file [" << inFilename << "]" << std::endl;
		return 0;
	}

	GLuint res = glCreateShader(inType);
	glShaderSource(res, 1, &theSource, NULL);
	free((void*)theSource);

	glCompileShader(res);
	GLint theCompileStatus = GL_FALSE;
	glGetShaderiv(res, GL_COMPILE_STATUS, &theCompileStatus);
	if (theCompileStatus == GL_FALSE) {
		std::cout << "Error while compiling shader [" << inFilename << "]" <<
				std::endl;
		print_log(res);
		glDeleteShader(res);
		return 0;
	}

	return res;
}

GLuint createShaderProgram(const char *inVertexShaderFile,
		const char *inFragmentShaderFile,
		GLuint &outVertexShaderID, GLuint &outFragmentShaderID)
{
	outVertexShaderID =
			createShader(inVertexShaderFile, GL_VERTEX_SHADER);
	if (outVertexShaderID == 0) return 0;

	outFragmentShaderID =
			createShader(inFragmentShaderFile, GL_FRAGMENT_SHADER);
	if (outFragmentShaderID == 0) return 0;

	GLint theProgramID = glCreateProgram();
	glAttachShader(theProgramID, outVertexShaderID);
	glAttachShader(theProgramID, outFragmentShaderID);
	glLinkProgram(theProgramID);

	GLint theLinkingSuccess = GL_FALSE;
	glGetProgramiv(theProgramID, GL_LINK_STATUS, &theLinkingSuccess);
	if(theLinkingSuccess == GL_FALSE) {
		fprintf(stderr, "Error while linking GLSL program");
		print_log(theProgramID);
		return 0;
	}

	return theProgramID;
}

////////////////////////////////////////////////////////////////////////////////
