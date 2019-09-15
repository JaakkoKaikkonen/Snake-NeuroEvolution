# Snake-NeuroEvolution

Neural Network that learns to play snake using a Genetic algorithm.

![alt text](https://github.com/JaakkoKaikkonen/Snake-NeuroEvolution/blob/master/Snake.gif)

Made with C++ using SFML and ImGui libraries.

The Neural Network has 12 input, 24 hidden, 4 output neurons.

First 4 inputs are the distances to its tail from its head in right, left, up and down directions.  
Second 4 are the distances to walls from its head in 4 directions.  
Third 4 tell if there is food in any of the 4 directions.  
(I didn't give it the distance to food cause it trained faster with it being just on/off)  

Outputs are: move right, move left, move up, move down.
