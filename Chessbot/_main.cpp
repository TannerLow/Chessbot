//#include <iostream>
//#include <vector>
//#include <random>
//#include <cmath>
//#include <array>
//#include <cassert>
//#include <algorithm>
//
//// Config
//constexpr int INPUT_SIZE = 768;
//constexpr int HIDDEN_SIZE = 128;
//constexpr int OUTPUT_SIZE = 128;
//constexpr float LEARNING_RATE = 0.01f;
//constexpr int POPULATION = 10;
//constexpr int GAMES = POPULATION / 2;
//constexpr int MAX_MOVES = 100;
//
//// RNG
//float randWeight() {
//    static std::mt19937 rng(std::random_device{}());
//    static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
//    return dist(rng);
//}
//
//float sigmoid(float x) {
//    return 1.0f / (1.0f + std::exp(-x));
//}
//
//float sigmoid_derivative(float y) {
//    return y * (1.0f - y);
//}
//
//// Board state and move representation
//using Board = std::array<float, INPUT_SIZE>;
//using Move = std::pair<int, int>;
//
//std::array<float, OUTPUT_SIZE> moveToOneHot(int src, int dst) {
//    std::array<float, OUTPUT_SIZE> target{};
//    target[src] = 1.0f;
//    target[64 + dst] = 1.0f;
//    return target;
//}
//
//// Simple neural network with 1 hidden layer and backprop
//struct NeuralNet {
//    std::vector<float> w1; // input to hidden
//    std::vector<float> w2; // hidden to output
//    std::array<float, HIDDEN_SIZE> hidden_output{};
//    std::array<float, OUTPUT_SIZE> final_output{};
//
//    NeuralNet() {
//        w1.resize(INPUT_SIZE * HIDDEN_SIZE);
//        w2.resize(HIDDEN_SIZE * OUTPUT_SIZE);
//        std::generate(w1.begin(), w1.end(), randWeight);
//        std::generate(w2.begin(), w2.end(), randWeight);
//    }
//
//    std::array<float, OUTPUT_SIZE> forward(const Board& input) {
//        for (int i = 0; i < HIDDEN_SIZE; ++i) {
//            float sum = 0.0f;
//            for (int j = 0; j < INPUT_SIZE; ++j)
//                sum += input[j] * w1[i * INPUT_SIZE + j];
//            hidden_output[i] = sigmoid(sum);
//        }
//        for (int i = 0; i < OUTPUT_SIZE; ++i) {
//            float sum = 0.0f;
//            for (int j = 0; j < HIDDEN_SIZE; ++j)
//                sum += hidden_output[j] * w2[i * HIDDEN_SIZE + j];
//            final_output[i] = sigmoid(sum);
//        }
//        return final_output;
//    }
//
//    void backward(const Board& input, const std::array<float, OUTPUT_SIZE>& target) {
//        std::array<float, OUTPUT_SIZE> delta_output;
//        for (int i = 0; i < OUTPUT_SIZE; ++i) {
//            float error = target[i] - final_output[i];
//            delta_output[i] = error * sigmoid_derivative(final_output[i]);
//        }
//
//        std::array<float, HIDDEN_SIZE> delta_hidden{};
//        for (int i = 0; i < HIDDEN_SIZE; ++i) {
//            float error = 0.0f;
//            for (int j = 0; j < OUTPUT_SIZE; ++j)
//                error += delta_output[j] * w2[j * HIDDEN_SIZE + i];
//            delta_hidden[i] = error * sigmoid_derivative(hidden_output[i]);
//        }
//
//        for (int i = 0; i < OUTPUT_SIZE; ++i)
//            for (int j = 0; j < HIDDEN_SIZE; ++j)
//                w2[i * HIDDEN_SIZE + j] += LEARNING_RATE * delta_output[i] * hidden_output[j];
//
//        for (int i = 0; i < HIDDEN_SIZE; ++i)
//            for (int j = 0; j < INPUT_SIZE; ++j)
//                w1[i * INPUT_SIZE + j] += LEARNING_RATE * delta_hidden[i] * input[j];
//    }
//
//    void train(const Board& input, const std::array<float, OUTPUT_SIZE>& target) {
//        forward(input);
//        backward(input, target);
//    }
//};
//
//// Dummy legal move generation
//std::vector<Move> generateLegalMoves(const Board&) {
//    std::vector<Move> moves;
//    for (int i = 0; i < 5; ++i)
//        moves.emplace_back(rand() % 64, rand() % 64);
//    return moves;
//}
//
//Move pickMove(const NeuralNet& nn, const Board& board, const std::vector<Move>& legalMoves) {
//    auto output = nn.forward(board);
//    Move best = legalMoves[0];
//    float bestScore = -1e9;
//
//    for (const auto& move : legalMoves) {
//        int index = move.first + 64 + move.second;
//        if (output[index] > bestScore) {
//            bestScore = output[index];
//            best = move;
//        }
//    }
//
//    return best;
//}
//
//// Game record structure
//struct GameRecord {
//    std::vector<Board> states;
//    std::vector<Move> moves;
//};
//
//// Game simulation
//GameRecord playGame(NeuralNet& white, NeuralNet& black) {
//    GameRecord record;
//    Board board{}; // All zeros
//    bool whiteTurn = true;
//
//    for (int turn = 0; turn < MAX_MOVES; ++turn) {
//        auto& player = whiteTurn ? white : black;
//        auto legal = generateLegalMoves(board);
//        if (legal.empty()) break;
//
//        Move move = pickMove(player, board, legal);
//
//        // Log state and move
//        record.states.push_back(board);
//        record.moves.push_back(move);
//
//        // Fake move: not modifying board for now
//        whiteTurn = !whiteTurn;
//    }
//
//    return record;
//}
//
//// Train model on winners
//void trainFromWinners(NeuralNet& model, const std::vector<GameRecord>& winners) {
//    for (const auto& game : winners) {
//        for (size_t i = 0; i < game.states.size(); ++i) {
//            auto target = moveToOneHot(game.moves[i].first, game.moves[i].second);
//            model.train(game.states[i], target);
//        }
//    }
//}
//
//// === Main ===
//int main() {
//    std::vector<NeuralNet> agents(POPULATION);
//    std::vector<GameRecord> winners;
//    NeuralNet trainedModel;
//
//    // Self-play
//    for (int i = 0; i < GAMES; ++i) {
//        auto& a = agents[i * 2];
//        auto& b = agents[i * 2 + 1];
//        auto game = playGame(a, b);
//
//        // Dummy rule: player who started wins
//        winners.push_back(game);
//    }
//
//    std::cout << "Training on " << winners.size() << " winning games...\n";
//    trainFromWinners(trainedModel, winners);
//
//    std::cout << "Done. You now have a trained model.\n";
//    return 0;
//}
