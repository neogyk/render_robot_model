varying vec3 f_color;
uniform float fade;
void main(void)
{
/*
	gl_FragColor[0] = (gl_FragCoord.x-gl_FragCoord.y)/gl_FragCoord.x;
	gl_FragColor[1] = (gl_FragCoord.y-gl_FragCoord.x)/gl_FragCoord.y;
	gl_FragColor[2] = 0.42;
  if (mod(gl_FragCoord.y, 50.0) > 25)
  {
    gl_FragColor[3] = 1;
  }
  else
  {
    gl_FragColor[3] = 0.75;
  }
*/

  gl_FragColor = vec4(f_color.x, f_color.y, f_color.z, fade);
}

