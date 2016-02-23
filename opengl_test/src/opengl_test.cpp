/**
User: Deiskos
Date: Somewhere around july 2015
Linked libraries: -lglut -lGLEW -lGL
Comment: ��� ����� ���������� � ���� ������ �������� �� wikibooks
          � ��������� ����� ������� ������
         ����������: wasd + rf, q - exit(0)
         ����� �������� ��� ��� � ����� �� ������ ����, ����� �����
*/

#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include <math.h>


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

//Screen properties
//----------------------------------------------------------
int screen_width=800, screen_height=600;//Screen size properties

GLint attribute_coord3d, attribute_v_color;

glm::mat4 view;
glm::vec3 EyePosition = glm::vec3(0.0, 0.0, 0.0);

//----------------------------------------------------------

//- Cube state
//-- Vertex data openGL buffer ID
GLuint vbo_cube_attrib;

//-- Index data openGL buffer ID
GLuint ibo_cube_elements;

//-- Frade (alpha) for the Cube
GLint uniform_fade;

//-- ID for MVP matrix
GLint uniform_mvp;

//--
GLint uniform_m_transform;

//-- Shader program ID
GLuint program;

//-- Cude position
glm::vec3 Position;

//- Constant CPU data to send
//-- Attributes data. Each line contains vertex data as "structure"
// (really just a pair of 3D vectors) with two values.
struct attributes {
		GLfloat coord3d[3];
		GLfloat v_color[3];
};

attributes cube_attributes[8] = {
		// front
		{{-1.0, -1.0,  1.0}, {1.0 , 1.0 , 1.0 }},
		{{ 1.0, -1.0,  1.0}, {0.25, 1.0, 0.25}},
		{{ 1.0,  1.0,  1.0}, {1.0 , 1.0 , 1.0 }},
		{{-1.0,  1.0,  1.0}, {1.0, 0.25, 0.25}},
		// back
		{{-1.0, -1.0, -1.0}, {0.25, 0.25, 0.25}},
		{{ 1.0, -1.0, -1.0}, {1.0 , 1.0 , 1.0 }},
		{{ 1.0,  1.0, -1.0}, {0.25, 0.25, 0.25}},
		{{-1.0,  1.0, -1.0}, {1.0 , 1.0 , 1.0 }},
	};

//-- Index data. Each line contains three indexes of vertex, that constructs a
// triangle. So, one line = one triangle
GLushort cube_elements[36] = {
	    // front
	    0, 1, 2,
	    2, 3, 0,
	    // top
	    3, 2, 6,
	    6, 7, 3,
	    // back
	    7, 6, 5,
	    5, 4, 7,
	    // bottom
	    4, 5, 1,
	    1, 0, 4,
	    // left
	    4, 0, 3,
	    3, 7, 4,
	    // right
	    1, 5, 6,
	    6, 2, 1
	  };

//-- Helper
bool IsCreated = false;

//- Uniform data to send
//-- Transformation matrixes (for cube model)
glm::mat4 model, projection, mvp;

//-- Animation matrix (extra transform)
glm::mat4 anim;

//----------------------------------------------------------
///GLUT functions
void display() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);

	// Cube.visualize
	if(!IsCreated) return;

	//Identity matrix
	model = glm::translate(glm::mat4(1.0f), Position);

	projection = glm::perspective(
		45.0f,                              //FOV
		1.0f*screen_width/screen_height,    //Aspect ratio
		0.1f,                               //Near
		10.0f                               //Far
	);

	float angle_anim = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (M_PI/4);
	glm::vec3 axis_z(0, 0, 1), axis_y(0, 1, 0), axis_x(1, 0, 0);
	anim = glm::rotate(glm::mat4(1.0f), angle_anim, axis_x);

	mvp = projection * view * model * anim;

	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_attrib);

	glEnableVertexAttribArray(attribute_coord3d);
	glUniform1f(uniform_fade, 1.0f);

	// Works without using VAO
	///Describe our vertices array to OpenGL (it can't guess its format automatically)
	glVertexAttribPointer(
		attribute_coord3d, // attribute
		3,                 // number of elements per vertex, here (x, y, z)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		sizeof(struct attributes),
		0
	);

	glEnableVertexAttribArray(attribute_v_color);
	glVertexAttribPointer(
		attribute_v_color, // attribute
		3,                 // number of elements per vertex, here (r,g,b)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		sizeof(struct attributes),
		(GLvoid*) offsetof(struct attributes, v_color)  // offset of the first element
	);

	//Bind index array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

	//>>> Draw
	glDrawElements(GL_TRIANGLES, (size/sizeof(GLushort)), GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(attribute_coord3d);
	glDisableVertexAttribArray(attribute_v_color);

	//Swap buffer
	glutSwapBuffers();
}

void Simulate(void)
{
	view = glm::lookAt(
		EyePosition,   //eye
		EyePosition + glm::vec3(0.0, 0.0, -1.0),  //center
		glm::vec3(0.0, 1.0, 0.0)    //up
	);

	double x = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0) / 10.0 * (2*M_PI);
	double y = cosf(glutGet(GLUT_ELAPSED_TIME) / 1000.0) / 10.0 * (2*M_PI);

	Position = glm::vec3(x, y, -5);
}

void Timer(int value) {
	Simulate();
	glutPostRedisplay();
	glutTimerFunc(12, Timer, 1);
}

//- Function for processing of screen size change event
void onReshape(int width, int height) {
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, screen_width, screen_height);
}

//- Function for keys processing
void Keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
            exit(0);
        break;
        case 'r':
          std::cout << "r" << std::endl;
          EyePosition += glm::vec3(0.0, 0.1, 0.0);
        break;
        case 'f':
          std::cout << "f" << std::endl;
          EyePosition -= glm::vec3(0.0, 0.1, 0.0);
        break;
        case 'w':
          std::cout << "w" << std::endl;
          EyePosition += glm::vec3(0.0, 0.0, -0.1);
        break;
        case 's':
            std::cout << "s" << std::endl;
          EyePosition -= glm::vec3(0.0, 0.0, -0.1);
        break;
        case 'a':
            std::cout << "a" << std::endl;
          EyePosition += glm::vec3(-0.1, 0.0, 0.0);
        break;
        case 'd':
            std::cout << "d" << std::endl;
          EyePosition -= glm::vec3(-0.1, 0.0, 0.0);
        break;
    }
}

//=============================================================================
int init_resources(void) {
	// GENERATE PROGRAM
	GLint link_ok = GL_FALSE;

	GLuint vs, fs;
	if ((vs = create_shader("shaders/triangle.v.glsl", GL_VERTEX_SHADER)) == 0)
		return 0;
	if ((fs = create_shader("shaders/triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0)
		return 0;

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if(link_ok == 0) {
		fprintf(stderr, "Error while linking GLSL program");
		print_log(program);
		return 0;
	}

	//- GET PROGRAM BINDING DATA
	//-- Vertexes biding data
	const char* attribute_name = "coord3d";
	attribute_coord3d = glGetAttribLocation(program, attribute_name);
	if (attribute_coord3d == -1) {
		fprintf(stderr, "Could not bind attribute%s\n", attribute_name);
		return 0;
	}

	attribute_name = "v_color";
	attribute_v_color = glGetAttribLocation(program, attribute_name);
	if (attribute_v_color == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	//-- Uniform binding data
	//Fade data (???)
	const char* uniform_name;
	uniform_name = "fade";
	uniform_fade = glGetUniformLocation(program, uniform_name);
	if (uniform_fade == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

	//Model-view-projection matrix
	uniform_name = "mvp";
	uniform_mvp = glGetUniformLocation(program, uniform_name);
	if (uniform_mvp == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

	//GENERATE CUBE BUFFERS
	//- Generate attributes (grouped as structure)
	glGenBuffers(1, &vbo_cube_attrib);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_attrib);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_attributes),
			cube_attributes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//- Generate index data
	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements),
			cube_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	IsCreated = true;

	return 1;
}

void free_resources() {
	glDeleteProgram(program);
}

//=============================================================================
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	//	glutInitContextVersion(2,0);

	glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutCreateWindow("Modern OpenGL");

	//
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

	// Enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	//glClearColor(0.0, 0.0, 0.0, 1.0);

	if(init_resources()) {
		glutDisplayFunc(display);
		glutReshapeFunc(onReshape);
		glutKeyboardFunc(Keyboard);
		glutTimerFunc(15, Timer, 1);
		glutMainLoop();
	}

	free_resources();

	return EXIT_SUCCESS;
}