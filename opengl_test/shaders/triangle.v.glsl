#version 330

in vec3 coord3d;
in vec3 v_color;

out vec3 f_color;

uniform mat4 mvp;

void main() {
	gl_Position = mvp * vec4(coord3d, 1.0);
	f_color = v_color;
}
