#pragma once
#include "NeuralNetwork.h"
#include <vector>

class GeneticAlgorithm
{
public:
	GeneticAlgorithm(NeuralNetwork neuralNetwork, int generationSize);

	void crossOver();
	void mutate(float mutationRate);

public:
	std::vector<NeuralNetwork> population;
	std::vector<float> scores;
};

