#include "camera.h"


Camera::Camera()
{
	eye = vec4(0.0, 0.5, 0.0, 1.0);
	at = vec4(0.0, 0.0, -1.0, 1.0);
	up = vec4(0.0, 1.0, 0.0, 0.0);

	radius = 2.0;

	offset = vec3(0.0, 0.0, 2.0);
	thirdp = false;
}

Camera::Camera(GLuint ncamLoc)
{
	eye = vec4(0.0, 0.5, 0.0, 1.0);
	at = vec4(0.0, 0.5, -1.0, 1.0);
	up = vec4(0.0, 1.0, 0.0, 0.0);

	radius = 2.0;

	camLoc = ncamLoc;

	offset = vec3(0.0, 0.25, 2.0);
	thirdp = false;
}


Camera::Camera(GLuint ncamLoc, vec4 neye, vec4 nat, vec4 nup)
{
	eye = neye;
	at = nat;
	up = nup;

	radius = 2.0;

	camLoc = ncamLoc;

	offset = vec3(0.0, 0.25, 2.0);
	thirdp = false;
}


void Camera::Toggle_Pov()
{
	if(thirdp)
	{
		eye -= offset;
		at.y += 0.5;
	}
	else
	{
		eye += offset;
		at.y -= 0.5;
	}

	thirdp = !thirdp;
}

void Camera::Move_Eye(vec4 neye)
{
	neye.w = 0.0;
	eye += neye;
}

void Camera::Move_Eye(float nx, float ny, float nz)
{
	eye += vec4(nx, ny, nz, 0.0);
	std::cout << "eye = " << eye << std::endl;
}

void Camera::Move_At(vec4 nat)
{
	at += nat;
}

void Camera::Move_At(GLfloat ntheta, GLfloat nphi)
{
	theta = ntheta;
	phi = nphi;
}

void Camera::Move_At(float nx, float ny, float nz)
{
	at += vec4(nx, ny, nz, 0.0);
}

void Camera::Move_Up(vec4 nup)
{
	up += nup;
}

void Camera::Move_Up(float nx, float ny, float nz)
{
	up += vec4(nx, ny, nz, 0.0);
}

void Camera::Reverse()
{
	at.z *= -1;
}

void Camera::Update_Camera()
{
	float deltax = eye.x + ((radius)*sin(theta));
	float deltay = eye.y + ((radius)*sin(phi));
	float deltaz = eye.z + (-1*(radius)*cos(theta)*cos(phi));

	at = vec4(deltax, deltay, deltaz, 1.0);

	camera_view = LookAt(eye, at, up);
	glUniformMatrix4fv(camLoc, 1, GL_TRUE, camera_view);
}

vec4 Camera::Get_Eye()
{
	return eye;
}

vec4 Camera::Get_At()
{
	return at;
}

vec4 Camera::Get_Up()
{
	return up;
}