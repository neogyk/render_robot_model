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

#include <vector>

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

#include <glm/gtx/string_cast.hpp>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
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

	GLuint _dataVAOGLID;		// Data VAO

	GLuint _indexesGLID;		// Indexes

	//-- CODE
	//--- GPU data handles
	GLuint _vertexShaderGLID;
	GLuint _fragmentShaderGLID;

	GLuint _shaderProgramGLID;

	//-- DRAW BINDING
	GLuint _mvpUniformBindingGLID;

	//- Methods
	//-- Private methods
	GLuint getAttributeLocation(GLint inProgram, const char *inAttributeName) {
		GLuint theAttributeLocationID =
				glGetAttribLocation(inProgram, inAttributeName);
		if (glGetError() != GL_NO_ERROR) {
			std::cout << "Could not bind attribute [" << inAttributeName << "]"
					<< std::endl;
			return -1;
		}
		return theAttributeLocationID;
	}

	GLuint getUniformLocation(GLint inProgram, const char *inAttributeName) {
		GLuint theAttributeLocationID =
				glGetUniformLocation(inProgram, inAttributeName);
		if (glGetError() != GL_NO_ERROR) {
			std::cout << "Could not bind attribute [" << inAttributeName << "]"
					<< std::endl;
			return GLuint(-1);
		}
		return theAttributeLocationID;
	}

	GLuint createBuffer(GLenum inTarget, const GLvoid *inData,
			GLsizeiptr inSize)
	{
		GLuint theBufferID;
		glGenBuffers(1, &theBufferID);
		glBindBuffer(inTarget, theBufferID);
		glBufferData(inTarget, inSize, inData, GL_STATIC_DRAW);
		glBindBuffer(inTarget, 0);

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

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// Create vertex buffers data
		_coordinatesData = new GLfloat[8 * 3];
		setVector(_coordinatesData, 0, -1.0, -1.0,  1.0);
		setVector(_coordinatesData, 1, 1.0, -1.0,  1.0);
		setVector(_coordinatesData, 2, 1.0,  1.0,  1.0);
		setVector(_coordinatesData, 3, -1.0,  1.0,  1.0);

		setVector(_coordinatesData, 4, -1.0, -1.0, -1.0);
		setVector(_coordinatesData, 5, 1.0, -1.0, -1.0);
		setVector(_coordinatesData, 6, 1.0,  1.0, -1.0);
		setVector(_coordinatesData, 7, -1.0,  1.0, -1.0);

		_colorData = new GLfloat[8 * 3];
		setVector(_colorData, 0, 1.0 , 1.0 , 1.0);
		setVector(_colorData, 1, 0.25, 1.0, 0.25);
		setVector(_colorData, 2, 1.0 , 1.0 , 1.0);
		setVector(_colorData, 3, 1.0, 0.25, 0.25);

		setVector(_colorData, 4, 0.25, 0.25, 0.25);
		setVector(_colorData, 5, 1.0 , 1.0 , 1.0);
		setVector(_colorData, 6, 0.25, 0.25, 0.25);
		setVector(_colorData, 7, 1.0 , 1.0 , 1.0);

		_indexesData = new GLushort[36];
		setIndexBlock(_indexesData, 0, 0, 1, 2, 2, 3, 0);
		setIndexBlock(_indexesData, 1, 3, 2, 6, 6, 7, 3);
		setIndexBlock(_indexesData, 2, 7, 6, 5, 5, 4, 7);
		setIndexBlock(_indexesData, 3, 4, 5, 1, 1, 0, 4);
		setIndexBlock(_indexesData, 4, 4, 0, 3, 3, 7, 4);
		setIndexBlock(_indexesData, 5, 1, 5, 6, 6, 2, 1);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// Create buffers
		_coordinatesGLID = createBuffer(GL_ARRAY_BUFFER,
				_coordinatesData, 8 * 3 * sizeof(GLfloat));

		_colorGLID = createBuffer(GL_ARRAY_BUFFER,
				_colorData, 8 * 3 * sizeof(GLfloat));

		_indexesGLID = createBuffer(GL_ELEMENT_ARRAY_BUFFER,
				_indexesData, 36 * sizeof(GLushort));

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// Create program
		_shaderProgramGLID = createShaderProgram(
				"shaders/triangle.v.glsl", "shaders/triangle.f.glsl",
				_vertexShaderGLID, _fragmentShaderGLID);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// Generate VAO
		glGenVertexArrays(1, &_dataVAOGLID);
		glBindVertexArray(_dataVAOGLID);

		GLuint theLocation;

		theLocation = getAttributeLocation(_shaderProgramGLID, "coord3d");
		glBindBuffer(GL_ARRAY_BUFFER, _coordinatesGLID);
		glEnableVertexAttribArray(theLocation);
		glVertexAttribPointer(
		/*index */		theLocation,
		/*size */		(GLint)3,
		/*type */		(GLenum)GL_FLOAT,
		/*normalized */	(GLboolean)GL_FALSE,
		/*stride */		(GLsizei)0,
		/*offset */		(const GLvoid*)0
		);

		theLocation = getAttributeLocation(_shaderProgramGLID, "v_color");
		glBindBuffer(GL_ARRAY_BUFFER, _colorGLID);
		glEnableVertexAttribArray(theLocation);
		glVertexAttribPointer(
		/*index */		theLocation,
		/*size */		(GLint)3,
		/*type */		(GLenum)GL_FLOAT,
		/*normalized */	(GLboolean)GL_FALSE,
		/*stride */		(GLsizei)0,
		/*offset */		(const GLvoid*)0
		);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// Create program
		_mvpUniformBindingGLID = getUniformLocation(_shaderProgramGLID, "mvp");
	}

	virtual ~Mesh() {
		glDeleteBuffers(1, &_coordinatesGLID);
		glDeleteBuffers(1, &_colorGLID);

		glDeleteBuffers(1, &_indexesGLID);

		//TODO: Maybe, remove shader program too
		glDeleteProgram(_shaderProgramGLID);

		glDeleteVertexArrays(1, &_dataVAOGLID);
	}

	void draw(const glm::mat4 &inProjection, const glm::mat4 &inView,
			const glm::mat4 &inModel)
	{
		float angle_anim = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (M_PI/4);
		glm::vec3 axis_z(0, 0, 1), axis_y(0, 1, 0), axis_x(1, 0, 0);
		glm::mat4 anim = glm::rotate(glm::mat4(1.0f), angle_anim, axis_x);

		glm::mat4 theMVP = inProjection * inView * inModel * anim;

		glUseProgram(_shaderProgramGLID);

		glUniformMatrix4fv(_mvpUniformBindingGLID, 1, GL_FALSE,
				glm::value_ptr(theMVP));

		glBindVertexArray(_dataVAOGLID);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexesGLID);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

		glBindVertexArray(0);
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
	};

	//-- Model data
	struct Model {
		Mesh *mesh;

		glm::mat4 coordinateSystem;
		glm::mat4 mvp; // Caching matrix
	};

	//- State
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

		glm::vec3 EyePosition = glm::vec3(0.0, 0.0, 0.0);
		_mainCamera.view = glm::lookAt(
			EyePosition,								//eye
			EyePosition + glm::vec3(0.0, 0.0, -1.0),	//center
			glm::vec3(0.0, 1.0, 0.0)					//up
		);
	}

	void loadMeshFromFile(const char *inMeshName, const char *inFilePath) {
		_meshes.push_back(new Mesh(inFilePath));
	}

	void createModelForMesh(const char *inMeshName,
			const glm::mat4 &inCoordinateSystem)
	{
		Model *theModel = new Model();
		theModel->coordinateSystem = inCoordinateSystem;
		theModel->mesh = _meshes[0];

		_models.push_back(theModel);
	}

	void draw() {
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		for (size_t theIndex = 0, theSize = _models.size(); theIndex < theSize;
				++theIndex)
		{
			Model *theModel = _models[theIndex];
			theModel->mesh->draw(_mainCamera.projection, _mainCamera.view,
					theModel->coordinateSystem);
		}

		glutSwapBuffers();
	}
};


////////////////////////////////////////////////////////////////////////////////
Scene *gScene = NULL;

// GLUT RUNLOOP CALLBACKS
//-
void glutOnTimer(int value) {
	glutPostRedisplay();
	glutTimerFunc(12, glutOnTimer, 1);
}

void glutOnDisplay() {
	gScene->draw();
}

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

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
	// Create window
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Modern OpenGL");

	// Initialize GLEW
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

	// Setup GL context
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	gScene = new Scene();
	gScene->loadMeshFromFile("test_mesh", "...");

	double x = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0) / 10.0 * (2*M_PI);
	double y = cosf(glutGet(GLUT_ELAPSED_TIME) / 1000.0) / 10.0 * (2*M_PI);
	gScene->createModelForMesh("test_mesh",
			glm::translate(glm::mat4(1.0f), glm::vec3(x, y, -5)));

	// Setup GLEW runloop callback
	glutDisplayFunc(glutOnDisplay);
	glutReshapeFunc(glutCallbackOnWindowResize);
	glutKeyboardFunc(glutCallbackOnKeyDown);
	glutTimerFunc(12, glutOnTimer, 1);

	// START MAIN LOOP
	glutMainLoop();

	delete gScene;

	return EXIT_SUCCESS;
}
