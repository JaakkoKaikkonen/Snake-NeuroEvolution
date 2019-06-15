#include "NeuralNetwork.hpp"
#include <math.h>
#include <string>


NeuralNetwork::NeuralNetwork(int _numOfInputs, int _numOfhiddenNodes, int _numOfoutputs)
	: inputs(_numOfInputs, 1), 
	  hiddenNodes(_numOfhiddenNodes, 1), 
	  outputs(_numOfoutputs, 1),
	  weights1(_numOfhiddenNodes, _numOfInputs), 
	  weights2(_numOfoutputs, _numOfhiddenNodes),
	  bias_hidden(_numOfhiddenNodes, 1), 
	  bias_output(_numOfoutputs, 1)
{
	weights1.randomize();
	weights2.randomize();
	bias_hidden.randomize();
	bias_output.randomize();
}


float NeuralNetwork::sigmoid(float num) {
	return (1 / (1 + exp(-num)));
}


int NeuralNetwork::predict(const Matrix<float>& inputs) {


	hiddenNodes = weights1 * inputs;
	hiddenNodes += bias_hidden;
	hiddenNodes.map(NeuralNetwork::sigmoid);

	outputs = weights2 * hiddenNodes;
	outputs += bias_output;
	outputs.map(NeuralNetwork::sigmoid);


	int index = 0;
	float record = 0;
	for (int i = 1; i <= outputs.rows; i++) {
		if (outputs.get(i, 1) > record) {
			record = outputs.get(i, 1);
			index = i;
		}
	}

	return index;
}

void NeuralNetwork::randomize() {
	weights1.randomize();
	weights2.randomize();
	bias_hidden.randomize();
	bias_output.randomize();
}


void NeuralNetwork::save(char* snakeFileName) {

	std::ofstream outputFile;

	if (*snakeFileName == *"") {

		int snakeIndex = 0;
		std::ifstream f;

		do {
			f.close();
			snakeIndex++;
			f.open("snake" + std::to_string(snakeIndex));
		} while (f.good());

		outputFile.open("snake" + std::to_string(snakeIndex), std::ios::binary);

		std::cout << "Save " << "\"" << "snake" + std::to_string(snakeIndex) << "\"" << std::endl;

	} else {

		outputFile.open(snakeFileName, std::ios::binary);

		std::cout << "Save " << "\"" << snakeFileName << "\"" << std::endl;

	}

	if (outputFile.is_open()) {

		outputFile.write(reinterpret_cast<char*>(weights1.data), (weights1.rows * weights1.cols) * sizeof(float));
		outputFile.write(reinterpret_cast<char*>(weights2.data), (weights2.rows * weights2.cols) * sizeof(float));
		outputFile.write(reinterpret_cast<char*>(bias_hidden.data), bias_hidden.rows * sizeof(float));
		outputFile.write(reinterpret_cast<char*>(bias_output.data), bias_output.rows * sizeof(float));

		outputFile.close();

	} else {
		std::cout << "Problem with output file" << std::endl;
	}

}
