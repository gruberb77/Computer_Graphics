//******************************************************************* 
//                                                                    
//  File:        Skybox                                           
//                                                                     
//  Author:      Brent Gruber
//  Email:       bg240711@ohio.edu
//                                                                    
//
//                                                                    
//  Description: This is the implementation of a class that contains a Skybox object in 3d space                 
//                                                                    
//  Date:        11/1/2016
//                                                                    
//*******************************************************************


#include "skybox.h"


GLint  Skybox::NumPoints = 36;
bool  	Skybox::inited = false;

// Vertices of a unit Skybox centered at origin, sides aligned with axes
point4 vert[8] = {
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
//  Purpose:    initialize points for Skybox                           
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: points will be in VAO
//                                                                  
//******************************************************************
void Skybox::init_points()
{
   if(!inited) {
	colorSkybox();
	inited = true;
   }
}

//******************************************************************
//                                                                  
//  Function:   MyQuad
//                                                                  
//  Purpose:    creates a face of Skybox                          
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
void Skybox::MyQuad(int a, int b, int c, int d)
{
  vec3 normal3 = cross(vert[b] - vert[a],
      vert[c]  - vert[a]);
  vec3 normal = vec3(normal3[0], normal3[1], normal3[2]);

  //build face of cube

   tex_coords[counter] = vec2(0.0, 1.0); 
   points[counter] = vert[a]; counter++;
   tex_coords[counter] = vec2(0.0, 0.0); 
   points[counter] = vert[b]; counter++;
   tex_coords[counter] = vec2(1.0, 0.0); 
   points[counter] = vert[c]; counter++;

   normal3 = cross(vert[d] - vert[a],
      vert[c] - vert[a]);
   normal = vec3(normal3[0], normal3[1], normal3[2]);

   tex_coords[counter] = vec2(0.0, 1.0); 
   points[counter] = vert[a]; counter++;
   tex_coords[counter] = vec2(1.0, 0.0); 
   points[counter] = vert[c]; counter++;
   tex_coords[counter] = vec2(1.0, 1.0); 
   points[counter] = vert[d]; counter++;
}

//******************************************************************
//                                                                  
//  Function:   colorSkybox
//                                                                  
//  Purpose:    creates a Skybox                         
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Skybox is created
//
// Original Auth: David M. Chelberg
//                                                                  
//******************************************************************
void Skybox::colorSkybox()
{
  //build each face of Skybox
  //back
  MyQuad(1, 0, 3, 2);
  //right
  MyQuad(2, 3, 7, 6);
  //bottom
  MyQuad(3, 0, 4, 7);
  //top
  MyQuad(6, 5, 1, 2);
  //front
  MyQuad(5, 4, 7, 6);
  //left
  MyQuad(5, 4, 0, 1);
}


//******************************************************************
//                                                                  
//  Function:   Skybox
//                                                                  
//  Purpose:    constructor                       
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Skybox instance created
//                                                                  
//******************************************************************
Skybox::Skybox()
{
  index=0;
  counter = 0;
  model_view = Scale(1.0, 1.0, 1.0);

  init_points();
}

//******************************************************************
//                                                                  
//  Function:   Skybox
//                                                                  
//  Purpose:    constructor                       
//                                                                  
//  Parameters: inputs
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Skybox instance created with given inputs
//                                                                  
//******************************************************************
Skybox::Skybox(GLuint nindex, point4 *npoints, vec2 *ntexcoords, GLint nmvLoc)
{
  index = nindex;
  counter = nindex;
  points = npoints;
  tex_coords = ntexcoords;
  model_view = Scale(1.0, 1.0, 1.0);
  mvLoc = nmvLoc;
  texture = false;

  init_points();
}

//******************************************************************
//                                                                  
//  Function:   Skybox
//                                                                  
//  Purpose:    constructor                       
//                                                                  
//  Parameters: inputs
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Skybox instance created with given inputs
//                                                                  
//******************************************************************
Skybox::Skybox(GLuint nindex, point4 *npoints, vec2 *ntexcoords, GLint nmvLoc, GLint ncolLoc, GLuint *ntextures)
{
  index = nindex;
  counter = nindex;
  points = npoints;
  tex_coords = ntexcoords;
  model_view = Scale(1.0, 1.0, 1.0);
  clr = vec4(1.0, 1.0, 1.0, 1.0);
  mvLoc = nmvLoc;
  colLoc = ncolLoc;
  textures = ntextures;

  init_points();
}

//******************************************************************
//                                                                  
//  Function:   Skybox
//                                                                  
//  Purpose:    constructor                       
//                                                                  
//  Parameters: nSkybox - Skybox to copy from
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Skybox instance created from another Skybox
//                                                                  
//******************************************************************
Skybox::Skybox(Skybox *nSkybox)
{
  index = nSkybox->index;
  counter = nSkybox->counter;
  points = nSkybox->points;
  model_view = nSkybox->model_view;
  trans_vec = nSkybox->trans_vec;
  rot_vec = nSkybox->rot_vec;
  scl_vec = nSkybox->scl_vec;
  mvLoc = nSkybox->mvLoc;
  colLoc = nSkybox->colLoc;
}

//******************************************************************
//                                                                  
//  Function:   draw
//                                                                  
//  Purpose:    draw the Skybox                       
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Skybox is visible on screen
//                                                                  
//******************************************************************
void Skybox::draw()
{
  int facePoints = 6;

	//send values and draw
	glUniform4f(colLoc, clr.x, clr.y, clr.z, clr.w);
    glUniformMatrix4fv(mvLoc, 1, GL_TRUE, model_view);

    //behind
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glDrawArrays(GL_TRIANGLES, index, facePoints);
    index+=6;

    //right
    glBindTexture(GL_TEXTURE_2D, textures[4]);

    glDrawArrays(GL_TRIANGLES, index, facePoints);
    index+=6;

    //below
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    glDrawArrays(GL_TRIANGLES, index, facePoints);
    index+=6;

    //top
    glBindTexture(GL_TEXTURE_2D, textures[5]);

    glDrawArrays(GL_TRIANGLES, index, facePoints);
    index+=6;

    //front
    glBindTexture(GL_TEXTURE_2D, textures[2]);

    glDrawArrays(GL_TRIANGLES, index, facePoints);
    index+=6;

    //left
    glBindTexture(GL_TEXTURE_2D, textures[3]);

    glDrawArrays(GL_TRIANGLES, index, facePoints);
    index=0;
}

//******************************************************************
//                                                                  
//  Function:   draw
//                                                                  
//  Purpose:    draw the Skybox                     
//                                                                  
//  Parameters: modview - external model view matrix to use
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Skybox will be visible on screen, transformed by modview
//                                                                  
//******************************************************************
void Skybox::draw(mat4 modview)
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
//  Purpose:    translate the Skybox                      
//                                                                  
//  Parameters: new_vec - translation vector to move Skybox with
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: model_view matrix is updated with a translate vector
//                                                                  
//******************************************************************
void Skybox::trans(vec3 new_vec)
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
//  Purpose:    rotate the Skybox                    
//                                                                  
//  Parameters: new_vec - rotation vector to move Skybox with (in degrees)
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Skybox is rotated by amounts given in vector
//                                                                  
//******************************************************************
void Skybox::rot(vec3 new_vec)
{
	mat4 temp, temp2;
	temp = RotateZ(new_vec.z) * RotateY(new_vec.y) * RotateX(new_vec.x);
	rot_vec += new_vec;
	temp2 = model_view;
	model_view = temp * temp2; 
}

//******************************************************************
//                                                                  
//  Function:   scl
//                                                                  
//  Purpose:    scale the Skybox                     
//                                                                  
//  Parameters: new_vec - new scale vector
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Skybox is scaled by the vector
//                                                                  
//******************************************************************
void Skybox::scl(vec3 new_vec)
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
void Skybox::update_mv()
{
	model_view = Translate(trans_vec) * (RotateX(rot_vec.x) * RotateY(rot_vec.y) * RotateZ(rot_vec.z)) * Scale(scl_vec);
}

//******************************************************************
//                                                                  
//  Function:   transform
//                                                                  
//  Purpose:    to transform the Skybox including a translation, a rotation, and a scale                      
//                                                                  
//  Parameters: t_vec - translatio vec
//				r_vec - rotation vec
//				s_vec - scale vec
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Skybox is transformed
//                                                                  
//******************************************************************
void Skybox::transform(vec3 t_vec, vec3 r_vec, vec3 s_vec)
{
	trans_vec += t_vec;
	rot_vec += r_vec;
	scl_vec = s_vec;
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
void Skybox::Identity()
{
	trans_vec = (0.0, 0.0, 0.0);
	rot_vec = (0.0, 0.0, 0.0);
	scl_vec = (0.0, 0.0, 0.0);
}



void Skybox::set_pos(vec3 t_vec)
{
  trans_vec = t_vec;
  update_mv();
}

//******************************************************************
//                                                                  
//  Function:   Check_Collision
//                                                                  
//  Purpose:    check if there is a collision with the Skybox                      
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
bool Skybox::Check_Collision(vec3 point, vec3 movement)
{
	//get positions of faces of each wall
	GLfloat left = trans_vec.x - (scl_vec.x/2); //left side of Skybox
	GLfloat right = trans_vec.x + (scl_vec.x/2); //right side of Skybox
	GLfloat front = trans_vec.z + (scl_vec.z/2); //front of Skybox
	GLfloat back = trans_vec.z - (scl_vec.z/2); //back of Skyboxw
	GLfloat bottom = trans_vec.y - (scl_vec.y/2); //bottom of Skybox
	GLfloat top = trans_vec.y + (scl_vec.y/2); //top of Skybox

	//check if the point is within the x, y or z planes of the Skybox
	bool withinx = (point.x < right && point.x > left);
	bool withiny = (point.y < top && point.y > bottom);
	bool withinz = (point.z > back && point.z < front);

	//within the xy plane of Skybox and point crosses one of the z planes after movement
	if(withinx && withiny && ((point.z > front && (point.z + movement.z) < front) || (point.z < back && (point.z + movement.z) > back)))
	{
		return true;
	}

	//within the yz plane of Skybox and point crosses one of the x planes after movement
	if(withiny && withinz && ((point.x < left && (point.x + movement.x) > left) || (point.x > right && (point.x + movement.x) < right)))
	{
		return true;
	}

	//within the xz plane of Skybox and point crosses one of the y planes after movement
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
void Skybox::color(GLfloat nr, GLfloat ng, GLfloat nb)
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
vec3 Skybox::get_translation()
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
vec3 Skybox::get_rotation()
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
vec3 Skybox::get_scale()
{
	return scl_vec;
}

//******************************************************************
//                                                                  
//  Function:   get_location
//                                                                  
//  Purpose:    return the location of the Skybox                      
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: return the location vector
//                                                                  
//******************************************************************
vec3 Skybox::get_location()
{
	return trans_vec;
}