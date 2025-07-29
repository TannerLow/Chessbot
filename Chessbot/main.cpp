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

struct Move {
    Coord from;
    Coord to;
};

int main() {
    ChessBoard board;
    board.printBoard();

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

        if (legalMoves.empty()) {
            std::cout << (whiteTurn ? "Black" : "White") << " wins! No moves left.\n";
            break;
        }

        // Choose a random legal move
        std::uniform_int_distribution<int> dist(0, legalMoves.size() - 1);
        Move chosen = legalMoves[dist(rng)];

        // Make move
        board.makeMove(chosen.from, chosen.to);
        moveCount++;
        std::cout << "Move " << moveCount << ": " << (whiteTurn ? "White" : "Black")
            << " moved from (" << chosen.from.row + 1 << "," << char('a' + chosen.from.col)
            << ") to (" << chosen.to.row + 1 << "," << char('a' + chosen.to.col) << ")\n";

        board.printBoard();
        whiteTurn = !whiteTurn;

        // Optional delay
        // std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    return 0;
}


