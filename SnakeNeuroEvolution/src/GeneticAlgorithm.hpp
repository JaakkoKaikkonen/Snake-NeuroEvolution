#pragma once
#include "NeuralNetwork.hpp"
#include <vector>

class GeneticAlgorithm
{
public:
	GeneticAlgorithm(NeuralNetwork neuralNetwork, int generationSize);

	void crossOver();
	void mutate(float mutationRate);

	void loadFromFile(char* snakeFileName);

public:
	std::vector<NeuralNetwork> population;
	std::vector<float> scores;
};

