#ifndef CAMERA_H
#define CAMERA_H
// Class definition of player object
//
// Written by Brent Gruber, based on cube class
//
// Cube must be defined in VAO at index

#include "Angel.h"


class Camera{
	public:
		//default constructor
		Camera();

		//second constructor
		Camera(GLuint ncamLoc);

		//third constructor
		Camera(GLuint ncamLoc, vec4 neye, vec4 nat, vec4 nup);

		//change the point of view
		void Toggle_Pov();

		//move eye
		void Move_Eye(vec4 neye);
		void Move_Eye(float nx, float ny, float nz);

		//move where to look at
		void Move_At(vec4 nat);
		void Move_At(float nx, float ny, float nz);
		void Move_At(GLfloat theta, GLfloat phi);

		//move which way is up
		void Move_Up(vec4 nup);	
		void Move_Up(float nx, float ny, float nz);

		//update and send the camera matrix
		void Update_Camera();	

		//reverse the camera
		void Reverse();

		//return the eye
		vec4 Get_Eye();

		//return at
		vec4 Get_At();
		
		//return up
		vec4 Get_Up();

	private:
		//matrix to hold camera transformations
		mat4 camera_view;

		//location to pass matrix
		GLuint camLoc;

		//radius of view sphere
		GLfloat radius;

		//theta for x component
		GLfloat theta;

		//phi for y component
		GLfloat phi;

		//where the eye is located
		vec4 eye;

		//where to look at
		vec4 at;

		//which way is up
		vec4 up;

		//offset to move camera when toggling point of view
		vec3 offset;

		//if camera is in third person or not
		bool thirdp;
};

#endif
