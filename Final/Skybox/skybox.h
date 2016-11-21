//******************************************************************* 
//                                                                    
//  File:        Skybox                                           
//                                                                     
//  Author:      Brent Gruber
//  Email:       bg240711@ohio.edu
//                                                                    
//
//                                                                    
//  Description: This is the definition of a class that contains a Skybox object in 3d space
//				 points must be in VAO to implement a Skybox                 
//                                                                    
//  Date:        11/1/2016
//                                                                    
//*******************************************************************

#ifndef Skybox_H
#define Skybox_H

#include "Angel.h"
#include <iostream>

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

class Skybox
{
public:
	//number of points in a Skybox
	static GLint NumPoints;

	//Default Constructor
	Skybox();

	//second constructor
	Skybox(GLuint nindex, point4 *npoints, vec2 *ntexcoords, GLint nmvLoc);

	//third Constructor
	Skybox(GLuint nindex, point4 *npoints, vec2 *ntexcoords, GLint nmvLoc, GLint colLoc, GLuint *ntextures);

	// Constructor
	Skybox(Skybox *nSkybox);

	// Initialize the points
	void init_points();

	// Code to draw the Skybox
	void draw();

	// draw the Skybox given a 4x4 matrix
	void draw(mat4 modview);

	// Create the Skybox
	void colorSkybox();

	// set the position, not dependent on previous position
	void set_pos(vec3 t_vec);
	
	// create a face of the Skybox
	void MyQuad(int a, int b, int c, int d);

	// translate the Skybox
	void trans(vec3 tran_vec);

	// Rotate in degrees
	void rot(vec3 rot_vec);

	// scale the Skybox
	void scl(vec3 scale_vec);

	// pass in all transformations at one time
	void transform(vec3 tran_vec, vec3 rot_vec, vec3 scale_vec);

	// set model view to identity matrix
	void Identity();

	// change color
	void color(GLfloat nr, GLfloat ng, GLfloat nb);

	// check if a point is within the volume of the Skybox
	bool Check_Collision(vec3 point, vec3 movement);

	//get the translation of the object
	vec3 get_translation();
	vec3 get_rotation();
	vec3 get_scale();
	vec3 get_location();

private:
	void update_mv();

	static bool inited;

	//whether to use a texture or not
	bool texture;

	// transform matrix
	mat4 model_view;

	// location of center
	vec3 location;

	// color of Skybox
	vec4 clr;

	//translation vector
	vec3 trans_vec;
	
	//rotation vector
	vec3 rot_vec;

	//scale vector
	vec3 scl_vec;

	// location of model view matrix
	GLuint mvLoc;

	// location of color vector
	GLuint colLoc;

	// index in VAO
	GLuint index;

	// Pointer to points array
	vec4 *points;

	// pointer to normals array
	vec2 *tex_coords;

	//used for any motion
	GLint last_time;

	// counter for initializing points
	GLuint counter;

	// textures
	GLuint *textures;

};

#endif