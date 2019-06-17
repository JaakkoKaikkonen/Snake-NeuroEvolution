#pragma once
#include "NeuralNetwork.hpp"
#include <vector>

#include "ImGuiLog.h"

class GeneticAlgorithm
{
public:
	GeneticAlgorithm(NeuralNetwork neuralNetwork, int generationSize);

	void crossOver();
	void mutate(float mutationRate);

	void loadFromFile(char* snakeFileName, ImGuiLog& ImGuiLog);

public:
	std::vector<NeuralNetwork> population;
	std::vector<int> scores;
};

