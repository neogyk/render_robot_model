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
// GL Utils
//TODO: Place utils near "createShader" method and so on
////////////////////////////////////////////////////////////////////////////////
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

GLuint createBuffer(GLenum inTarget, const GLvoid *inData, GLsizeiptr inSize) {
	GLuint theBufferID;
	glGenBuffers(1, &theBufferID);
	glBindBuffer(inTarget, theBufferID);
	glBufferData(inTarget, inSize, inData, GL_STATIC_DRAW);
	glBindBuffer(inTarget, 0);

	return theBufferID;
}

////////////////////////////////////////////////////////////////////////////////
//Maybe usefull for coordinate system drawer
//// Create coordinates buffer
//GLfloat *theCoordinates = new GLfloat[9];
//
//glm::vec4 theVector;
//
//theVector = _matrix * glm::vec4(1.f, 0.f, 0.f, 1.f);
//theCoordinates[0] = theVector.x;
//theCoordinates[1] = theVector.y;
//theCoordinates[2] = theVector.z;
//
//theVector = _matrix * glm::vec4(0.f, 1.f, 0.f, 1.f);
//theCoordinates[3] = theVector.x;
//theCoordinates[4] = theVector.y;
//theCoordinates[5] = theVector.z;
//
//theVector = _matrix * glm::vec4(0.f, 0.f, 1.f, 1.f);
//theCoordinates[6] = theVector.x;
//theCoordinates[7] = theVector.y;
//theCoordinates[8] = theVector.z;
//
//// Create colors buffer
//GLfloat *theColors = new GLfloat[9];
//theColors[0] = 1.f;
//theColors[1] = 0.f;
//theColors[2] = 0.f;

////////////////////////////////////////////////////////////////////////////////
class LineServer {
private:
	//- State
	//-- DATA
	//--- GPU data handles
	GLuint _coordinatesGLID;	// Coordinates
	GLuint _colorGLID;			// Color

	GLuint _dataVAOGLID;		// Data VAO

	//-- CODE
	//--- GPU data handles
	GLuint _vertexShaderGLID;
	GLuint _fragmentShaderGLID;

	GLuint _shaderProgramGLID;

	//-- DRAW BINDING
	GLuint _mvpUniformBindingGLID;

	//- Methods
	void internal_setMatrix(const glm::mat4 &inMatrix) {
	}

public:
	//- Methods
	//-- Memory lifecycle
	LineServer() {
//		_name = inMesh->mName.data;
//
//		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//		// Create vertex buffers data
//		_vertexCount = inMesh->mNumVertices;
//		aiVector3D *theVertexes = inMesh->mVertices;
//
//		_coordinatesData = new GLfloat[_vertexCount * 3];
//		_colorData = new GLfloat[_vertexCount * 3];
//
//		for(unsigned int i = 0; i < _vertexCount; ++i) {
//			_coordinatesData[i*3 + 0] = theVertexes[i].x;
//			_coordinatesData[i*3 + 1] = theVertexes[i].y;
//			_coordinatesData[i*3 + 2] = theVertexes[i].z;
//
//			_colorData[i*3 + 0] = ((float)rand()) / RAND_MAX;
//			_colorData[i*3 + 1] = ((float)rand()) / RAND_MAX;
//			_colorData[i*3 + 2] = ((float)rand()) / RAND_MAX;
//		}
//
//		// Create index array
//		unsigned int theFacesCount = inMesh->mNumFaces;
//		_indexCount = theFacesCount * 3;
//
//		aiFace *theFaces = inMesh->mFaces;
//
//		_indexesData = new GLushort[_indexCount];
//
//		for(unsigned int i = 0; i < theFacesCount; i++) {
//			//TODO: Create verify if face has not three indexes
//			// (inMesh->mNumIndices)
//			aiFace face = theFaces[i];
//
//			_indexesData[i*3 + 0] = face.mIndices[0];
//			_indexesData[i*3 + 1] = face.mIndices[1];
//			_indexesData[i*3 + 2] = face.mIndices[2];
//		}
//
//		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//		// Create buffers
//		_coordinatesGLID = createBuffer(GL_ARRAY_BUFFER,
//				_coordinatesData, _vertexCount * 3 * sizeof(GLfloat));
//
//		_colorGLID = createBuffer(GL_ARRAY_BUFFER,
//				_colorData, _vertexCount * 3 * sizeof(GLfloat));
//
//		_indexesGLID = createBuffer(GL_ELEMENT_ARRAY_BUFFER,
//				_indexesData, _indexCount * sizeof(GLushort));
//
//		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//		// Create program
//		_shaderProgramGLID = createShaderProgram(
//				"shaders/triangle.v.glsl", "shaders/triangle.f.glsl",
//				_vertexShaderGLID, _fragmentShaderGLID);
//
//		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//		// Generate VAO
//		glGenVertexArrays(1, &_dataVAOGLID);
//		glBindVertexArray(_dataVAOGLID);
//
//		GLuint theLocation;
//
//		theLocation = getAttributeLocation(_shaderProgramGLID, "coord3d");
//		glBindBuffer(GL_ARRAY_BUFFER, _coordinatesGLID);
//		glEnableVertexAttribArray(theLocation);
//		glVertexAttribPointer(
//		/*index */		theLocation,
//		/*size */		(GLint)3,
//		/*type */		(GLenum)GL_FLOAT,
//		/*normalized */	(GLboolean)GL_FALSE,
//		/*stride */		(GLsizei)0,
//		/*offset */		(const GLvoid*)0
//		);
//
//		theLocation = getAttributeLocation(_shaderProgramGLID, "v_color");
//		glBindBuffer(GL_ARRAY_BUFFER, _colorGLID);
//		glEnableVertexAttribArray(theLocation);
//		glVertexAttribPointer(
//		/*index */		theLocation,
//		/*size */		(GLint)3,
//		/*type */		(GLenum)GL_FLOAT,
//		/*normalized */	(GLboolean)GL_FALSE,
//		/*stride */		(GLsizei)0,
//		/*offset */		(const GLvoid*)0
//		);
//
//		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//		// Create program
//		_mvpUniformBindingGLID = getUniformLocation(_shaderProgramGLID, "mvp");
	}

	virtual ~LineServer() {
//		glDeleteBuffers(1, &_coordinatesGLID);
//		glDeleteBuffers(1, &_colorGLID);
//
//		glDeleteBuffers(1, &_indexesGLID);
//
//		//TODO: Maybe, remove shader program too
//		glDeleteProgram(_shaderProgramGLID);
//
//		glDeleteVertexArrays(1, &_dataVAOGLID);
	}

	void addLine(const glm::vec3 &inFrom, const glm::vec3 &inTo,
			const glm::vec3 &inColor, float inLifeTime)
	{
		//1. Create some structure
	}


	//-- Drawing workflow
	void draw(const glm::mat4 &inProjection, const glm::mat4 &inView,
			const glm::mat4 &inModel)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////
class Mesh {
private:
	//- State
	std::string _name;

	//-- DATA
	//--- CPU data buffers
	GLfloat *_coordinatesData;	// Coordinates
	GLfloat *_colorData;		// Color
	GLint _vertexCount;

	GLushort *_indexesData;		// Indexes
	GLint _indexCount;

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

public:
	//- Methods
	//-- Memory lifecycle
	Mesh(aiMesh *inMesh) {
		_name = inMesh->mName.data;

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// Create vertex buffers data
		_vertexCount = inMesh->mNumVertices;
		aiVector3D *theVertexes = inMesh->mVertices;

		_coordinatesData = new GLfloat[_vertexCount * 3];
		_colorData = new GLfloat[_vertexCount * 3];

		for(unsigned int i = 0; i < _vertexCount; ++i) {
			_coordinatesData[i*3 + 0] = theVertexes[i].x;
			_coordinatesData[i*3 + 1] = theVertexes[i].y;
			_coordinatesData[i*3 + 2] = theVertexes[i].z;

			_colorData[i*3 + 0] = ((float)rand()) / RAND_MAX;
			_colorData[i*3 + 1] = ((float)rand()) / RAND_MAX;
			_colorData[i*3 + 2] = ((float)rand()) / RAND_MAX;
		}

		// Create index array
		unsigned int theFacesCount = inMesh->mNumFaces;
		_indexCount = theFacesCount * 3;

		aiFace *theFaces = inMesh->mFaces;

		_indexesData = new GLushort[_indexCount];

		for(unsigned int i = 0; i < theFacesCount; i++) {
			//TODO: Create verify if face has not three indexes
			// (inMesh->mNumIndices)
			aiFace face = theFaces[i];

			_indexesData[i*3 + 0] = face.mIndices[0];
			_indexesData[i*3 + 1] = face.mIndices[1];
			_indexesData[i*3 + 2] = face.mIndices[2];
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// Create buffers
		_coordinatesGLID = createBuffer(GL_ARRAY_BUFFER,
				_coordinatesData, _vertexCount * 3 * sizeof(GLfloat));

		_colorGLID = createBuffer(GL_ARRAY_BUFFER,
				_colorData, _vertexCount * 3 * sizeof(GLfloat));

		_indexesGLID = createBuffer(GL_ELEMENT_ARRAY_BUFFER,
				_indexesData, _indexCount * sizeof(GLushort));

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

	//-- Registering
	const std::string &name() { return _name; }

	//-- Drawing workflow
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

		//TODO: Try to use here GL_LINES - you'll see how to draw lines
		glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_SHORT, 0);

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

	//- Methods
	//-- Meshes loading
	void processNode(aiNode *inAssimpNode, const aiScene *inAssimpScene){
		for(unsigned int i = 0; i < inAssimpNode->mNumMeshes; ++i) {
			// Bind mesh
			//TODO: Process hierarchical meshes
			_meshes.push_back(
					new Mesh(inAssimpScene->mMeshes[inAssimpNode->mMeshes[i]]));
		}
		for(unsigned int i = 0; i < inAssimpNode->mNumChildren; ++i) {
			processNode(inAssimpNode->mChildren[i], inAssimpScene);
		}
	}

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

	bool loadMeshesFromFile(const char *inFilePath) {
		Assimp::Importer theImporter;
		const aiScene *theScene = theImporter.ReadFile(inFilePath,
				aiProcess_CalcTangentSpace       |
				aiProcess_Triangulate            |
				aiProcess_JoinIdenticalVertices  |
				aiProcess_SortByPType);

		if(!theScene) return false;
		processNode(theScene->mRootNode, theScene);

		return true;
	}

	void createModelForMesh(const char *inMeshName,
			const glm::mat4 &inCoordinateSystem)
	{
		for (unsigned int i = 0, size = _meshes.size(); i < size; ++i) {
			if (_meshes[i]->name() == inMeshName) {
				Model *theModel = new Model();
				theModel->coordinateSystem = inCoordinateSystem;
				theModel->mesh = _meshes[i];
				_models.push_back(theModel);

				return;
			}
		}
		std::cout << "CANNOT FIND THE MESH" << std::endl;
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
	gScene->loadMeshesFromFile("Assets.dxf");

	double x = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0) / 10.0 * (2*M_PI);
	double y = cosf(glutGet(GLUT_ELAPSED_TIME) / 1000.0) / 10.0 * (2*M_PI);
	gScene->createModelForMesh("0",
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
