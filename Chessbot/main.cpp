#include <NeuralNetwork.h>


//int main() {
//    NeuralNetwork net(3, 2, { 5, 4 });
//
//    Matrix input = Matrix::fromVector({ 1.0, 0.5, -0.5 });
//    Matrix target = Matrix::fromVector({ 0.0f, 1.0f });
//
//    for (int i = 0; i < 1000; ++i)
//        net.backprop(input, target, 0.1);
//
//    Matrix prediction = net.forward(input);
//    prediction.print(); // should lean toward target
//
//    NeuralNetwork chessbot(768, 128, {500, 500});
//    Matrix prediction1 = chessbot.forward(Matrix::fromVector(std::vector<float>(768)));
//    prediction1.print();
//}

//int main() {
//    ChessBoard board;
//    board.printBoard();
//    std::cout << std::endl;
//
//    Coord from = { 1, 4 }; // e2
//    Coord to = { 3, 4 };   // e4
//    board.makeMove(from, to);
//    board.printBoard();
//    std::cout << std::endl;
//
//    auto moves = board.getValidMoves({ 6, 3 }); // d7
//    std::cout << "Valid moves for piece at d7:\n";
//    for (int r = 7; r >= 0; --r) {
//        for (int c = 0; c < 8; ++c) {
//            std::cout << (moves[r][c] ? '*' : '.') << " ";
//        }
//        std::cout << "\n";
//    }
//}

#include <ChessBoard.h>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <string>
#include <unordered_set>
#include <ChessBot.h>
#include <iostream>

bool writeVectorToFile(const std::vector<std::string>& lines, const std::string& filename) {
    std::ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        // Print an error message to the standard error stream.
        std::cerr << "Error: Could not open the file '" << filename << "'" << std::endl;
        return false;
    }

    for (const auto& line : lines) {
        // Write the current string to the file, followed by a newline character.
        outputFile << line << "\n";
    }
    
    std::cout << "Successfully wrote " << lines.size() << " lines to '" << filename << "'" << std::endl;
    return true;
}

void simulateGameAndDumpToFile(int gameNumber) {
    std::vector<std::string> moveHistory;
    moveHistory.reserve(500);

    ChessBoard board;
    // board.printBoard();

    std::mt19937 rng(std::random_device{}());
    bool whiteTurn = true;
    int moveCount = 0;

    while (true) {
        std::vector<Move> legalMoves;

        // Scan all pieces for current player
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Piece p = board.getPiece(r, c);
                if (p == Piece::Empty) continue;

                if ((whiteTurn && p >= Piece::WhitePawn && p <= Piece::WhiteKing) ||
                    (!whiteTurn && p >= Piece::BlackPawn && p <= Piece::BlackKing)) {

                    Coord from = { r, c };
                    auto moves = board.getValidMoves(from);
                    for (int tr = 0; tr < 8; ++tr) {
                        for (int tc = 0; tc < 8; ++tc) {
                            if (moves[tr][tc]) {
                                legalMoves.push_back({ from, {tr, tc} });
                            }
                        }
                    }
                }
            }
        }

        //if (legalMoves.empty()) {
        //    std::cout << (whiteTurn ? "Black" : "White") << " wins! No moves left.\n";
        //    break;
        //}
        std::string result = board.getGameResult(whiteTurn);
        if (result != "Game continues") {
            std::cout << result << "\n";
            break;
        }

        // Choose a random legal move
        std::uniform_int_distribution<int> dist(0, legalMoves.size() - 1);
        Move chosen = legalMoves[dist(rng)];

        // Make move
        std::string move = board.toSAN(chosen.from, chosen.to);
        //std::cout << move << "\n";
        moveHistory.push_back(move);
        board.makeMove(chosen.from, chosen.to);
        moveCount++;
        //std::cout << "Move " << moveCount << ": " << (whiteTurn ? "White" : "Black")
        //    << " moved from (" << chosen.from.row + 1 << "," << char('a' + chosen.from.col)
        //    << ") to (" << chosen.to.row + 1 << "," << char('a' + chosen.to.col) << ")\n";

        // board.printBoard();
        whiteTurn = !whiteTurn;
    }

    writeVectorToFile(moveHistory, "movesets/moveset" + std::to_string(gameNumber) + ".lan");
}

void simulateAndDumpNGames(unsigned int gameCount) {
    for (int i = 0; i < gameCount; i++) {
        simulateGameAndDumpToFile(i);
    }
}

void simulateAndDumpGame2(int gameNumber) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist;

    std::vector<std::string> moveHistory;
    moveHistory.reserve(500);

    ChessBoard board;
    bool whiteTurn = true;
    int moveCount = 0;

    while (true) {
        std::string result = board.getGameResult(whiteTurn);
        if(board.getGameResult(whiteTurn) != "Game continues") {
            std::cout << result << "\n";
            break;
        }

        std::vector<Coord> movablePieces = board.getMovablePieces(whiteTurn);
        dist = std::uniform_int_distribution<int>(0, movablePieces.size() - 1);
        Coord from = movablePieces[dist(rng)];
        std::vector<Coord> moveOptions = board.getValidDestinations(from);
        dist = std::uniform_int_distribution<int>(0, moveOptions.size() - 1);
        Coord to = moveOptions[dist(rng)];

        std::string move = board.toSAN(from, to);
        //std::cout << move << "\n";
        moveHistory.push_back(move);
        board.makeMove(from, to);
        moveCount++;

        whiteTurn = !whiteTurn;
    }

    writeVectorToFile(moveHistory, "movesets/moveset" + std::to_string(gameNumber) + ".lan");
}

void simulateAndDumpNGames2(unsigned int gameCount) {
    for (int i = 0; i < gameCount; i++) {
        simulateGameAndDumpToFile(i);
    }
}

std::string simulateBotVsBot() {
    ChessBot botA(true);
    ChessBot botB(false);

    std::vector<std::string> moveHistory;
    moveHistory.reserve(500);

    ChessBoard board;
    bool whiteTurn = true;
    int moveCount = 0;

    ChessBot* currentTurn = botA.isWhite ? &botA : &botB;
    std::string result = "undetermined";
    while (true) {
        result = board.getGameResult(whiteTurn);
        if (board.getGameResult(whiteTurn) != "Game continues") {
            std::cout << result << "\n";
            break;
        }

        Move move = currentTurn->decideMove(board);

        std::string moveStr = board.toSAN(move.from, move.to);
        //std::cout << "move #" << moveCount << ": " << moveStr << "\n";
        moveHistory.push_back(moveStr);
        board.makeMove(move.from, move.to);
        //board.printBoard();
        moveCount++;

        whiteTurn = !whiteTurn;
        currentTurn = (currentTurn == &botB) ? &botA : &botB;
    }

    writeVectorToFile(moveHistory, "movesets/moveset.lan");

    return result;
}

int main() {
    for (int i = 0; i < 300; i++) {
        std::string outcome = simulateBotVsBot();
        assert(outcome == "Draw (50-move rule)");
        if (outcome != "Draw (50-move rule)") {
            std::cout << "took " << i << " games" << std::endl;
            break;
        }
    }

    return 0;
}

