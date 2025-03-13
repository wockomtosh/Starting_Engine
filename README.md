This is an outward facing repository of a game engine project I completed over the course of 2 semesters (with some gaps).
In this engine I implemented:
1. A game object-component system, including controllers and factory methods
2. Creating and loading game objects using JSON
3. Elastic and blocking collisions through checking axes of separation and open/close times for those axes
4. Basic physics using rigidbodies

For graphics and rendering it uses a graphics library called GLib. 
Unfortunately, because of several compatibility issues that I ran into while using GLib, only one sprite was usable in the project and the game cannot process inputs while in the Debug configuration. The Release configuration is able to process inputs.

The majority of the "game" code created for this engine was made for the purpose of testing various engine features. As such, it only comes with a very simple pong-like demo. To play this demo, use w and s to move the left-hand player and i and k to move the right-hand player.
