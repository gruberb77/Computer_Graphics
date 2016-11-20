I chose to base all of the shapes for my program on cubes, for ease of implementation.  I created a cube class that most of the objects are based on.  I also created a room class that uses cubes for every wall of the room, I used cubes instead of planes because I wanted the walls to have thickness.  I created a class for the camera that will affect all the camera movements.  I would have liked to implement each wall as it's own class, so that I would have been able to make the rooms more unique, i.e. have different classes (plain wall, windowed wall, doored wall, etc), however right now all the rooms look the same.  The doors are able to open as long as the player is looking at them.  The player cannot walk through any walls or doors, unless they are open.

I control the position of the character and camera using the wasd keys, and what the camera is looking at is controlled by the passive motion function of the mouse, so no buttons will be pressed.  There are a random number of ghosts in the house that will follow the players z position, and can walk through walls.  

Controls:
	w - forward
	s - backward
	a - strafe left
	d - strafe right
	mouse - move camera
	left mouse button will open a door while looking at it, also spins ceiling fan
	right mouse button switch between first and third person

