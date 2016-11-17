#include "room.h"

Room::Room()
{
	offset = vec3(0.0, 0.0, 0.0);
	myCube = new Cube();
	nextRoom = NULL;
	prevRoom = NULL;
	
	doorAngle = 0.0;
	doorAngleIncr = 0.0;
	open = false;
	build_Room();
}

Room::Room(vec3 noffset, Cube *ncube, Room *Prev, Room *Next)
{
	offset = noffset;
	myCube = ncube;

	nextRoom = Next;
	prevRoom = Prev;

	doorAngle = 0.0;
	doorAngleIncr = 0.0;
	open = false;
	build_Room();
}

void Room::build_Room()
{
  GLfloat thickness = 0.06;
  GLfloat normal = 2.0;
  GLfloat translation = normal/2.0;
  GLfloat sidew = 0.35*normal;
  GLfloat doorw = 0.375*normal;
  GLfloat topw = 0.875*normal;

  vec3 thinx = vec3(thickness, normal, normal);
  vec3 thiny = vec3(normal, thickness, normal);
  vec3 thinz = vec3(normal, normal, thickness);

  //floor
  myCube->transform((vec3(0.0, 0.0, 0.0)+offset), vec3(0.0, 0.0, 0.0), thiny);
  floor = new Cube((*myCube));
  floor->color(0.0, 0.0, 1.0);
  myCube->Identity();

  //right wall
  myCube->transform((vec3(translation, translation, 0.0)+offset), vec3(0.0, 0.0, 0.0), thinx);
  right = new Cube((*myCube));
  right->color(0.6, 0.0, 0.3);
  myCube->Identity();

  //left wall
  myCube->transform((vec3(-translation, translation, 0.0)+offset), vec3(0.0, 0.0, 0.0), thinx);
  left = new Cube((*myCube));
  left->color(0.6, 0.0, 0.3);
  myCube->Identity();

  //back wall
  myCube->transform((vec3(0.0, translation, -translation)+offset), vec3(0.0, 0.0, 0.0), thinz);
  back = new Cube((*myCube));
  back->color(0.6, 0.0, 0.3);
  myCube->Identity();

  //ceiling
  myCube->transform((vec3(0.0, normal, 0.0)+offset), vec3(0.0, 0.0, 0.0), thiny);
  ceiling = new Cube((*myCube));
  ceiling->color(0.6, 0.6, 0.6);
  myCube->Identity();

  //left of front wall
  myCube->transform((vec3(-sidew, translation, translation)+offset), vec3(0.0, 0.0, 0.0), vec3(0.6, normal, thickness));
  frontleft = new Cube((*myCube));
  frontleft->color(0.0, 0.3, 0.3);
  myCube->Identity();

  //right of front wall
  myCube->transform((vec3(sidew, translation, translation)+offset), vec3(0.0, 0.0, 0.0), vec3(0.6, normal, thickness));
  frontright = new Cube((*myCube));
  frontright->color(0.0, 0.3, 0.3);
  myCube->Identity();

  //top of front wall
  myCube->transform((vec3(0.0, topw, translation)+offset), vec3(0.0, 0.0, 0.0), vec3(0.8, 0.5, thickness));
  fronttop = new Cube((*myCube));
  fronttop->color(0.0, 0.3, 0.3);
  myCube->Identity();

  //Door
  myCube->transform((vec3(0.0, doorw, translation)+offset), vec3(0.0, 0.0, 0.0), vec3(0.8, 1.5, thickness));
  door = new Cube((*myCube));
  door->color(0.5, 0.5, 0.0);
  myCube->Identity();

  draw();
}

void Room::draw()
{
	left->draw();
	right->draw();
	ceiling->draw();
	frontleft->draw();
	frontright->draw();
	fronttop->draw(); 
	floor->draw();

	Move_Door();

	if(nextRoom == NULL)
	{
		back->draw();
	}
}


bool Room::collision(vec3 point, vec3 movement)
{

	return (left->Check_Collision(point, movement) ||
		right->Check_Collision(point, movement) ||
		frontleft->Check_Collision(point, movement) ||
		frontright->Check_Collision(point, movement) ||
		(!open && door->Check_Collision(point, movement)) ||
		(nextRoom == NULL && back->Check_Collision(point, movement)));
}

bool Room::Enter_Door(vec3 point, vec3 movement)
{
	bool entered =(open && door->Check_Collision(point, movement));

	return entered;
}

void Room::Connect_Rooms(Room *Next = NULL, Room *Prev = NULL)
{
	nextRoom = Next;
	prevRoom = Prev;
}

void Room::Toggle_Door(vec3 position)
{


    if(door->Check_Collision(position, vec3(0.0, 0.0, -5.0)))
    {
	std::cout << "position = " << position << std::endl;
	std::cout << "door location = " << door->get_location() << std::endl;
		if(open)
		{
			doorAngleIncr = -0.35;
		}
		else
		{
			doorAngleIncr = 0.35;
		}
	}
}

void Room::Move_Door()
{
  	static GLint lasttime = glutGet(GLUT_ELAPSED_TIME);
  	GLint time = glutGet(GLUT_ELAPSED_TIME);

	//door is fully open, so stop moving it
	if(doorAngle >= 90.0 &&  !open)
	{
		open = true;
		doorAngleIncr = 0.0;
	}
	else if(doorAngle <= 0.0 && open) //door is closed so stop moving
	{
		open = false;
		doorAngleIncr = 0.0;
	}

  	float angle = doorAngleIncr;
	doorAngle += angle;

	vec3 translation = door->get_translation();
	vec3 scale = vec3(door->get_scale().x/2, 0.0, 0.0);
	vec3 change = translation - scale;


	door->trans(-change);
	door->rot(vec3(0.0, -angle, 0.0));
	door->trans(change);
	door->draw();
}



bool Room::Get_Open()
{
	return open;
}


