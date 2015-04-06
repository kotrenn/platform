Timeline:  Fall 2012

A 3-dimensional platforming game with multiple types of rotations and worlds to explore.
===============================

The main gimmick of the game is that it provides players with two forms of rotation.  The first allows players to rotate the screen by 90 degrees.  This allows players to reach previously inaccessible areas.  Additionally, gravity will change direction appropriately, which can be used strategically.  The second form of rotation allows the player to change the plane of movement.  Similar to the rotation system in Fez, this action will rotate the camera "around" the player, bringing the foreground and background into reach and vice versa.

This project was originally prepared for a graduate course on computer graphics.  As a final project, we were assigned to create a computer game with 3D elements in OpenGL.  Having already developed some experience with game development, I decided to push myself.  SDL was incorporated with OpenGL to provide interfacing with the operating system, particularly with keyboard input.

A simple level description format was included based on voxels.  Each level is simply a 3D rectangular box of different cubes, with opportunities for different cubes to have different textures.  The current sample levels utilize this by having different colored blocks for different areas as a visual aesthetic.  Additionally, the current levels (aside from the first) have matching Python scripts to generate them.  Each one had a different "theme" of style, and designing these by hand would have been impractical.

Long-term plans:
- Textures for different types of blocks such as grass
- Octree or a custom data structure for compressed storage and access of level data
- Network of worlds to explore
- A level editor with controls similar to the Minecraft family of games
- Scripting for creating complex portions of levels
- Gameplay challenges which exploit the rotation system
- Boss fights which exploit the rotation system

Gameplay
----------------------

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


Credits
----------------------

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
