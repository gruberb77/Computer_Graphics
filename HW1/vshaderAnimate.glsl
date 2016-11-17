attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 fColor;

uniform vec3 offset;
uniform float size;
uniform vec2 windowSize;
uniform vec4 vcolor;

void main()
{

  gl_Position.x = (1.0/60.0)*(vPosition.x-60.0+offset.x);
  gl_Position.y = vPosition.y;
  gl_Position.z = vPosition.z;
  gl_Position.w = 1.0;


  if(vPosition.x <= 10.0 || vPosition.x >= 110.0)
  {
	fColor = vec4( 0.0, 0.0, 1.0, 1.0);
  }
  else
  {
	fColor = vec4( 1.0, 1.0, 1.0, 1.0);
  }
}
