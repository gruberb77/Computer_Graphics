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


void Cube::init_points()
{
   if(!inited) {
	colorcube();
	inited = true;
   }
}

void Cube::MyQuad(int a, int b, int c, int d)
{
  points[counter] = vertex[a]; counter++;
  points[counter] = vertex[b]; counter++;
  points[counter] = vertex[c]; counter++;
  points[counter] = vertex[a]; counter++;
  points[counter] = vertex[c]; counter++;
  points[counter] = vertex[d]; counter++;
}

//----------------------------------------------------------------------------
// generate 12 triangles: 36 vertices and 36 colors
void Cube::colorcube()
{
  MyQuad(1, 0, 3, 2);
  MyQuad(2, 3, 7, 6);
  MyQuad(3, 0, 4, 7);
  MyQuad(6, 5, 1, 2);
  MyQuad(4, 5, 6, 7);
  MyQuad(5, 4, 0, 1);
}


Cube::Cube()
{
  index=0;
  counter = 0;

  model_view = Scale(1.0, 1.0, 1.0);

  init_points();
}

Cube::Cube(GLuint nindex, point4 *npoints, GLint nmvLoc)
{
  index = nindex;
  counter = nindex;
  points = npoints;
  model_view = Scale(1.0, 1.0, 1.0);

  mvLoc = nmvLoc;

  init_points();
}


Cube::Cube(GLuint nindex, point4 *npoints, GLint nmvLoc, GLint ncolLoc)
{
  index = nindex;
  counter = nindex;
  points = npoints;
  model_view = Scale(1.0, 1.0, 1.0);
  clr = vec4(1.0, 1.0, 1.0, 1.0);

  mvLoc = nmvLoc;
  colLoc = ncolLoc;

  init_points();
}

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

//default draw
void Cube::draw()
{
	glUniform4f(colLoc, clr.x, clr.y, clr.z, clr.w);
    glUniformMatrix4fv(mvLoc, 1, GL_TRUE, model_view);
    glDrawArrays(GL_TRIANGLES, index, NumPoints);
}

//draw the cube with a passed in model view matrix
void Cube::draw(mat4 modview)
{ 
    update_mv();
    mat4 temp = model_view;
    mat4 model_view = modview * temp;
    glUniform4f(colLoc, clr.x, clr.y, clr.z, clr.w);
    glUniformMatrix4fv(mvLoc, 1, GL_TRUE, model_view);
    glDrawArrays(GL_TRIANGLES, index, NumPoints);
}


//Translate Cube
void Cube::trans(vec3 new_vec)
{

	mat4 temp, temp2;
	trans_vec += new_vec;
	temp = Translate(new_vec.x, new_vec.y, new_vec.z);

	temp2 = model_view;
	model_view = temp * temp2;

	//trans_vec += new_vec;
	//update_mv();
}

//Rotate in X and/or Y and/or Z
void Cube::rot(vec3 new_vec)
{

	mat4 temp, temp2;
	temp = RotateZ(new_vec.z) * RotateY(new_vec.y) * RotateX(new_vec.x);
	rot_vec += new_vec;
	temp2 = model_view;
	model_view = temp * temp2; 

	//rot_vec += new_vec;
	//update_mv();
}

//Scale the cube
void Cube::scl(vec3 new_vec)
{
	/*mat4 temp;
	scl_vec = new_vec;
	temp = Scale(scl_vec.x, scl_vec.y, scl_vec.z);
	model_view = temp * model_view;*/

	scl_vec = new_vec;
	update_mv();
}

void Cube::update_mv()
{
	model_view = Translate(trans_vec) * (RotateX(rot_vec.x) * RotateY(rot_vec.y) * RotateZ(rot_vec.z)) * Scale(scl_vec);
}

//transform the cube
void Cube::transform(vec3 t_vec, vec3 r_vec, vec3 s_vec)
{
	trans_vec += t_vec;
	rot_vec += r_vec;
	scl_vec = s_vec;
	update_mv();

	/*scl(s_vec);
	rot(r_vec);
	trans(t_vec); */
}

//set model_view to the identity matrix
void Cube::Identity()
{
	trans_vec = (0.0, 0.0, 0.0);
	rot_vec = (0.0, 0.0, 0.0);
	scl_vec = (0.0, 0.0, 0.0);
}

bool Cube::Check_Collision(vec3 point, vec3 movement)
{

	GLfloat left = trans_vec.x - (scl_vec.x/2); //left side of cube
	GLfloat right = trans_vec.x + (scl_vec.x/2); //right side of cube
	GLfloat front = trans_vec.z + (scl_vec.z/2); //front of cube
	GLfloat back = trans_vec.z - (scl_vec.z/2); //back of cube
	GLfloat bottom = trans_vec.y - (scl_vec.y/2); //bottom of cube
	GLfloat top = trans_vec.y + (scl_vec.y/2); //top of cube

	bool withinx = (point.x < right && point.x > left);
	bool withiny = (point.y < top && point.y > bottom);
	bool withinz = (point.z > back && point.z < front);

	std::cout << "top = " << top << " bottom = " << bottom << " point.y = " << point.y << std::endl; 

	//within the xy plane of cube and point crosses one of the z planes after movement
	if(withinx && withiny && ((point.z > front && (point.z + movement.z) < front) || (point.z < back && (point.z + movement.z) > back)))
	{
		std::cout << "1" << std::endl;
		return true;
	}

	//within the yz plane of cube and point crosses one of the x planes after movement
	if(withiny && withinz && ((point.x < left && (point.x + movement.x) > left) || (point.x > right && (point.x + movement.x) < right)))
	{
				std::cout << "2" << std::endl;
		return true;
	}

	//within the xz plane of cube and point crosses one of the y planes after movement
	if(withinx && withinz && ((point.y < bottom && (point.y + movement.y) > bottom) || (point.y > top && (point.y + movement.y) < top)))
	{
				std::cout << "3" << std::endl;
		return true;
	}

	return false;
}

void Cube::color(GLfloat nr, GLfloat ng, GLfloat nb)
{
	clr = vec4(nr, ng, nb, 1.0);
}

vec3 Cube::get_translation()
{
	return trans_vec;
}

vec3 Cube::get_rotation()
{
	return rot_vec;
}

vec3 Cube::get_scale()
{
	return scl_vec;
}

vec3 Cube::get_location()
{
	return trans_vec;
}





  
