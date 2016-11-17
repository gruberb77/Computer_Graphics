attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 fColor;

uniform vec3 player1;
uniform vec3 player2;
uniform float flag;

void main()
{

  if(flag == 1.0)
  {
		gl_Position.x = (1.0/60.0)*(player1.x - 60.0);
  		gl_Position.y = player1.y;
  		gl_Position.z = player1.z;
  		gl_Position.w = 1.0;
  }
  else if(flag == 2.0)
  {
		gl_Position.x = (1.0/60.0)*(player2.x - 60.0);
		gl_Position.y = player2.y;
		gl_Position.z = player2.z;
		gl_Position.w = 1.0;
  }
  else
  {
	  	gl_Position.x = (1.0/60.0)*(vPosition.x-60.0);
  		gl_Position.y = vPosition.y;
  		gl_Position.z = vPosition.z;
  		gl_Position.w = 1.0;
  }


  if(vPosition.x <= 10.0 || vPosition.x >= 110.0)
  {
	fColor = vec4( 0.0, 0.0, 1.0, 1.0);
  }
  else
  {
	fColor = vec4( 1.0, 1.0, 1.0, 1.0);
  }
}
