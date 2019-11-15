# Snake-NeuroEvolution

Neural Network learns to play snake using a Genetic algorithm.  
(High score: 179)

![alt text](https://github.com/JaakkoKaikkonen/Snake-NeuroEvolution/blob/master/Snake.gif)

Made with C++ using SFML and ImGui libraries.

The Neural Network has 12 input, 24 hidden, 4 output neurons.

inputs (1, 2, 3, 4) are distances to snakes tail from its head in 4 directions.  
inputs (5, 6, 7, 8) are distances to walls from snakes head in 4 directions.  
inputs (9, 10, 11, 12) are 1 or 0 depending on if there is food in that direction.

Outputs (1, 2, 3, 4) are (move right, move left, move up, move down).
