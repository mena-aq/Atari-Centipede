# Atari-Centipede
Recreating Atari's Centipede arcade game using C++ and SFML.

# Description
My first semester coding project, where I had to recreate Atari's Centipede Arcade Game using C++ and SFML. SFML was used for graphic/audio purposes, whereas the logic was implemented using simple C++ beginner concepts.  
In this game, the player fires bullets at a centipede as it makes its way down the game window, when it hits the edge of the window or a mushroom. Whenever the player hits a segment, it is destroyed and a mushroom is left behind. The centipede also splits into 2 centipedes at this point. This continues until all segments are destroyed. The game contains levels where the centipede speeds up and more mushrooms spawn at the beginning of gameplay. The player earns points for shooting different game objects.
For more information: [retrogamedeconstructionzone](https://www.retrogamedeconstructionzone.com/2020/08/the-characters-of-centipede.html)

# Executing the program
Make sure GNU g++ compiler is installed in the system. Also install SFML.
 ## To compile:
 ```
g++ -c Centipede.cpp
```
```
g++ Centipede.o -o sfml-app -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
```
## To run the game:
```
./sfml-app
```
# Version History 
* 0.1
  * Initial release
  * Functionality to record scores for future gameplay not yet added

  ## Enjoy your gameplay !!
  ![image](https://github.com/mena-aq/Atari-Centipede/assets/156236313/d47eafb9-b3b6-4c50-8839-38860b23c8d2)

