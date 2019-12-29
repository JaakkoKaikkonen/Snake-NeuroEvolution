#pragma once

#define WINDOW_TITLE "Snake"

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700

#define FONT_FILEPATH "Resources/Fonts/font.ttf"

#define TOP_ICON_FILEPATH "Resources/Images/topIcon.png"

#define TILESIZE 25


enum class Dir { Up, Down, Right, Left };


//NeuralNetwork
#define NUM_OF_INPUTS 12
#define NUM_OF_HIDDEN 24
#define NUM_OF_OUTPUT 4

//Genetic algorithm
#define GENERATION_SIZE 20000