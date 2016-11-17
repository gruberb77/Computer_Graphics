// animate.cc
// This Program is a basic game that will allow user to fly a plane and drop food to people below
// blue characters are good characters, hitting one will add a point, hitting a red character will remove points
// 
//
// Written by Brent Gruber, taken from Animate code written by David Chelberg
//
//  last-modified: Wed Oct 12
//
#include <Angel.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <string>
#include <sstream>
#include "rectangle.h"
#include "circle.h"
#include "triangle.h"

using std::cout;
using std::endl;
using std::list;
using std::string;

// Bool to determine whether to clear the screen
bool clear=true;

//Structure to hold a Plane object made of three circles
struct Plane{
	Circle *Body;
	Circle *Wings;
	Circle *Tail;
};

//Structure to hold a tree object, made of a rectangle and a triangle
struct Tree{
	Rectangle *Trunk;
	Triangle  *Leaves;
};

//Structure to hold a person object, just a colored rectangle
struct Person{
	bool good;
	bool direction;
	Rectangle *human;
};


// Locations of variables in shaders.
// Offset of square
GLint offsetLoc;
// Size of square
GLint sizeLoc;
// Color of square 
GLint colorLoc;

// Pointers to the objects (must be initialized after
// initialization of shaders, so leave it as a pointer
Plane * MyPlane;
Tree * Trees;
Person * People;
Rectangle * Ground;

// Some stuff to keep track of the state of the game
int numpeople;
int rnd = 1;
int score = 0;
int drops = 10;

// list containing all the food particles on the screen
// need to keep track in order to detect collisions
list<Circle> food;


// Data storage for points to draw objects
vec2 *points;

// Window Size 
GLint windowSizeLoc;

// Mouse location
int mouse_x=0;
int mouse_y=0;
// Window size
int win_h=0;
int win_w=0;


void reinit();

//returns a random vec2 size (x, y) between 0 and 100
vec2 randsize()
{
  float random = (rand() * 33);
  random = (rand() % 40) + 1;
  vec2 ret = (random, random);
  return ret;
}

//This method will update the score and game information across the window title
void update_score()
{
	
    std::ostringstream ss (std::ostringstream::ate);
    ss << "Round " << rnd << " Score: " << score << " Drops Remaining: " << drops;
    string title = ss.str();
    glutSetWindowTitle(title.c_str());
}


// Initialize all the trees on the screen
// This will generate 5 randomly sized trees at
// random places within the screen
void initTrees(GLint offsetLoc, GLint sizeLoc, GLint colorLoc)
{

  int random;
  int tree_y = 50; //height of ground
  vec2 ransize;

  //create array to hold all trees
  Trees = new Tree[5];

  //loop through trees and initialize them all
  for(uint i=0; i<5; i++)
  {
	//get a random location and size
	random = (rand() * 33);
  	random = (rand() % win_w)+1;
  	ransize = randsize();	

	ransize.x = (ransize.x/6)+5;
	ransize.y = ransize.x * 3.5;

	//initialize the tree trunk as a brown rectangle
  	Trees[i].Trunk = new Rectangle(0, points, offsetLoc, sizeLoc, colorLoc);
  	Trees[i].Trunk->change_size(ransize);
  	Trees[i].Trunk->move(random, tree_y);
	Trees[i].Trunk->color(0.7, 0.5, 0.5);

	//initialize the leaves as a green triangle, should sit on top of trunk
	Trees[i].Leaves = new Triangle(Rectangle::NumPoints+Circle::NumPoints, points, offsetLoc, sizeLoc, colorLoc);
	Trees[i].Leaves->change_size(ransize.y*2.5);
	Trees[i].Leaves->move(random, tree_y+ransize.y);
	Trees[i].Leaves->color(0.0, 1.0, 0.0);
  }
}

//Initialize the plane object, initializes to hard coded location
//3 circles make up plane, a body, wings, and a tail
void initPlane(GLint offsetLoc, GLint sizeLoc, GLint colorLoc)
{
  //Create a new Plane and initialize body
  MyPlane = new Plane;
  MyPlane->Body = new Circle(Rectangle::NumPoints, points, offsetLoc, sizeLoc, colorLoc);
  MyPlane->Body->change_size(vec2(70.0, 14.0));
  MyPlane->Body->move(800, 800);

  //initialize wings
  MyPlane->Wings = new Circle(Rectangle::NumPoints, points, offsetLoc, sizeLoc, colorLoc);
  MyPlane->Wings->change_size(vec2(7.0, 49.0));
  MyPlane->Wings->move(815, 800);

  //initialize tail
  MyPlane->Tail = new Circle(Rectangle::NumPoints, points, offsetLoc, sizeLoc, colorLoc);
  MyPlane->Tail->change_size(vec2(7.0, 21.0));
  MyPlane->Tail->move(730, 800);
}

//initialize the ground,
//it is a green rectangle that spans the whole screen
void initGround(GLint offsetLoc, GLint sizeLoc, GLint colorLoc)
{
  Ground = new Rectangle(0, points, offsetLoc, sizeLoc, colorLoc);

  Ground->change_size(vec2(win_w, 50));
  Ground->move(0,0);
  Ground->color(0.0, 1.0, 0.0);
}

//Initialize all the people to drop food to,
//will create a random number of people on the ground
//and give them a direction to start moving
void initPeople(GLint offsetLoc, GLint sizeLoc, GLint colorLoc)
{
    int pos = 100;

    //how many people to generate, random
    numpeople = rand() % 10;
    People = new Person[numpeople];
    
    //initialize every individual
    for(uint i = 0; i<numpeople; i++)
    {
	//create rectangle for current person
	People[i].human = new Rectangle(0, points, offsetLoc, sizeLoc, colorLoc);
	People[i].human->move(vec2(pos, 50));
	People[i].human->change_size(vec2(8, 17));
	People[i].direction = rand() % 2;

	//randomly generate whether person is a good guy or bad guy
	//1 = good guy, 0 = bad guy
	if((rand() % 10) > 5)
	{
	    People[i].good = 1;
	    People[i].human->color(1.0, 0.0, 0.0);
	}
	else
	{
	    People[i].good = 0;
	    People[i].human->color(0.0, 0.0, 1.0);
	}
	pos += 200;
    }
}

//Check for collisions between food and other objects on screen
//takes as input the location of an object, the size of the object and what type
//of object it is
void check_collision(vec2 object, vec2 size, int type)
{
  //nothing to do if no food
  if(!food.empty())
  {
     Circle first = food.front();
     vec2 pos = first.get_pos();

     //food hit the ground, stop tracking it
     if(pos.y <= 20.0)
     {
	 food.pop_front();
     }

     //calculate distance between food and object in question
     float distx = object.x - pos.x;
     distx = distx*distx;

     //if food is close enough to object we have collision
     if(pos.y <= (object.y + size.y) && distx < (size.x * size.x))
     {
	//stop drawing food
	food.pop_front();
	
	//determine how this affects score
	switch(type)
	{
	    case 1: //good guy
		score++;
		update_score();
		break;
	    case 2: //bad guy
		score--;
		update_score();
		break;
	    case 3: //tree
		break;
	}
     }
  }
}  


// This method will check if the round is over or not
void check_round()
{
  //if there are no drops left and no food on the screen then start next round
  if(drops == 0 && food.empty())
  {
	rnd++;
	update_score();
	reinit();
  }
}


//Move the plane based on an offset passed in
//idea is that there can be powerups by increasing/decreasing offset
//which will make plane move faster/slower
void move_plane(int offset)
{
  vec2 pos = MyPlane->Body->get_pos();
  vec2 size = MyPlane->Body->get_size();

  //at the edge of the screen, prevent moving off
  if((pos.x <= 0 && offset < 0) || (pos.x >= win_w && offset > 0))
  {
    return;
  }   

  //move the plane
  MyPlane->Body->change_x(offset);

  //Orient the plane in whatever direction it is moving
  if(offset > 0)
  {
  	MyPlane->Wings->move(pos.x+15+offset, pos.y);
  	MyPlane->Tail->move(pos.x-70+offset, pos.y);
  }
  else
  {
	MyPlane->Wings->move(pos.x-15+offset, pos.y);
	MyPlane->Tail->move(pos.x+70+offset, pos.y);
  }
}

//Drop a piece of food from the location of the plane
//will initiate a piece of food on the screen where the plane is.
void drop_food()
{
	//check if any drops left
	if(drops > 0)
	{
	  //initialize food
	  vec2 location = MyPlane->Body->get_pos();
	  Circle myFood(Rectangle::NumPoints, points, offsetLoc, sizeLoc, colorLoc);
	  myFood.move(location);
	  myFood.change_size(vec2(8.0,8.0));
	  myFood.color(0.25, 0.5, 0.25);
	  food.push_back(myFood);

	  //update drops and update window title
	  drops--;
	  update_score();
	}
}

//This method will move people around the screen,
//every character has a direction in which they will be moving
//May be cool to add individualized speed, or some form of AI
void move_people()
{
	vec2 current;
	float speed = 0.2;	

	//loop through all the people and move them by some offset
	for(uint i=0; i<numpeople; i++)
	{
	    vec2 pos = People[i].human->get_pos();
	    if(People[i].direction > 0)
	    {
		if(pos.x < float(win_w)-0.1)
		{
			People[i].human->change_x(speed);
		}
		else
		{
			//hit edge of screen, turn around
			People[i].direction = 0;
			People[i].human->change_x(speed*-1);
		}
	    }
	    else
	    {
		if(pos.x > 0.1)
		{
			People[i].human->change_x(speed*-1);
		}
		else
		{
			//hit edge of screen, turn around
			People[i].direction = 1;
			People[i].human->change_x(speed);
		}
	    }
	}
}


//move food towards bottom of screen
//loops through every piece of food on the screen and moves it closer to bottom
void update_food()
{
  for(list<Circle>::iterator it=food.begin(); it != food.end(); ++it)
  {
     Circle circ = *it;
     (*it).change_y(-1.0);
     (*it).draw();
  }
}

// display function that will draw all objects
extern "C" void display()
{
  if (clear) {
    glClear(GL_COLOR_BUFFER_BIT);
  }

  vec2 pos;
  vec2 size;
 
  //draw ground
  //offset getting messed up in sub classes, not being sent to gpu correctly
  //seems to have something to do with float/int discrepancies, for now just send to gpu here
  glUniform2i(offsetLoc,0,0); 
  Ground->draw();

  //Draw the plane
  MyPlane->Body->draw();
  MyPlane->Wings->draw();
  MyPlane->Tail->draw();

  //draw the trees
  //check for collisions here rather than looping through again later
  for(uint i=0; i<5; i++)
  {
	pos = Trees[i].Trunk->get_pos();
	glUniform2i(offsetLoc, pos.x, pos.y);
	Trees[i].Trunk->draw();
	Trees[i].Leaves->draw();
	pos = Trees[i].Leaves->get_pos();
	size = Trees[i].Leaves->get_size();
	check_collision(pos, size, 3);
  }

  //draw the people
  //check for collisions here rather than looping through again later
  for(uint i=0; i<numpeople; i++)
  {
	pos = People[i].human->get_pos();
	glUniform2i(offsetLoc, pos.x, pos.y);
	People[i].human->draw();
	size = People[i].human->get_size();
	check_collision(pos, size, int(People[i].good + 1));
  }

  //move and draw food
  update_food();

  glFlush();
}

//Idle callback, constantly checking lowest food for collision with ground, moving people, and checking if the round is over
//then redraws the screen
extern "C" void idle()
{
  move_people();
  check_collision(vec2(0,0), 0.0, 3);
  check_round();
  glutPostRedisplay();
}


// Maintains the mapping from screen to world coordinates.
extern "C" void myReshape(int w, int h)
{
  glViewport(0,0,w,h);
  win_h = h;
  win_w = w;
  glUniform2f(windowSizeLoc, win_w, win_h);       // Pass the window size
						  // size
  Ground->change_size(vec2(win_w, 50));

  glutPostRedisplay();
}


// Special Keys events.
// Left and Right arrows will move plane, down arrow drops food
extern "C" void special(int k, int xx, int yy)
{
  int speed = 10;

  if (k==GLUT_KEY_UP) {
  } else if (k==GLUT_KEY_DOWN) {
    drop_food();
  } else if (k==GLUT_KEY_LEFT) {
    move_plane(speed*-1);
  } else if (k==GLUT_KEY_RIGHT) {
    move_plane(speed);
  }

}

// Initialize all OpenGL callbacks, create window.

void myinit()
{
  win_w = 900;
  win_h = 900;

  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(win_w,win_h);
  glutInitWindowPosition(20,20);

  glutCreateWindow("Start Game, 10 drops remaining");

  // Color initializations
  glClearColor(0.0, 0.0, 0.0, 1.0);

  // Callbacks
  glutDisplayFunc(display); 
  glutIdleFunc(idle); 
  glutReshapeFunc (myReshape);
  glutSpecialFunc(special);
}

// This function initializes the buffers and shaders
// Initializes all objects that will be displayed on screen
void init()
{


  // Create a vertex array object on the GPU
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create and initialize a buffer object for transferring data to
  // the GPU.
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshader.glsl", "fshader.glsl");

  // InitShader does the glUseProgram
  // So the following would be redundant
  // glUseProgram(program);

  // Initialize the vertex position attribute from the vertex shader
  windowSizeLoc = glGetUniformLocation(program, "windowSize");
  if (windowSizeLoc==-1) {
    std::cerr << "Unable to find windowSize parameter" << std::endl;
  }
  offsetLoc  = glGetUniformLocation(program, "offset");
  if (offsetLoc==-1) {
    std::cerr << "Unable to find offsetLoc parameter" << std::endl;
  }
  sizeLoc  = glGetUniformLocation(program, "size");
  if (sizeLoc==-1) {
    std::cerr << "Unable to find sizeLoc parameter" << std::endl;
  }
  colorLoc  = glGetUniformLocation(program, "vcolor");
  if (colorLoc==-1) {
    std::cerr << "Unable to find colorLoc parameter" << std::endl;
  }
  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glUniform2f(offsetLoc, 0.0, 0.0);               // Initial offset
						  // for mouse loc.
  points = new vec2[Rectangle::NumPoints+Circle::NumPoints+Triangle::NumPoints];
  //  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  initGround(offsetLoc, sizeLoc, colorLoc);
  initPeople(offsetLoc, sizeLoc, colorLoc);
  initTrees(offsetLoc, sizeLoc, colorLoc);
  initPlane(offsetLoc, sizeLoc, colorLoc);

  // Send the data to the graphics card, after it has been generated
  // by creating the objects in the world (above).
  glBufferData(GL_ARRAY_BUFFER, (Rectangle::NumPoints+Circle::NumPoints+Triangle::NumPoints)*sizeof(vec2), points, GL_STATIC_DRAW);

}

//Reinitialize the game, used to start a new round
void reinit()
{
  drops = 10;

  initGround(offsetLoc, sizeLoc, colorLoc);
  initPeople(offsetLoc, sizeLoc, colorLoc);
  initTrees(offsetLoc, sizeLoc, colorLoc);
  initPlane(offsetLoc, sizeLoc, colorLoc);
}



//  Main Program
int main(int argc, char** argv)
{
  srand(time(NULL));


  // Several people forgot to put in the following line.  This is an
  // error, even if it still works on your machine, a program is
  // incorrect without the following call to initialize GLUT.
  glutInit(&argc,argv);

  myinit();             // set window attributes, and initial data

  // Initialize the "magic" that glues all the code together.
  glewInit();

  init();               // set up shaders and display environment

  glutMainLoop();       // enter event loop

  return(EXIT_SUCCESS); // return successful exit code
}
