# Pac-Man

## Thoughts and decisions

### General
Tried to rename things to be more self-explanatory.

Refactored overly wordy parts to be more compact and readable.

Introduced C++11 shorthand in iterations, for improved readability foremost.

There were a lot of magic numbers. Extracted them to instructively named constants. Needs more work though. Some of them are defined twice in different places.

Replaced raw pointers with smart ones.

Handled positions as units, using vector classes rather than separate variables for x and y. Added an integer vector for grid positions.

I’d like to make the movements smoother, easing in and out when starting and stopping, preferably using simple physics with driving and damping forces.

### Engine
Images were being loaded in every draw call. Replaced the drawing functionality with a small engine that preloads images, and renders them as textures using OpenGL 3. It also handles text using a bitmap font, as well as user input. I’d like to implement keyboard input better though, using an enum for key codes, or something. Now it’s just some arbitrary integers.

### States
Created 3 game states, separating intro, gameplay and game over.

### Map
The map file was being parsed 3 times on initialization. Reduced it to one time, creating tiles and dots at once.

Tiles were stored as a list, being traversed a lot when searching. Changed it to a 2D matrix instead, for direct access via grid positions.

I would like to have objects, like dots, tied to the tiles they reside on, rather than being stored in separate lists. Then, seeing if the avatar’s reached a dot, would be a much simpler matter of looking what’s on the tile the avatar is at, rather than traversing the lists of objects every frame.

I also don’t like that the methods checking whether the avatar is close enough to eat a dot, World::HasIntersectedDot and World::HasIntersectedBigDot, actually delete the dots. Firstly, I’d prefer it to be a single method for checking intersected objects, and I’d like to have it only do what its name says, and handle the removal of objects in Play::Update or something. Or, at least, rename the function to say that it also deletes objects.

### Avatar
Made the avatar face its moving direction. Animated the mouth while in motion, leaving it open while still.

Added a short pause when encountering a ghost, before respawning. Would like to have some animation there as well.

Would like to make controlling the avatar smoother. It feels a bit jagged as it is.

### Ghosts
Made the 4 ghosts spawn on specific positions, and have their individual colors.

I’d like to make the ghosts use a better path finding algorithm, something like A*.
Also, I’d like to make them slowly avoid the avatar when vulnerable. Now they just keep on chasing it as in the normal mode.

### Dependencies
SDL2-2.0.9  
SDL2_ttf-2.0.14  
SDL2_image-2.0.4  
freeglut  
glew-2.1.0  
glm
