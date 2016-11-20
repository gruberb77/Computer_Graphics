attribute  vec4 vPosition;
attribute vec4 Normal;
attribute vec2 vTexCoord;
uniform  vec4 vColor;
varying vec4 color;
varying vec2 texCoord;

uniform mat4 model_view;
uniform mat4 camera_view;
uniform mat4 projection;


void main()
{
    gl_Position = projection*camera_view*model_view*vPosition;
    texCoord = vTexCoord;
    color = vColor;
}