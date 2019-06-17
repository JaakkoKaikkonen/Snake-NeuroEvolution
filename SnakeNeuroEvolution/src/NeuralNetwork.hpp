#pragma once
#include "Matrix.hpp"
#include <fstream>
#include "ImGuiLog.h"

class NeuralNetwork
{
public:
	NeuralNetwork(int numOfInputs, int numOfhiddenNodes, int numOfoutputs);


	static float sigmoid(float num);

	int predict(const Matrix<float>& inputs);

	void randomize();

	void save(char* snakeFileName, ImGuiLog& ImGuiLog);

public:
	Matrix<float> weights1;
	Matrix<float> weights2;
	Matrix<float> bias_hidden;
	Matrix<float> bias_output;

private:
	Matrix<float> inputs;
	Matrix<float> hiddenNodes;
	Matrix<float> outputs;

};

