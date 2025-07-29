#pragma once
#include <Matrix.h>
#include <Activation.h>
#include <random>

struct NeuralNetwork {
    std::vector<Matrix> weights;
    std::vector<Matrix> biases;
    std::vector<Activation> activations;
    size_t inputSize, outputSize;
    size_t layerCount;

    // Sigmoid activations and final layer softmax
    NeuralNetwork(size_t inputSize, size_t outputSize, const std::vector<size_t>& hiddenSizes)
        : inputSize(inputSize), outputSize(outputSize) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(-1.0, 1.0);

        std::vector<size_t> layerSizes = { inputSize };
        layerSizes.insert(layerSizes.end(), hiddenSizes.begin(), hiddenSizes.end());
        layerSizes.push_back(outputSize);

        layerCount = layerSizes.size() - 1;

        for (size_t i = 0; i < layerCount; ++i) {
            weights.emplace_back(Matrix(layerSizes[i + 1], layerSizes[i]));
            biases.emplace_back(Matrix(layerSizes[i + 1], 1));
            activations.push_back(i == layerCount - 1 ? Activation::Softmax : Activation::Sigmoid);

            // initialize weights and biases
            for (size_t r = 0; r < weights[i].rows; ++r) {
                for (size_t c = 0; c < weights[i].cols; ++c)
                    weights[i].data[r][c] = dist(gen);
                biases[i].data[r][0] = dist(gen);
            }
        }
    }

    // Forward pass
    Matrix forward(const Matrix& input, std::vector<Matrix>* layerOutputs = nullptr) const {
        Matrix a = input;
        if (layerOutputs) layerOutputs->clear();
        for (size_t i = 0; i < layerCount; ++i) {
            Matrix z = (weights[i] * a) + biases[i];
            if (activations[i] == Activation::Sigmoid)
                a = z.sigmoid();
            else
                a = z.softmax();
            if (layerOutputs) layerOutputs->push_back(a);
        }
        return a;
    }

    // Backpropagation (one sample)
    void backprop(const Matrix& input, const Matrix& target, double learningRate) {
        std::vector<Matrix> activationsCache;
        Matrix output = forward(input, &activationsCache);

        std::vector<Matrix> deltas(layerCount);
        std::vector<Matrix> zs(layerCount);

        // Output error (Cross-Entropy + Softmax derivative simplifies to: prediction - target)
        deltas.back() = output + (target * -1.0f); // delta = output - target

        // Backpropagate error
        for (int i = layerCount - 2; i >= 0; --i) {
            Matrix& a = activationsCache[i];
            Matrix da = a.sigmoidDerivative();
            Matrix wT = transpose(weights[i + 1]);
            deltas[i] = hadamard(wT * deltas[i + 1], da);
        }

        // Update weights and biases
        Matrix a_prev = input;
        for (size_t i = 0; i < layerCount; ++i) {
            const Matrix& delta = deltas[i];
            const Matrix& a_input = (i == 0 ? a_prev : activationsCache[i - 1]);

            Matrix dw = delta * transpose(a_input);
            for (size_t r = 0; r < weights[i].rows; ++r)
                for (size_t c = 0; c < weights[i].cols; ++c)
                    weights[i].data[r][c] -= learningRate * dw.data[r][c];

            for (size_t r = 0; r < biases[i].rows; ++r)
                biases[i].data[r][0] -= learningRate * delta.data[r][0];
        }
    }

    // Transpose helper
    static Matrix transpose(const Matrix& m) {
        Matrix result(m.cols, m.rows);
        for (size_t i = 0; i < m.rows; ++i)
            for (size_t j = 0; j < m.cols; ++j)
                result.data[j][i] = m.data[i][j];
        return result;
    }

    // Element-wise multiplication
    static Matrix hadamard(const Matrix& a, const Matrix& b) {
        assert(a.rows == b.rows && a.cols == b.cols);
        Matrix result(a.rows, a.cols);
        for (size_t i = 0; i < a.rows; ++i)
            for (size_t j = 0; j < a.cols; ++j)
                result.data[i][j] = a.data[i][j] * b.data[i][j];
        return result;
    }
};