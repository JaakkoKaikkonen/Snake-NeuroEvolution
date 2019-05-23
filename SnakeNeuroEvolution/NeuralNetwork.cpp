#include "NeuralNetwork.h"
#include <math.h>


NeuralNetwork::NeuralNetwork(int numOfInputs, int numOfhiddenNodes, int numOfoutputs)
	: inputs(numOfInputs, 1), 
	  hiddenNodes(numOfhiddenNodes, 1), 
	  outputs(numOfoutputs, 1),
	  weights1(numOfhiddenNodes, numOfInputs), 
	  weights2(numOfoutputs, numOfhiddenNodes),
	  bias_hidden(numOfhiddenNodes, 1), 
	  bias_output(numOfoutputs, 1)
{
	weights1.randomize();
	weights2.randomize();
	bias_hidden.randomize();
	bias_output.randomize();

	/*weights1.print();
	std::cout << "\n";
	weights2.print();
	std::cout << "\n";
	bias_hidden.print();
	std::cout << "\n";
	bias_output.print();
	std::cout << "\n";*/
}


float NeuralNetwork::sigmoid(float num) {
	return (1 / (1 + exp(-num)));
}


int NeuralNetwork::predict(const Matrix<float>& inputs) {

	/*inputs.print();
	std::cout << "\n";
	inputs.map(NeuralNetwork::sigmoid);
	inputs.print();
	std::cout << "\n";*/

	//std::cout << "Weights1: \n";
	//weights1.print();
	//std::cout << "\n";
	hiddenNodes = weights1 * inputs;
	//std::cout << "hiddenNodes: \n";
	//hiddenNodes.print();
	//std::cout << "\n";
	hiddenNodes += bias_hidden;
	//std::cout << "hiddenNodes with bias: \n";
	//hiddenNodes.print();
	//std::cout << "\n";
	hiddenNodes.map(NeuralNetwork::sigmoid);
	//std::cout << "hiddenNodes with sigmoid: \n";
	//hiddenNodes.print();
	//std::cout << "\n";

	outputs = weights2 * hiddenNodes;
	outputs += bias_output;
	outputs.map(NeuralNetwork::sigmoid);

	//std::cout << "outputs: \n";
	//outputs.print();
	//std::cout << "\n";

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
