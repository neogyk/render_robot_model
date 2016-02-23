#if 0

#include "primitives.h"

Cube::Cube(void)
	: cube_attributes{
		{{-1.0, -1.0,  1.0}, {1.0, 0.0, 0.0}},
		{{ 1.0, -1.0,  1.0}, {0.0, 1.0, 0.0}},
		{{ 1.0,  1.0,  1.0}, {0.0, 0.0, 1.0}},
		{{-1.0,  1.0,  1.0}, {0.0, 1.0, 1.0}},
		{{-1.0, -1.0, -1.0}, {0.0, 1.0, 0.0}},
		{{ 1.0, -1.0, -1.0}, {0.0, 0.0, 1.0}},
		{{ 1.0,  1.0, -1.0}, {1.0, 0.0, 0.0}},
		{{-1.0,  1.0, -1.0}, {1.0, 0.0, 1.0}},
		},
		cube_elements{
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
	    6, 2, 1,
	  }
{
	IsCreated = false;
  glGenBuffers(1, &vbo_cube_attrib);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_attrib);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_attributes), cube_attributes, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &ibo_cube_elements);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  IsCreated = true;
}

void Cube::Visualize()
{
  model = glm::translate(glm::mat4(1.0f), Position);
  view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0),
                     glm::vec3(0.0, 1.0, 0.0));
  projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height,
                                0.1f, 10.0f);
  anim = glm::mat4(1.0f);
  mvp = projection * view * model * anim;

  glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

  glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_attrib);
  glEnableVertexAttribArray(attribute_coord3d);
  //glUniform1f(uniform_fade, 1.0);
  ///Describe our vertices array to OpenGL (it can't guess its format automatically)
  glVertexAttribPointer(
    attribute_coord3d, // attribute
    3,                 // number of elements per vertex, here (x, y, z)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    sizeof(struct attributes),
    0
  );
  glVertexAttribPointer(
    attribute_v_color, // attribute
    3,                 // number of elements per vertex, here (r,g,b)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    sizeof(struct attributes),
    (GLvoid*) offsetof(struct attributes, v_color)  // offset of the first element
  );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
  int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
}

#endif

