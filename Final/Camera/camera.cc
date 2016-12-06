//******************************************************************* 
//                                                                    
//  File:        Camera                                          
//                                                                     
//  Author:      Brent Gruber
//  Email:       bg240711@ohio.edu
//                                                                    
//
//                                                                    
//  Description: This is the implementation of a class that contains a camera object, for 3d space               
//                                                                    
//  Date:        11/1/2016
//                                                                    
//*******************************************************************


#include "camera.h"

//******************************************************************
//                                                                  
//  Function:   Camera
//                                                                  
//  Purpose:    default constructor                      
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: new instance of camera is defined
//                                                                  
//******************************************************************
Camera::Camera()
{
	eye = vec4(0.0, 0.5, 3.0, 1.0);
	at = vec4(0.0, 0.0, -1.0, 1.0);
	up = vec4(0.0, 1.0, 0.0, 0.0);
	offset = vec3(0.0, 0.0, 2.0);
	radius = 2.0;
	thirdp = false;
}

//******************************************************************
//                                                                  
//  Function:   Camera
//                                                                  
//  Purpose:    constructor                      
//                                                                  
//  Parameters: ncamLoc - location of camera view matrix
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: new instance of camera created
//                                                                  
//******************************************************************
Camera::Camera(GLuint ncamLoc)
{
	eye = vec4(0.0, 0.5, 3.0, 1.0);
	at = vec4(0.0, 0.5, -1.0, 1.0);
	up = vec4(0.0, 1.0, 0.0, 0.0);
	offset = vec3(0.0, 0.25, 2.0);
	radius = 2.0;
	camLoc = ncamLoc;
	thirdp = false;
}

//******************************************************************
//                                                                  
//  Function:   Camera
//                                                                  
//  Purpose:    constructor                     
//                                                                  
//  Parameters: inputs
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: new instance of camera with inputs, created
//                                                                  
//******************************************************************
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

//******************************************************************
//                                                                  
//  Function:   Toggle_Pov
//                                                                  
//  Purpose:    toggle between first and third person                      
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: if in first person, changed to third, else change to first
//                                                                  
//******************************************************************
void Camera::Toggle_Pov()
{
	//if third person change to first
	if(thirdp)
	{
		eye -= offset;
		at.y += 0.5;
	}
	else //if first person change to third
	{
		eye += offset;
		at.y -= 0.5;
	}

	//update flag
	thirdp = !thirdp;
}

//******************************************************************
//                                                                  
//  Function:   Move_Eye
//                                                                  
//  Purpose:    move the location of the eye                  
//                                                                  
//  Parameters: neye - location of new eye
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: eye is moved
//                                                                  
//******************************************************************
void Camera::Move_Eye(vec4 neye)
{
	neye.w = 0.0;
	eye += neye;
}


//******************************************************************
//                                                                  
//  Function:   Move_Eye
//                                                                  
//  Purpose:    move the location of the eye                  
//                                                                  
//  Parameters: nx, ny, nz - location of new eye
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: eye is moved
//                                                                  
//******************************************************************
void Camera::Move_Eye(float nx, float ny, float nz)
{
	eye += vec4(nx, ny, nz, 0.0);
}


//******************************************************************
//                                                                  
//  Function:   Move_At
//                                                                  
//  Purpose:    move where the camera is looking at                
//                                                                  
//  Parameters: nat - location of new at
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: at is moved
//                                                                  
//******************************************************************
void Camera::Move_At(vec4 nat)
{
	at += nat;
}

//******************************************************************
//                                                                  
//  Function:   Move_At
//                                                                  
//  Purpose:    move where the camera is looking at                
//                                                                  
//  Parameters: theta - angle to move camera by xz components
//				phi - angle to move camera by zy components
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: at is moved
//                                                                  
//******************************************************************
void Camera::Move_At(GLfloat ntheta, GLfloat nphi)
{
	theta = ntheta;
	phi = nphi;
}

//******************************************************************
//                                                                  
//  Function:   Move_At
//                                                                  
//  Purpose:    move where the camera is looking at                
//                                                                  
//  Parameters: nx, ny, nz - new location
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: at is moved
//                                                                  
//******************************************************************
void Camera::Move_At(float nx, float ny, float nz)
{
	at += vec4(nx, ny, nz, 0.0);
}

//******************************************************************
//                                                                  
//  Function:   Move_Up
//                                                                  
//  Purpose:    move which direction is up               
//                                                                  
//  Parameters: nup - new up vector
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: up is moved
//                                                                  
//******************************************************************
void Camera::Move_Up(vec4 nup)
{
	up += nup;
}

//******************************************************************
//                                                                  
//  Function:   Move_Up
//                                                                  
//  Purpose:    move which direction is up              
//                                                                  
//  Parameters: nx, ny, nz - new up vector
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: up is moved
//                                                                  
//******************************************************************
void Camera::Move_Up(float nx, float ny, float nz)
{
	up += vec4(nx, ny, nz, 0.0);
}

//******************************************************************
//                                                                  
//  Function:   Reverse
//                                                                  
//  Purpose:    reverse the camera             
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: camera is reversed
//                                                                  
//******************************************************************
void Camera::Reverse()
{
	at.z *= -1;
}

//******************************************************************
//                                                                  
//  Function:   Update_Camera
//                                                                  
//  Purpose:    update the camera view matrix and send to shaders             
//                                                                  
//  Parameters:
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: camera view matrix is sent to shaders
//                                                                  
//******************************************************************
void Camera::Update_Camera()
{
	//get the deltas to be used for trackball emulation
	float deltax = eye.x + ((radius)*sin(theta));
	float deltay = eye.y + ((radius)*sin(phi));
	float deltaz = eye.z + (-1*(radius)*cos(theta)*cos(phi));

	//update the at vector based on sphere calculations
	at = vec4(deltax, deltay, deltaz, 1.0);

	//update camera view matrix and send to shader
	camera_view = LookAt(eye, at, up);
	glUniformMatrix4fv(camLoc, 1, GL_TRUE, camera_view);
}

//******************************************************************
//                                                                  
//  Function:   Get_Eye
//                                                                  
//  Purpose:    return location of eye             
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: location of eye is returned
//                                                                  
//******************************************************************
vec4 Camera::Get_Eye()
{
	return eye;
}

void Camera::Set_Eye(vec4 location)
{
	eye = location;
	thirdp = false;
}

//******************************************************************
//                                                                  
//  Function:   Get_At
//                                                                  
//  Purpose:    return where camera is looking             
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: location of where camera is looking is returned
//                                                                  
//******************************************************************
vec4 Camera::Get_At()
{
	return at;
}

//******************************************************************
//                                                                  
//  Function:   Get_Up
//                                                                  
//  Purpose:    return which direction is up            
//                                                                  
//  Parameters:
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: return which direction is up
//                                                                  
//******************************************************************
vec4 Camera::Get_Up()
{
	return up;
}