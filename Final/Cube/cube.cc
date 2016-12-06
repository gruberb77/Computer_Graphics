//******************************************************************* 
//                                                                    
//  File:        Cube                                           
//                                                                     
//  Author:      Brent Gruber
//  Email:       bg240711@ohio.edu
//                                                                    
//
//                                                                    
//  Description: This is the implementation of a class that contains a cube object in 3d space                 
//                                                                    
//  Date:        11/1/2016
//                                                                    
//*******************************************************************


#include "cube.h"


GLint  Cube::NumPoints = 36;
bool  	Cube::inited = false;

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertex[8] = {
  point4(-0.5, -0.5,  0.5, 1.0),
  point4(-0.5,  0.5,  0.5, 1.0),
  point4( 0.5,  0.5,  0.5, 1.0),
  point4( 0.5, -0.5,  0.5, 1.0),
  point4(-0.5, -0.5, -0.5, 1.0),
  point4(-0.5,  0.5, -0.5, 1.0),
  point4( 0.5,  0.5, -0.5, 1.0),
  point4( 0.5, -0.5, -0.5, 1.0)
};


//******************************************************************
//                                                                  
//  Function:   init_points
//                                                                  
//  Purpose:    initialize points for cube                           
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: points will be in VAO
//                                                                  
//******************************************************************
void Cube::init_points()
{
   if(!inited) {
	colorcube();
	inited = true;
   }
}

//******************************************************************
//                                                                  
//  Function:   MyQuad
//                                                                  
//  Purpose:    creates a face of cube                          
//                                                                  
//  Parameters: a, b, c, d - indices of 4 corners of face
//                                                                  
//
// Pre Conditions: vertex array contains points;
//
// Post Conditions: Face is created
//
// Original Auth: David M. Chelberg
//                                                                  
//******************************************************************
void Cube::MyQuad(int a, int b, int c, int d)
{
  vec3 normal3 = cross(vertex[b] - vertex[a],
      vertex[c]  - vertex[a]);
  vec3 normal = vec3(normal3[0], normal3[1], normal3[2]);

  //build face of cube

   normals[counter] = normal; 
   points[counter] = vertex[a]; counter++;
   normals[counter] = normal; 
   points[counter] = vertex[b]; counter++;
   normals[counter] = normal; 
   points[counter] = vertex[c]; counter++;

   normal3 = cross(vertex[d] - vertex[a],
      vertex[c] - vertex[a]);
   normal = vec3(normal3[0], normal3[1], normal3[2]);

   normals[counter] = normal; 
   points[counter] = vertex[a]; counter++;
   normals[counter] = normal; 
   points[counter] = vertex[c]; counter++;
   normals[counter] = normal; 
   points[counter] = vertex[d]; counter++;
}

//******************************************************************
//                                                                  
//  Function:   colorcube
//                                                                  
//  Purpose:    creates a cube                         
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: cube is created
//
// Original Auth: David M. Chelberg
//                                                                  
//******************************************************************
void Cube::colorcube()
{
  //build each face of cube
  MyQuad(1, 0, 3, 2);
  MyQuad(2, 3, 7, 6);
  MyQuad(3, 0, 4, 7);
  MyQuad(6, 5, 1, 2);
  MyQuad(4, 5, 6, 7);
  MyQuad(5, 4, 0, 1);
}


//******************************************************************
//                                                                  
//  Function:   Cube
//                                                                  
//  Purpose:    constructor                       
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Cube instance created
//                                                                  
//******************************************************************
Cube::Cube()
{
  index=0;
  counter = 0;
  model_view = Scale(1.0, 1.0, 1.0);

  init_points();
}

//******************************************************************
//                                                                  
//  Function:   Cube
//                                                                  
//  Purpose:    constructor                       
//                                                                  
//  Parameters: inputs
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Cube instance created with given inputs
//                                                                  
//******************************************************************
Cube::Cube(GLuint nindex, point4 *npoints, vec3 *nnormals, GLint nmvLoc)
{
  index = nindex;
  counter = nindex;
  points = npoints;
  normals = nnormals;
  model_view = Scale(1.0, 1.0, 1.0);
  mvLoc = nmvLoc;
  texture = false;

  init_points();
}

//******************************************************************
//                                                                  
//  Function:   Cube
//                                                                  
//  Purpose:    constructor                       
//                                                                  
//  Parameters: inputs
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Cube instance created with given inputs
//                                                                  
//******************************************************************
Cube::Cube(GLuint nindex, point4 *npoints, vec3 *nnormals, GLint nmvLoc, GLint ncolLoc)
{
  index = nindex;
  counter = nindex;
  points = npoints;
  normals = nnormals;
  model_view = Scale(1.0, 1.0, 1.0);
  clr = vec4(1.0, 1.0, 1.0, 1.0);
  mvLoc = nmvLoc;
  colLoc = ncolLoc;

  init_points();
}

//******************************************************************
//                                                                  
//  Function:   Cube
//                                                                  
//  Purpose:    constructor                       
//                                                                  
//  Parameters: nCube - cube to copy from
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Cube instance created from another cube
//                                                                  
//******************************************************************
Cube::Cube(Cube *nCube)
{
  index = nCube->index;
  counter = nCube->counter;
  points = nCube->points;
  model_view = nCube->model_view;
  trans_vec = nCube->trans_vec;
  rot_vec = nCube->rot_vec;
  scl_vec = nCube->scl_vec;
  mvLoc = nCube->mvLoc;
  colLoc = nCube->colLoc;
}

//******************************************************************
//                                                                  
//  Function:   draw
//                                                                  
//  Purpose:    draw the cube                       
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: cube is visible on screen
//                                                                  
//******************************************************************
void Cube::draw()
{
	//send values and draw
	glUniform4f(colLoc, clr.x, clr.y, clr.z, clr.w);
    glUniformMatrix4fv(mvLoc, 1, GL_TRUE, model_view);
    glDrawArrays(GL_TRIANGLES, index, NumPoints);
}

//******************************************************************
//                                                                  
//  Function:   draw
//                                                                  
//  Purpose:    draw the cube                     
//                                                                  
//  Parameters: modview - external model view matrix to use
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Cube will be visible on screen, transformed by modview
//                                                                  
//******************************************************************
void Cube::draw(mat4 modview)
{ 
    update_mv();
    mat4 temp = model_view;
    mat4 model_view = modview * temp;
    glUniform4f(colLoc, clr.x, clr.y, clr.z, clr.w);
    glUniformMatrix4fv(mvLoc, 1, GL_TRUE, model_view);
    glDrawArrays(GL_TRIANGLES, index, NumPoints);
}


//******************************************************************
//                                                                  
//  Function:   trans
//                                                                  
//  Purpose:    translate the cube                      
//                                                                  
//  Parameters: new_vec - translation vector to move cube with
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: model_view matrix is updated with a translate vector
//                                                                  
//******************************************************************
void Cube::trans(vec3 new_vec)
{
	mat4 temp, temp2;
	trans_vec += new_vec;
	temp = Translate(new_vec.x, new_vec.y, new_vec.z);
	temp2 = model_view;
	model_view = temp * temp2;
}

//******************************************************************
//                                                                  
//  Function:   rot
//                                                                  
//  Purpose:    rotate the cube                    
//                                                                  
//  Parameters: new_vec - rotation vector to move cube with (in degrees)
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Cube is rotated by amounts given in vector
//                                                                  
//******************************************************************
void Cube::rot(vec3 new_vec)
{
	mat4 temp, temp2;
	temp = RotateZ(new_vec.z) * RotateY(new_vec.y) * RotateX(new_vec.x);
	rot_vec += new_vec;
	temp2 = model_view;
	model_view = temp * temp2; 
}


void Cube::set_rot(vec3 new_vec)
{
  rot_vec = new_vec;
  update_mv();
}

//******************************************************************
//                                                                  
//  Function:   scl
//                                                                  
//  Purpose:    scale the cube                     
//                                                                  
//  Parameters: new_vec - new scale vector
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Cube is scaled by the vector
//                                                                  
//******************************************************************
void Cube::scl(vec3 new_vec)
{
	scl_vec = new_vec;
	update_mv();
}

//******************************************************************
//                                                                  
//  Function:   update_mv
//                                                                  
//  Purpose:    update the model view matrix                   
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: model_view matrix will contain all transformations
//                                                                  
//******************************************************************
void Cube::update_mv()
{
	model_view = Translate(trans_vec) * (RotateX(rot_vec.x) * RotateY(rot_vec.y) * RotateZ(rot_vec.z)) * Scale(scl_vec);
}

//******************************************************************
//                                                                  
//  Function:   transform
//                                                                  
//  Purpose:    to transform the cube including a translation, a rotation, and a scale                      
//                                                                  
//  Parameters: t_vec - translatio vec
//				r_vec - rotation vec
//				s_vec - scale vec
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Cube is transformed
//                                                                  
//******************************************************************
void Cube::transform(vec3 t_vec, vec3 r_vec, vec3 s_vec)
{
	trans_vec += t_vec;
	rot_vec += r_vec;
	scl_vec = s_vec;
	update_mv();
}



void Cube::set_pos(vec3 t_vec)
{
  trans_vec = t_vec;
  update_mv();
}

//******************************************************************
//                                                                  
//  Function:   Identity
//                                                                  
//  Purpose:    create identity matrix                       
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: returns identity matrix
//                                                                  
//******************************************************************
void Cube::Identity()
{
	trans_vec = (0.0, 0.0, 0.0);
	rot_vec = (0.0, 0.0, 0.0);
	scl_vec = (0.0, 0.0, 0.0);
}

//******************************************************************
//                                                                  
//  Function:   Check_Collision
//                                                                  
//  Purpose:    check if there is a collision with the cube                      
//                                                                  
//  Parameters: point - point to check collision with
//				movement - movement vector of point
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: returns true if there was a collision
//                                                                  
//******************************************************************
bool Cube::Check_Collision(vec3 point, vec3 movement)
{

	//get positions of faces of each wall
	GLfloat left = trans_vec.x - (scl_vec.x/2); //left side of cube
	GLfloat right = trans_vec.x + (scl_vec.x/2); //right side of cube
	GLfloat front = trans_vec.z + (scl_vec.z/2); //front of cube
	GLfloat back = trans_vec.z - (scl_vec.z/2); //back of cubew
	GLfloat bottom = trans_vec.y - (scl_vec.y/2); //bottom of cube
	GLfloat top = trans_vec.y + (scl_vec.y/2); //top of cube

	//check if the point is within the x, y or z planes of the cube
	bool withinx = (point.x <= right && point.x >= left);
	bool withiny = (point.y <= top && point.y >= bottom);
	bool withinz = (point.z >= back && point.z <= front);

	//within the xy plane of cube and point crosses one of the z planes after movement
	if(withinx && withiny && ((point.z > front && (point.z + movement.z) < front) || (point.z < back && (point.z + movement.z) > back)))
	{
		return true;
	}

	//within the yz plane of cube and point crosses one of the x planes after movement
	if(withiny && withinz && ((point.x < left && (point.x + movement.x) > left) || (point.x > right && (point.x + movement.x) < right)))
	{
		return true;
	}

	//within the xz plane of cube and point crosses one of the y planes after movement
	if(withinx && withinz && ((point.y < bottom && (point.y + movement.y) > bottom) || (point.y > top && (point.y + movement.y) < top)))
	{
		return true;
	}


	return false;
}

//******************************************************************
//                                                                  
//  Function:   color
//                                                                  
//  Purpose:    change color                      
//                                                                  
//  Parameters: nr - amoutn of red
//				ng - amount of green
//				nb - amount of blue
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: color vector is changed
//                                                                  
//******************************************************************
void Cube::color(GLfloat nr, GLfloat ng, GLfloat nb)
{
	clr = vec4(nr, ng, nb, 1.0);
}

//******************************************************************
//                                                                  
//  Function:   get_translation
//                                                                  
//  Purpose:    return the translation vector                      
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: returns the translation vector
//                                                                  
//******************************************************************
vec3 Cube::get_translation()
{
	return trans_vec;
}

//******************************************************************
//                                                                  
//  Function:   get_rotation
//                                                                  
//  Purpose:    return the rotation vector                      
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: return the rotation vector
//                                                                  
//******************************************************************
vec3 Cube::get_rotation()
{
	return rot_vec;
}

//******************************************************************
//                                                                  
//  Function:   get_scale
//                                                                  
//  Purpose:    return the scale vector                      
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: return the scale vector
//                                                                  
//******************************************************************
vec3 Cube::get_scale()
{
	return scl_vec;
}

//******************************************************************
//                                                                  
//  Function:   get_location
//                                                                  
//  Purpose:    return the location of the cube                      
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: return the location vector
//                                                                  
//******************************************************************
vec3 Cube::get_location()
{
	return trans_vec;
}