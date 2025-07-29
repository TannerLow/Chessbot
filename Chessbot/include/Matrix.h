#pragma once
#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>

struct Matrix {
    std::vector<std::vector<float>> data;
    size_t rows;
    size_t cols;

    Matrix() : rows(0), cols(0) {}
    Matrix(size_t r, size_t c, float init = 0.0) : rows(r), cols(c), data(r, std::vector<float>(c, init)) {}

    static Matrix fromVector(const std::vector<float>& vec) {
        Matrix m(vec.size(), 1);
        for (size_t i = 0; i < vec.size(); ++i)
            m.data[i][0] = vec[i];
        return m;
    }

    std::vector<float> toVector() const {
        assert(cols == 1);
        std::vector<float> vec(rows);
        for (size_t i = 0; i < rows; ++i)
            vec[i] = data[i][0];
        return vec;
    }

    // Element-wise addition
    Matrix operator+(const Matrix& other) const {
        assert(rows == other.rows && cols == other.cols);
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                result.data[i][j] = data[i][j] + other.data[i][j];
        return result;
    }

    // Matrix multiplication
    Matrix operator*(const Matrix& other) const {
        assert(cols == other.rows);
        Matrix result(rows, other.cols);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < other.cols; ++j)
                for (size_t k = 0; k < cols; ++k)
                    result.data[i][j] += data[i][k] * other.data[k][j];
        return result;
    }

    Matrix operator*(const float scalar) const {
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result.data[i][j] = data[i][j] * scalar;
            }
        }

        return result;
    }

    // Apply sigmoid function element-wise
    Matrix sigmoid() const {
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                result.data[i][j] = 1.0 / (1.0 + std::exp(-data[i][j]));
        return result;
    }

    // Apply sigmoid derivative element-wise
    Matrix sigmoidDerivative() const {
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j) {
                float s = 1.0 / (1.0 + std::exp(-data[i][j]));
                result.data[i][j] = s * (1.0 - s);
            }
        return result;
    }

    // Apply softmax (column vector only)
    Matrix softmax() const {
        assert(cols == 1);
        std::vector<double> exp_vals(rows);
        float max_val = data[0][0];
        for (size_t i = 1; i < rows; ++i)
            if (data[i][0] > max_val)
                max_val = data[i][0];

        float sum = 0.0;
        for (size_t i = 0; i < rows; ++i) {
            exp_vals[i] = std::exp(data[i][0] - max_val);
            sum += exp_vals[i];
        }

        Matrix result(rows, 1);
        for (size_t i = 0; i < rows; ++i)
            result.data[i][0] = exp_vals[i] / sum;
        return result;
    }

    // Derivative of softmax (assuming output is softmax vector and index is true class)
    Matrix softmaxDerivative(size_t trueIndex) const {
        assert(cols == 1);
        Matrix result(rows, 1);
        for (size_t i = 0; i < rows; ++i) {
            float si = data[i][0];
            if (i == trueIndex)
                result.data[i][0] = si * (1.0 - si);
            else
                result.data[i][0] = -si * data[trueIndex][0];
        }
        return result;
    }

    void print() const {
        for (const auto& row : data) {
            for (float val : row)
                std::cout << val << ' ';
            std::cout << '\n';
        }
    }
};