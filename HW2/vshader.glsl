attribute vec4 vPosition;

varying vec4 fcolor;

uniform ivec2 offset;
uniform vec2 size;
uniform vec2 windowSize;
uniform vec4 vcolor;

void main() 
{
  gl_Position.x=(2.0*(size.x*(vPosition.x)+float(offset.x)))/windowSize.x-1.0;
  gl_Position.y=(2.0*(size.y*(vPosition.y)+float(offset.y)))/windowSize.y-1.0;
  gl_Position.z=0.0;
  gl_Position.w=1.0;

  fcolor = vcolor;
}
