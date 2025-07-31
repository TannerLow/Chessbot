#pragma once
#include <array>
#include <iostream>

enum class Piece {
    Empty,
    WhitePawn, WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing,
    BlackPawn, BlackRook, BlackKnight, BlackBishop, BlackQueen, BlackKing
};

struct Coord {
    int row;
    int col;
};

struct Move {
    Coord from;
    Coord to;
};

class ChessBoard {
public: 
    std::array<std::array<Piece, 8>, 8> board;

private:
    int movesSincePawnMovement = 0;
    int movesSinceCapture = 0;

public:
    // helpers
    bool isInside(int row, int col) const {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    bool isWhitePiece(Piece p) const {
        return p >= Piece::WhitePawn && p <= Piece::WhiteKing;
    }

    bool isBlackPiece(Piece p) const {
        return p >= Piece::BlackPawn && p <= Piece::BlackKing;
    }

    bool sameColor(Piece a, Piece b) const {
        if (a == Piece::Empty || b == Piece::Empty) return false;
        return (isWhitePiece(a) && isWhitePiece(b)) || (isBlackPiece(a) && isBlackPiece(b));
    }

public:
    ChessBoard() {
        // Empty board
        for (int r = 0; r < 8; ++r)
            for (int c = 0; c < 8; ++c)
                board[r][c] = Piece::Empty;

        // Setup white pieces
        for (int i = 0; i < 8; ++i) board[1][i] = Piece::WhitePawn;
        board[0][0] = board[0][7] = Piece::WhiteRook;
        board[0][1] = board[0][6] = Piece::WhiteKnight;
        board[0][2] = board[0][5] = Piece::WhiteBishop;
        board[0][3] = Piece::WhiteQueen;
        board[0][4] = Piece::WhiteKing;

        // Setup black pieces
        for (int i = 0; i < 8; ++i) board[6][i] = Piece::BlackPawn;
        board[7][0] = board[7][7] = Piece::BlackRook;
        board[7][1] = board[7][6] = Piece::BlackKnight;
        board[7][2] = board[7][5] = Piece::BlackBishop;
        board[7][3] = Piece::BlackQueen;
        board[7][4] = Piece::BlackKing;
    }

    void makeMove(Move move) {
        makeMove(move.from, move.to);
    }

    void makeMove(Coord from, Coord to) {
        if (!isInside(from.row, from.col) || !isInside(to.row, to.col)) return;

        Piece moving = board[from.row][from.col];
        if (moving == Piece::WhitePawn && to.row == 7) {
            board[from.row][from.col] = Piece::WhiteQueen;
        }
        else if (moving == Piece::BlackPawn && to.row == 0) {
            board[from.row][from.col] = Piece::BlackQueen;
        }
            
        // track for 50-move rule
        if (board[to.row][to.col] != Piece::Empty) {
            movesSinceCapture = 0;
        }
        else {
            movesSinceCapture++;
        }

        // track for 50-move rule
        if (board[from.row][from.col] == Piece::WhitePawn or board[from.row][from.col] == Piece::BlackPawn) {
            movesSincePawnMovement = 0;
        }
        else {
            movesSincePawnMovement++;
        }

        board[to.row][to.col] = board[from.row][from.col];
        board[from.row][from.col] = Piece::Empty;
    }

    //std::array<std::array<bool, 8>, 8> getValidMoves(Coord from) const {
    //    std::array<std::array<bool, 8>, 8> moves = {};
    //    if (!isInside(from.row, from.col)) return moves;

    //    Piece piece = board[from.row][from.col];
    //    if (piece == Piece::Empty) return moves;

    //    int r = from.row;
    //    int c = from.col;

    //    if (piece == Piece::WhitePawn) {
    //        if (isInside(r + 1, c) && board[r + 1][c] == Piece::Empty) moves[r + 1][c] = true;
    //        if (r == 1 && board[r + 1][c] == Piece::Empty && board[r + 2][c] == Piece::Empty) moves[r + 2][c] = true;
    //        if (isInside(r + 1, c - 1) && isBlack(board[r + 1][c - 1])) moves[r + 1][c - 1] = true;
    //        if (isInside(r + 1, c + 1) && isBlack(board[r + 1][c + 1])) moves[r + 1][c + 1] = true;
    //    }
    //    else if (piece == Piece::BlackPawn) {
    //        if (isInside(r - 1, c) && board[r - 1][c] == Piece::Empty) moves[r - 1][c] = true;
    //        if (r == 6 && board[r - 1][c] == Piece::Empty && board[r - 2][c] == Piece::Empty) moves[r - 2][c] = true;
    //        if (isInside(r - 1, c - 1) && isWhite(board[r - 1][c - 1])) moves[r - 1][c - 1] = true;
    //        if (isInside(r - 1, c + 1) && isWhite(board[r - 1][c + 1])) moves[r - 1][c + 1] = true;
    //    }
    //    else if (piece == Piece::WhiteKnight || piece == Piece::BlackKnight) {
    //        int dr[8] = { -2,-1,1,2,2,1,-1,-2 };
    //        int dc[8] = { 1,2,2,1,-1,-2,-2,-1 };
    //        for (int i = 0; i < 8; ++i) {
    //            int nr = r + dr[i];
    //            int nc = c + dc[i];
    //            if (isInside(nr, nc) && !sameColor(piece, board[nr][nc])) moves[nr][nc] = true;
    //        }
    //    }
    //    else if (piece == Piece::WhiteKing || piece == Piece::BlackKing) {
    //        for (int dr = -1; dr <= 1; ++dr) {
    //            for (int dc = -1; dc <= 1; ++dc) {
    //                if (dr == 0 && dc == 0) continue;
    //                int nr = r + dr;
    //                int nc = c + dc;
    //                if (isInside(nr, nc) && !sameColor(piece, board[nr][nc])) moves[nr][nc] = true;
    //            }
    //        }
    //    }
    //    else if (piece == Piece::WhiteBishop || piece == Piece::BlackBishop) {
    //        int dirs[4][2] = { {1,1},{1,-1},{-1,1},{-1,-1} };
    //        for (int d = 0; d < 4; ++d) {
    //            for (int i = 1; i < 8; ++i) {
    //                int nr = r + i * dirs[d][0];
    //                int nc = c + i * dirs[d][1];
    //                if (!isInside(nr, nc)) break;
    //                if (board[nr][nc] == Piece::Empty) {
    //                    moves[nr][nc] = true;
    //                }
    //                else {
    //                    if (!sameColor(piece, board[nr][nc])) moves[nr][nc] = true;
    //                    break;
    //                }
    //            }
    //        }
    //    }
    //    else if (piece == Piece::WhiteRook || piece == Piece::BlackRook) {
    //        int dirs[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
    //        for (int d = 0; d < 4; ++d) {
    //            for (int i = 1; i < 8; ++i) {
    //                int nr = r + i * dirs[d][0];
    //                int nc = c + i * dirs[d][1];
    //                if (!isInside(nr, nc)) break;
    //                if (board[nr][nc] == Piece::Empty) {
    //                    moves[nr][nc] = true;
    //                }
    //                else {
    //                    if (!sameColor(piece, board[nr][nc])) moves[nr][nc] = true;
    //                    break;
    //                }
    //            }
    //        }
    //    }
    //    else if (piece == Piece::WhiteQueen || piece == Piece::BlackQueen) {
    //        int dirs[8][2] = { {1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1} };
    //        for (int d = 0; d < 8; ++d) {
    //            for (int i = 1; i < 8; ++i) {
    //                int nr = r + i * dirs[d][0];
    //                int nc = c + i * dirs[d][1];
    //                if (!isInside(nr, nc)) break;
    //                if (board[nr][nc] == Piece::Empty) {
    //                    moves[nr][nc] = true;
    //                }
    //                else {
    //                    if (!sameColor(piece, board[nr][nc])) moves[nr][nc] = true;
    //                    break;
    //                }
    //            }
    //        }
    //    }

    //    return moves;
    //}

    std::array<std::array<bool, 8>, 8> getValidMoves(Coord from) const {
        std::array<std::array<bool, 8>, 8> legalMoves = {};
        if (!isInside(from.row, from.col)) return legalMoves;

        Piece piece = board[from.row][from.col];
        if (piece == Piece::Empty) return legalMoves;

        bool whiteTurn = isWhitePiece(piece);
        auto pseudo = getPseudoLegalMoves(from); // new helper we'll add

        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                if (!pseudo[r][c]) continue;

                ChessBoard copy = *this;
                copy.makeMove(from, { r, c });
                if (!copy.isInCheck(whiteTurn)) {
                    legalMoves[r][c] = true;
                }
            }
        }

        return legalMoves;
    }

    std::array<std::array<bool, 8>, 8> getPseudoLegalMoves(Coord from) const {
        std::array<std::array<bool, 8>, 8> moves = {};
        if (!isInside(from.row, from.col)) return moves;

        Piece piece = board[from.row][from.col];
        if (piece == Piece::Empty) return moves;

        int r = from.row;
        int c = from.col;

        if (piece == Piece::WhitePawn) {
            if (isInside(r + 1, c) && board[r + 1][c] == Piece::Empty) moves[r + 1][c] = true;
            if (r == 1 && board[r + 1][c] == Piece::Empty && board[r + 2][c] == Piece::Empty) moves[r + 2][c] = true;
            if (isInside(r + 1, c - 1) && isBlackPiece(board[r + 1][c - 1])) moves[r + 1][c - 1] = true;
            if (isInside(r + 1, c + 1) && isBlackPiece(board[r + 1][c + 1])) moves[r + 1][c + 1] = true;
        }
        else if (piece == Piece::BlackPawn) {
            if (isInside(r - 1, c) && board[r - 1][c] == Piece::Empty) moves[r - 1][c] = true;
            if (r == 6 && board[r - 1][c] == Piece::Empty && board[r - 2][c] == Piece::Empty) moves[r - 2][c] = true;
            if (isInside(r - 1, c - 1) && isWhitePiece(board[r - 1][c - 1])) moves[r - 1][c - 1] = true;
            if (isInside(r - 1, c + 1) && isWhitePiece(board[r - 1][c + 1])) moves[r - 1][c + 1] = true;
        }
        else if (piece == Piece::WhiteKnight || piece == Piece::BlackKnight) {
            int dr[8] = { -2,-1,1,2,2,1,-1,-2 };
            int dc[8] = { 1,2,2,1,-1,-2,-2,-1 };
            for (int i = 0; i < 8; ++i) {
                int nr = r + dr[i];
                int nc = c + dc[i];
                if (isInside(nr, nc) && !sameColor(piece, board[nr][nc])) moves[nr][nc] = true;
            }
        }
        else if (piece == Piece::WhiteKing || piece == Piece::BlackKing) {
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    int nr = r + dr;
                    int nc = c + dc;
                    if (isInside(nr, nc) && !sameColor(piece, board[nr][nc])) moves[nr][nc] = true;
                }
            }
        }
        else if (piece == Piece::WhiteBishop || piece == Piece::BlackBishop) {
            int dirs[4][2] = { {1,1},{1,-1},{-1,1},{-1,-1} };
            for (int d = 0; d < 4; ++d) {
                for (int i = 1; i < 8; ++i) {
                    int nr = r + i * dirs[d][0];
                    int nc = c + i * dirs[d][1];
                    if (!isInside(nr, nc)) break;
                    if (board[nr][nc] == Piece::Empty) {
                        moves[nr][nc] = true;
                    }
                    else {
                        if (!sameColor(piece, board[nr][nc])) moves[nr][nc] = true;
                        break;
                    }
                }
            }
        }
        else if (piece == Piece::WhiteRook || piece == Piece::BlackRook) {
            int dirs[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
            for (int d = 0; d < 4; ++d) {
                for (int i = 1; i < 8; ++i) {
                    int nr = r + i * dirs[d][0];
                    int nc = c + i * dirs[d][1];
                    if (!isInside(nr, nc)) break;
                    if (board[nr][nc] == Piece::Empty) {
                        moves[nr][nc] = true;
                    }
                    else {
                        if (!sameColor(piece, board[nr][nc])) moves[nr][nc] = true;
                        break;
                    }
                }
            }
        }
        else if (piece == Piece::WhiteQueen || piece == Piece::BlackQueen) {
            int dirs[8][2] = { {1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1} };
            for (int d = 0; d < 8; ++d) {
                for (int i = 1; i < 8; ++i) {
                    int nr = r + i * dirs[d][0];
                    int nc = c + i * dirs[d][1];
                    if (!isInside(nr, nc)) break;
                    if (board[nr][nc] == Piece::Empty) {
                        moves[nr][nc] = true;
                    }
                    else {
                        if (!sameColor(piece, board[nr][nc])) moves[nr][nc] = true;
                        break;
                    }
                }
            }
        }

        return moves;
    }


    void printBoard() const {
        std::cout << "  a b c d e f g h\n";
        for (int r = 7; r >= 0; --r) {
            std::cout << r + 1 << " ";
            for (int c = 0; c < 8; ++c) {
                char symbol = '.';
                switch (board[r][c]) {
                case Piece::WhitePawn:   symbol = 'P'; break;
                case Piece::WhiteRook:   symbol = 'R'; break;
                case Piece::WhiteKnight: symbol = 'N'; break;
                case Piece::WhiteBishop: symbol = 'B'; break;
                case Piece::WhiteQueen:  symbol = 'Q'; break;
                case Piece::WhiteKing:   symbol = 'K'; break;
                case Piece::BlackPawn:   symbol = 'p'; break;
                case Piece::BlackRook:   symbol = 'r'; break;
                case Piece::BlackKnight: symbol = 'n'; break;
                case Piece::BlackBishop: symbol = 'b'; break;
                case Piece::BlackQueen:  symbol = 'q'; break;
                case Piece::BlackKing:   symbol = 'k'; break;
                default:                 symbol = '.'; break;
                }
                std::cout << symbol << " ";
            }
            std::cout << r + 1 << "\n";
        }
        std::cout << "  a b c d e f g h\n";
    }

    Piece getPiece(int row, int col) const {
        if (!isInside(row, col)) return Piece::Empty;
        return board[row][col];
    }

    //bool isSquareAttacked(int row, int col, bool byWhite) const {
    //    Coord target = { row, col };

    //    for (int r = 0; r < 8; ++r) {
    //        for (int c = 0; c < 8; ++c) {
    //            Piece attacker = board[r][c];
    //            if (attacker == Piece::Empty) continue;

    //            if (byWhite && isWhite(attacker)) {
    //                auto attacks = getValidMoves({ r, c });
    //                if (attacks[row][col]) return true;
    //            }
    //            if (!byWhite && isBlack(attacker)) {
    //                auto attacks = getValidMoves({ r, c });
    //                if (attacks[row][col]) return true;
    //            }
    //        }
    //    }
    //    return false;
    //}

    bool isSquareAttacked(Coord target, bool byWhite) const {
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Piece p = board[r][c];
                if (p == Piece::Empty || isWhitePiece(p) != byWhite) continue;

                Coord from = { r, c };
                auto pseudoMoves = getPseudoLegalMoves(from);
                if (pseudoMoves[target.row][target.col]) {
                    return true;
                }
            }
        }
        return false;
    }


    bool isInCheck(bool whiteTurn) const {
        int kingRow = -1, kingCol = -1;
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Piece p = board[r][c];
                if ((whiteTurn && p == Piece::WhiteKing) || (!whiteTurn && p == Piece::BlackKing)) {
                    kingRow = r;
                    kingCol = c;
                    break;
                }
            }
        }
        if (kingRow == -1) return true; // King not found = dead
        return isSquareAttacked({ kingRow, kingCol }, !whiteTurn);
    }

    bool hasLegalMoves(bool whiteTurn) {
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Piece p = board[r][c];
                if (p == Piece::Empty) continue;
                if ((whiteTurn && isWhitePiece(p)) || (!whiteTurn && isBlackPiece(p))) {
                    Coord from = { r, c };
                    auto moves = getValidMoves(from);
                    for (int tr = 0; tr < 8; ++tr) {
                        for (int tc = 0; tc < 8; ++tc) {
                            if (moves[tr][tc]) {
                                // Try move and test check
                                ChessBoard copy = *this;
                                copy.makeMove(from, { tr, tc });
                                if (!copy.isInCheck(whiteTurn))
                                    return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

    bool insufficientMaterial() const {
        std::vector<Piece> pieces;
        for (int r = 0; r < 8; ++r)
            for (int c = 0; c < 8; ++c)
                if (board[r][c] != Piece::Empty)
                    pieces.push_back(board[r][c]);

        if (pieces.size() == 2) return true; // King vs King

        if (pieces.size() == 3) {
            for (Piece p : pieces) {
                if (p == Piece::WhiteBishop || p == Piece::BlackBishop ||
                    p == Piece::WhiteKnight || p == Piece::BlackKnight)
                    return true; // King + minor vs King
            }
        }

        return false;
    }

    std::string getGameResult(bool whiteTurn) {
        if (insufficientMaterial()) {
            return "Draw (Insufficient Material)";
        }

        if (!hasLegalMoves(whiteTurn)) {
            if (isInCheck(whiteTurn)) {
                return whiteTurn ? "Black wins (Checkmate)" : "White wins (Checkmate)";
            }
            else {
                return "Draw (Stalemate)";
            }
        }

        if (movesSinceCapture >= 50 and movesSincePawnMovement >= 50) {
            return "Draw (50-move rule)";
        }

        return "Game continues";
    }

    std::string coordToStr(const Coord& c) const {
        std::string s;
        s += ('a' + c.col);      // column: 0 -> 'a', 1 -> 'b', ...
        s += ('1' + c.row);      // row: 0 -> '1', 1 -> '2', ...
        return s;
    }

    std::string toSAN(const Coord& from, const Coord& to) const {
        std::string san = coordToStr(from) + coordToStr(to);

        // Optionally handle promotion (simplified: always promote to queen for now)
        Piece moving = board[from.row][from.col];
        if ((moving == Piece::WhitePawn && to.row == 7) ||
            (moving == Piece::BlackPawn && to.row == 0)) {
            san += 'q'; // assume queen promotion
        }

        return san;
    }

    std::vector<Coord> getMovablePieces(bool isWhite) const {
        std::vector<Coord> result;

        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                Piece p = board[row][col];

                if (p == Piece::Empty) continue;
                if (isWhitePiece(p) != isWhite) continue;

                Coord from = { row, col };
                auto validMoves = getValidMoves(from);

                // Check if this piece has at least one valid move
                for (int r = 0; r < 8; ++r) {
                    for (int c = 0; c < 8; ++c) {
                        if (validMoves[r][c]) {
                            result.push_back(from);
                            goto next_piece;
                        }
                    }
                }

            next_piece:;
            }
        }

        return result;
    }

    std::vector<Coord> getValidDestinations(const Coord& from) const {
        std::vector<Coord> moves;

        auto valid = getValidMoves(from);
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                if (valid[row][col]) {
                    moves.push_back({ row, col });
                }
            }
        }

        return moves;
    }

    float getPercentageToward50MoveRulePawns() const {
        return movesSincePawnMovement / 50.f;
    }

    float getPercentageToward50MoveRuleCaptures() const {
        return movesSinceCapture / 50.f;
    }
};