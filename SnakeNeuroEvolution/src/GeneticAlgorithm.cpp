#include "GeneticAlgorithm.hpp"
#include <fstream>


GeneticAlgorithm::GeneticAlgorithm(NeuralNetwork neuralNetwork, int generationSize)
{
	population.assign(generationSize, neuralNetwork);
	for (int i = 1; i < population.size(); i++) {
		population.at(i).randomize();
	}

	scores.reserve(generationSize);
	for (int i = 0; i < generationSize; i++) {
		scores.push_back(0);
	}
}


void GeneticAlgorithm::nextGen() {

	//Copy best one
	/*int bestIndex = 0;
	int highScore = 0;
	for (int i = 0; i < population.size(); i++) {
		if (scores.at(i) > highScore) {
			highScore = scores.at(i);
			bestIndex = i;
		}
	}

	for (int i = 0; i < population.size(); i++) {
		population.at(i) = population.at(bestIndex);
	}*/

	
	//Select from old population with change assigned by score
	int highScoreIndex = 0;
	int totalScore = 0;
	for (int i = 0; i < scores.size(); i++) {
		totalScore += scores.at(i);
		if (scores.at(i) > scores.at(highScoreIndex)) {
			highScoreIndex = i;
		}
	}

	std::vector<NeuralNetwork> tempPopulation;
	tempPopulation.reserve(GENERATION_SIZE);

	tempPopulation.assign(GENERATION_SIZE * 0.2f, population.at(highScoreIndex));

	for (int i = GENERATION_SIZE * 0.2f; i < population.size(); i++) {
		int randNum = rand() / (float)RAND_MAX * totalScore;

		int counter = 0;
		int index = -1;
		
		do {
			index++;
			counter += scores.at(index);
		} while (counter < randNum);

		tempPopulation.push_back(population.at(index));

	}

	population = tempPopulation;

}

void GeneticAlgorithm::mutate(float mutationRate) {
	for (int i = GENERATION_SIZE * 0.1f; i < population.size(); i++) {
		for (int j = 0; j < population.at(i).weights1.rows * population.at(i).weights1.cols; j++) {
			if (((float)rand() / (float)RAND_MAX) < mutationRate) {
				population.at(i).weights1.set(j, (((float)rand() / (float)RAND_MAX) * 2) - 1);
			}
		}
		for (int j = 0; j < population.at(i).weights2.rows * population.at(i).weights2.cols; j++) {
			if (((float)rand() / (float)RAND_MAX) < mutationRate) {
				population.at(i).weights2.set(j, (((float)rand() / (float)RAND_MAX) * 2) - 1);
			}
		}
		for (int j = 0; j < population.at(i).bias_hidden.rows * population.at(i).bias_hidden.cols; j++) {
			if (((float)rand() / (float)RAND_MAX) < mutationRate) {
				population.at(i).bias_hidden.set(j, (((float)rand() / (float)RAND_MAX) * 2) - 1);
			}
		}
		for (int j = 0; j < population.at(i).bias_output.rows * population.at(i).bias_output.cols; j++) {
			if (((float)rand() / (float)RAND_MAX) < mutationRate) {
				population.at(i).bias_output.set(j, (((float)rand() / (float)RAND_MAX) * 2) - 1);
			}
		}
	}
}


void GeneticAlgorithm::loadFromFile(char* snakeFileName, ImGuiLog& ImGuiLog) {

	ImGuiLog.AddLog(("Load \"" + std::string(snakeFileName) + "\"\n").c_str());

	std::ifstream f(snakeFileName, std::ios::binary);

	if (f.good()) {

		Matrix<float> loaded_weights1(population.at(0).weights1.rows, population.at(0).weights1.cols);
		Matrix<float> loaded_weights2(population.at(0).weights2.rows, population.at(0).weights2.cols);
		Matrix<float> loaded_bias_hidden(population.at(0).bias_hidden.rows, population.at(0).bias_hidden.cols);
		Matrix<float> loaded_bias_output(population.at(0).bias_output.rows, population.at(0).bias_output.cols);


		f.read(reinterpret_cast<char*>(loaded_weights1.data), (loaded_weights1.rows * loaded_weights1.cols) * sizeof(float));
		f.read(reinterpret_cast<char*>(loaded_weights2.data), (loaded_weights2.rows * loaded_weights2.cols) * sizeof(float));
		f.read(reinterpret_cast<char*>(loaded_bias_hidden.data), (loaded_bias_hidden.rows) * sizeof(float));
		f.read(reinterpret_cast<char*>(loaded_bias_output.data), (loaded_bias_output.rows) * sizeof(float));

		f.close();

		for (int i = 0; i < population.size(); i++) {
			population.at(i).weights1 = loaded_weights1;
			population.at(i).weights2 = loaded_weights2;
			population.at(i).bias_hidden = loaded_bias_hidden;
			population.at(i).bias_output = loaded_bias_output;
		}

		scores.assign(scores.size(), 0);

	} else {
		ImGuiLog.AddLog("Bad file\n");
	}
	

}
