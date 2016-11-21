
varying vec4 color;
varying vec2 texCoord;
uniform sampler2D texture;
uniform bool text;

void main()
{

    if(text)
    {
    	gl_FragColor = color * texture2D(texture, texCoord);
    }
    else
    {
    	gl_FragColor = color;
    }
}