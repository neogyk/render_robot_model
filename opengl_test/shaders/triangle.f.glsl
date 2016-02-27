#version 110

#if __VERSION__ < 130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec3 f_color;

void main(void) {
	gl_FragColor = vec4(f_color.x, f_color.y, f_color.z, 1);
}
