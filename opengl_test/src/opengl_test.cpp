/**
User: Deiskos
Date: Somewhere around july 2015
Linked libraries: -lglut -lGLEW -lGL
Comment:  од чесно скопирован с того самого учебника на wikibooks
          и переделан моими кривыми руками
         ”правление: wasd + rf, q - exit(0)
         —коро перенесу это все с глута на чистые иксы, будет проще
*/

////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include <math.h>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#define _USE_MATH_DEFINES //In the name of Great and Powerful PI
#define M_PI		3.14159265358979323846

//Set directive of preprocessor
//GLEW
#include <GL/glew.h>
//GLUT
#include <GL/glut.h>

#include "utils/shader_utils.h"

#define GLM_FORCE_RADIANS
//GLM - Math library of 3d object
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// GLUT RUNLOOP CALLBACKS
//- Function for processing of screen size change event
void glutCallbackOnWindowResize(int width, int height) {
//	screen_width = width;
//	screen_height = height;
//	glViewport(0, 0, screen_width, screen_height);
}

//- Function for keys processing
void glutCallbackOnKeyDown(unsigned char key, int x, int y) {
//    switch (key) {
//        case 'r':	EyePosition += glm::vec3(0.0, 0.1, 0.0);	break;
//        case 'f':	EyePosition -= glm::vec3(0.0, 0.1, 0.0);	break;
//        case 'w':	EyePosition += glm::vec3(0.0, 0.0, -0.1);	break;
//        case 's':	EyePosition -= glm::vec3(0.0, 0.0, -0.1);	break;
//        case 'a':	EyePosition += glm::vec3(-0.1, 0.0, 0.0);	break;
//        case 'd':	EyePosition -= glm::vec3(-0.1, 0.0, 0.0);	break;
//        case 'q':	exit(0);									break;
//    }
}

//=============================================================================
class Mesh {
private:

	//- State
	//-- DATA
	//--- CPU data buffers
	GLfloat *_coordinatesData;	// Coordinates
	GLfloat *_colorData;		// Color

	GLushort *_indexesData;		// Indexes

	//--- GPU data handles
	GLuint _coordinatesGLID;	// Coordinates
	GLuint _colorGLID;			// Color

	GLuint _indexesGLID;		// Indexes

	//-- CODE
	//--- GPU data handles
	GLuint _vertexShaderGLID;
	GLuint _fragmentShaderGLID;

	GLuint _shaderProgramGLID;

	//- Methods
	//-- Private methods
	GLint getAttributeLocation(GLint inProgram, const char *inAttributeName) {
		GLint theAttributeLocationID =
				glGetAttribLocation(inProgram, inAttributeName);
		if (theAttributeLocationID == -1) {
			fprintf(stderr, "Could not bind attribute%s\n", inAttributeName);
			return -1;
		}
		return theAttributeLocationID;
	}

	GLint getUniformLocation(GLint inProgram, const char *inAttributeName) {
		GLint theAttributeLocationID =
				glGetUniformLocation(inProgram, inAttributeName);
		if (theAttributeLocationID == -1) {
			fprintf(stderr, "Could not bind attribute%s\n", inAttributeName);
			return -1;
		}
		return theAttributeLocationID;
	}

	GLuint createBuffer(GLenum inTarget, const GLvoid *inData,
			GLsizeiptr inSize)
	{
		GLuint theBufferID;
		glGenBuffers(1, &theBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
		glBufferData(GL_ARRAY_BUFFER, inSize, inData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return theBufferID;
	}

	//--- Small helpers
	void setVector(GLfloat *inVectorsBuffer, size_t inVectorIndex,
			GLfloat inX, GLfloat inY, GLfloat inZ)
	{
		inVectorsBuffer[inVectorIndex * 3 + 0] = inX;
		inVectorsBuffer[inVectorIndex * 3 + 1] = inY;
		inVectorsBuffer[inVectorIndex * 3 + 2] = inZ;
	}

	void setIndexBlock(GLushort *inVectorsBuffer, size_t inVectorIndex,
			GLushort in0, GLushort in1, GLushort in2,
			GLushort in3, GLushort in4, GLushort in5)
	{
		inVectorsBuffer[inVectorIndex * 6 + 0] = in0;
		inVectorsBuffer[inVectorIndex * 6 + 1] = in1;
		inVectorsBuffer[inVectorIndex * 6 + 2] = in2;
		inVectorsBuffer[inVectorIndex * 6 + 3] = in3;
		inVectorsBuffer[inVectorIndex * 6 + 4] = in4;
		inVectorsBuffer[inVectorIndex * 6 + 5] = in5;
	}

public:
	Mesh(const char *inModelFile) {
		// Create vertex buffers data
		_coordinatesData = new GLfloat[8 * 3];
		setVector(_coordinatesData, 0, -1.0, -1.0,  1.0);
		setVector(_coordinatesData, 0, 1.0, -1.0,  1.0);
		setVector(_coordinatesData, 0, 1.0,  1.0,  1.0);
		setVector(_coordinatesData, 0, -1.0,  1.0,  1.0);

		setVector(_coordinatesData, 0, -1.0, -1.0, -1.0);
		setVector(_coordinatesData, 0, 1.0, -1.0, -1.0);
		setVector(_coordinatesData, 0, 1.0,  1.0, -1.0);
		setVector(_coordinatesData, 0, -1.0,  1.0, -1.0);

		_colorData = new GLfloat[8 * 3];
		setVector(_colorData, 0, 1.0 , 1.0 , 1.0);
		setVector(_colorData, 0, 0.25, 1.0, 0.25);
		setVector(_colorData, 0, 1.0 , 1.0 , 1.0);
		setVector(_colorData, 0, 1.0, 0.25, 0.25);

		setVector(_colorData, 0, 0.25, 0.25, 0.25);
		setVector(_colorData, 0, 1.0 , 1.0 , 1.0);
		setVector(_colorData, 0, 0.25, 0.25, 0.25);
		setVector(_colorData, 0, 1.0 , 1.0 , 1.0);

		_indexesData = new GLushort[36];
		setIndexBlock(_indexesData, 0, 0, 1, 2, 2, 3, 0);
		setIndexBlock(_indexesData, 1, 3, 2, 6, 6, 7, 3);
		setIndexBlock(_indexesData, 2, 7, 6, 5, 5, 4, 7);
		setIndexBlock(_indexesData, 3, 4, 5, 1, 1, 0, 4);
		setIndexBlock(_indexesData, 4, 4, 0, 3, 3, 7, 4);
		setIndexBlock(_indexesData, 5, 1, 5, 6, 6, 2, 1);

		// Create buffers
		_coordinatesGLID = createBuffer(GL_ARRAY_BUFFER,
				_coordinatesData, 8 * 3 * sizeof(GLfloat));

		_colorGLID = createBuffer(GL_ARRAY_BUFFER,
				_colorData, 8 * 3 * sizeof(GLfloat));

		_indexesGLID = createBuffer(GL_ELEMENT_ARRAY_BUFFER,
				_indexesData, 8 * 3 * sizeof(GLfloat));

		// Create program
		_shaderProgramGLID = createShaderProgram(
				"shaders/triangle.v.glsl", "shaders/triangle.f.glsl",
				_vertexShaderGLID, _fragmentShaderGLID);

		// Get program binding points
		_attribute_coord3d = getAttributeLocation(_materialProgram, "coord3d");
		_attribute_v_color = getAttributeLocation(_materialProgram, "v_color");

		_uniform_fade = getUniformLocation(_materialProgram, "fade");
		_uniform_mvp = getUniformLocation(_materialProgram, "mvp");
	}

	virtual ~Mesh() {
		glDeleteProgram(_materialProgram);
	}

	void draw() {
		// Update MVP matrix in uniform
		glUniformMatrix4fv(_uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

		//-------- DRAWING
		// Bind PROGRAMM
		glUseProgram(_materialProgram);

		//---------------------------------------
		// Bind DATA
		glBindBuffer(GL_ARRAY_BUFFER, _vbo_cube_attrib);

		glEnableVertexAttribArray(_attribute_coord3d);
		glUniform1f(_uniform_fade, 1.0f);

		//--------------------------------------
		//TODO: Use VAO here!
		//Bind ATTRIBUTES INFO (as in VAO)
		glVertexAttribPointer(
			_attribute_coord3d, // attribute
			3,                 // number of elements per vertex, here (x, y, z)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			sizeof(struct attributes),
			0
		);

		glEnableVertexAttribArray(_attribute_v_color);
		glVertexAttribPointer(
			_attribute_v_color, // attribute
			3,                 // number of elements per vertex, here (r,g,b)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			sizeof(struct attributes),
			(GLvoid*) offsetof(struct attributes, v_color)  // offset of the first element
		);

		//------------------- DRAW CALL
		//Bind index array
		glDrawElements(GL_TRIANGLES, (_iboSize/sizeof(GLushort)),
				GL_UNSIGNED_SHORT, 0);

		//------------ UNBIND ATTRIBUTES
		glDisableVertexAttribArray(_attribute_coord3d);
		glDisableVertexAttribArray(_attribute_v_color);
	}
};

//=============================================================================
class Scene {
private:
	//- Types
	//-- Scene data
	struct ViewPortInfo {
		int width;
		int height;
	};

	struct Camera {
		glm::mat4 projection;
		glm::mat4 view;
		glm::vec3 eyePosition = glm::vec3(0.0, 0.0, 0.0);
	};

	//-- Model data
	struct Model {
		Mesh *mesh;

		glm::mat4 coordinateSystem;
		glm::mat4 mvp; // Caching matrix
	};

	//- State

	//-- Frade (alpha) for the Cube
	GLint _uniform_fade;
	GLint _uniform_mvp;


	ViewPortInfo _viewPort;
	Camera _mainCamera;

	std::vector<Mesh *> _meshes;
	std::vector<Model *> _models;

public:
	Scene()
		: _viewPort(), _mainCamera(), _meshes(), _models()
	{
		// Viewport init
		_viewPort.width = 800;
		_viewPort.height = 600;

		// Camera init
		_mainCamera.projection = glm::perspective(
			45.0f,                              		//FOV
			1.0f * _viewPort.width / _viewPort.height,	//Aspect ratio
			0.1f,										//Near
			10.0f										//Far
		);
	}

	void loadMeshFromFile(const char *inMeshName, const char *inFilePath) {
		//TODO: Implement [createModelForMesh] method:
		//1. Load vertex data from mesh file.
		//2. Create mesh object and setup it's info from loaded data.
		//3. .
	}

	void createModelForMesh(const char *inMeshName,
			const glm::mat4 &inCoordinateSystem)
	{
		//TODO: Implement [createModelForMesh] method:
		//1. Find mesh by name.
		//2. Create model, set its mesh from found mesh.
		//3. Set coordiante system for model.
	}

	void draw() {
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


		for (size_t theIndex = 0, theSize = _models.size(); theIndex < theSize;
				++theIndex)
		{
			Model *theModel = _models[theIndex];

			//TODO: Use this data for local coordinate system

//			// ANIMATION MATRIX - MOVE TO MODEL COORDINATES SYSTEM
//			float angle_anim = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (M_PI/4);
//			glm::vec3 axis_z(0, 0, 1);
//			glm::vec3 axis_y(0, 1, 0);
//			glm::vec3 axis_x(1, 0, 0);
//			anim = glm::rotate(glm::mat4(1.0f), angle_anim, axis_x);
//
//			// MODEL MATRIX - SAME AS ANIM MATRIX
//			model = glm::translate(glm::mat4(1.0f), Position);
//			mvp = projection * view * model * anim;

			//TODO: Maybe pass here coordinate system data
			theModel->mesh->draw();
		}

		glutSwapBuffers();
	}
};

//=============================================================================
int main(int argc, char* argv[]) {
	// CREATE WINDOW (USING GLUT)
	glutInit(&argc, argv);
	//	glutInitContextVersion(2,0);

	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Modern OpenGL");

	// INITIALIZE GLEW
	GLenum glew_status = glewInit();
	if(glew_status != GLEW_OK) {
		fprintf(stderr, "Error: %s\n",glewGetErrorString(glew_status));
		return EXIT_FAILURE;
	}
	if (!GLEW_VERSION_2_0) {
		fprintf(stderr,
				"Error: your graphic card does not support OpenGL 2.0\n");
		return 1;
	}

	// SETUP GL CONTEXT
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	//glClearColor(0.0, 0.0, 0.0, 1.0);

	// SETUP GLEW RUNLOOP CALLBACK
	glutDisplayFunc(display);
	glutReshapeFunc(glutCallbackOnWindowResize);
	glutKeyboardFunc(glutCallbackOnKeyDown);

	// START MAIN LOOP
	glutMainLoop();

	return EXIT_SUCCESS;
}
