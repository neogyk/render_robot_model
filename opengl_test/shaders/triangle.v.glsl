#version 110

#if __VERSION__ < 130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec3 coord3d;
IN vec3 v_color;

OUT vec3 f_color;

uniform mat4 mvp;

void main() {
	gl_Position = mvp * vec4(coord3d, 1.0);
	f_color = v_color;
}
