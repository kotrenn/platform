This is a spatial platformer shooter.  To elaborate, the player
controls an entity that can move around and shoot bullets at
enemies in a similar fashion to platform games of the early 90's.
In addition, there is a 3D element involved--the player can
rotate the camera in two ways.  The first of which is to rotate
the screen by 90 degrees, which alters gravity.  The second is
to rotate the plane on which the player can move around in.
Specifically, the player will be able to move along two different
axes from before--if the player was moving in the x- and y-
directions, they could now move in the x- and z- directions.
This leads to an interesting experience for four levels.



--Gameplay--

Controls:
Arrow keys - move
Z - shoot
X - jump
A - rotate gravity left
S - rotate gravity right
Q - rotate camera left
W - rotate camera right
P - pause


The exit to the next level is a white block.  Player's health is
represented by red blocks around the player--each red block is
one hit point.  By not getting hit the player can slowly heal.
In addition enemy health is represented by the same system, but
enemies do not heal.  When hit, the player is momentarily
invincible.  This also does not apply to enemies.

Level 4 is also special in that exits will not appear until all
of the enemies are destroyed.



--Technical Details--

The game is written in C++ and uses the OpenGL and SDL libraries
for rendering, input, and output.  A simple game loop
architecture was used, with actors (entities) being updated every
frame.  Collision detection was simplified by just having actors
look at the nearby voxels (boxes) to check for collision, and if
so, update as necessary.  No spatial structure was used on
bullets--any actor checked for collisions with all bullets made
by an enemy.  The bullet count was low enough for this to be
manageable without a loss in performance.

Levels were stored in a custom text format, with one character
per block.  A header for levels denote what type each block was,
with texture material specified.  Python scripts were used to
generate three out of the four levels, by essentially "rendering"
the level in ASCII (the custom level file).

A configurable data file (data.txt) is used to store many of
the game's constants.  This includes camera dimensions, gravity,
player and enemy strength, and enabling of a couple "cheats."

Some tricks were used to render only the few blocks in front of
the player.  Specifics can be found in layout.cpp under
Layout::draw().

The physics engine for gravity was slightly hacked together--
there is just a simple acceleration in whatever direction is
"down" at the time.  Some constants were hand tuned to make
the game appear to run smoothly.

There was an emphasis on trying to split classes up to only
manage simple tasks at a time.  This was partially motivated
by being a first medium sized project using OpenGL.  In
addition, there was some consideration of expanding this
project into a full game, so there are several places where
attempts were made to make the game extendable.



--Credits--

The idea for the rotation of the camera to different axes was
based on Fez by Phil Fish.  I added in the other rotation to
make something I thought would be interesting to play around
in tandem with the Fez mechanic, and has likely been used in
several games in the past.

Sounds were generated with SFXR, a free tool for generating
sound effects for games.

This was made as a final project for a graduate course on
computer graphics at North Carolina State University under the
instruction of Dr. Benjamin Watson.  The final project had an
option to come up with your own, which I chose and planned out
a project I could do in the time frame.
