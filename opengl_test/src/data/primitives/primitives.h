#ifndef PRIMITIVES_H_INCLUDED
#define PRIMITIVES_H_INCLUDED


#if 0
#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct attributes {
		GLfloat coord3d[3];
		GLfloat v_color[3];
};

class Cube
{
public:
	Cube(void);
	~Cube(void);
	bool setTexture(char*);
	void Visualize();

private:
	bool IsCreated;
	GLuint vbo_cube_attrib;
	GLuint ibo_cube_elements;
	glm::vec3 Position;
	attributes cube_attributes[8];
  GLushort cube_ele#endifments[36];
  glm::mat4 model, view, projection, anim, mvp;
};

#endif

#endif // PRIMITIVES_H_INCLUDED
