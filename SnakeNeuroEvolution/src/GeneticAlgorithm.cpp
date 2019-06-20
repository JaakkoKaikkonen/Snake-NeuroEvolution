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


void GeneticAlgorithm::crossOver() {

	//cross over
	/*std::vector<NeuralNetwork> newPopulation;

	newPopulation.assign(population.size(), NeuralNetwork(population.at(0).weights1.cols, population.at(0).bias_hidden.rows, population.at(0).bias_output.rows));

	for(int i = 0; i < scores.size(); i++) {
		scores.at(i) = pow(scores.at(i), 10);
	}

	int totalScore = 0;
	for (int i = 0; i < scores.size(); i++) {
		totalScore += scores.at(i);
	}
	for (int i = 0; i < scores.size(); i++) {
		scores.at(i) /= totalScore;
	}


	//std::cout << "Population size:" << population.size() << std::endl;
	//std::cout << "new Population size: " << population.size() << std::endl;
	//std::cout << "Scores.size(): " << scores.size() << std::endl;

	for(int i = 0; i < newPopulation.size(); i++) {
		//Pick
		//Weights 1
		for(int j = 0; j < population.at(0).weights1.rows * population.at(0).weights1.cols; j++) {

			int index = 0;
			float r = ((float)rand() / (float)RAND_MAX);

			if(r == 1) {
				r -= 0.01f;
			} else if(r == 0) {
				r += 0.01f;
			}
			//std::cout << r << std::endl;

			while(r > 0) {
				r = r - scores.at(index);
				index++;
			}
			index--;

			newPopulation.at(i).weights1.set(j, population.at(index).weights1.get(j));
		}
		//Weights 2
		for(int j = 0; j < population.at(0).weights2.rows * population.at(0).weights2.cols; j++) {

			int index = 0;
			float r = ((float)rand() / (float)RAND_MAX);

			if(r == 1) {
				r -= 0.01f;
			} else if(r == 0) {
				r += 0.01f;
			}

			while(r > 0) {
				r = r - scores.at(index);
				index++;
			}
			index--;

			newPopulation.at(i).weights2.set(j, population.at(index).weights2.get(j));
		}
		//Bias hidden
		for(int j = 0; j < population.at(0).bias_hidden.rows * population.at(0).bias_hidden.cols; j++) {

			int index = 0;
			float r = ((float)rand() / (float)RAND_MAX);

			if(r == 1) {
				r -= 0.01f;
			} else if(r == 0) {
				r += 0.01f;
			}

			while(r > 0) {
				r = r - scores.at(index);
				index++;
			}
			index--;

			newPopulation.at(i).bias_hidden.set(j, population.at(index).bias_hidden.get(j));
		}
		//Bias output
		for(int j = 0; j < population.at(0).bias_output.rows * population.at(0).bias_output.cols; j++) {

			int index = 0;
			float r = ((float)rand() / (float)RAND_MAX);

			if(r == 1) {
				r -= 0.01f;
			} else if(r == 0) {
				r += 0.01f;
			}

			while(r > 0) {
				r = r - scores.at(index);
				index++;
			}
			index--;
		
			newPopulation.at(i).bias_output.set(j, population.at(index).bias_output.get(j));
		}
	}


	population = newPopulation;*/

	//Copy best one
	int bestIndex = 0;
	int highScore = 0;
	for (int i = 0; i < population.size(); i++) {
		if(scores.at(i) > highScore) {
			highScore = scores.at(i);
			bestIndex = i;
		}
	}

	for (int i = 0; i < population.size(); i++) {
		population.at(i) = population.at(bestIndex);
	}
}

void GeneticAlgorithm::mutate(float mutationRate) {
	for (int i = 2000; i < population.size(); i++) {
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
