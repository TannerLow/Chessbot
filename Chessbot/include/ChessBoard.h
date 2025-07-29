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

class ChessBoard {
private:
    std::array<std::array<Piece, 8>, 8> board;

    bool isInside(int row, int col) const {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    bool isWhite(Piece p) const {
        return p >= Piece::WhitePawn && p <= Piece::WhiteKing;
    }

    bool isBlack(Piece p) const {
        return p >= Piece::BlackPawn && p <= Piece::BlackKing;
    }

    bool sameColor(Piece a, Piece b) const {
        if (a == Piece::Empty || b == Piece::Empty) return false;
        return (isWhite(a) && isWhite(b)) || (isBlack(a) && isBlack(b));
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

    void makeMove(Coord from, Coord to) {
        if (!isInside(from.row, from.col) || !isInside(to.row, to.col)) return;
        board[to.row][to.col] = board[from.row][from.col];
        board[from.row][from.col] = Piece::Empty;
    }

    std::array<std::array<bool, 8>, 8> getValidMoves(Coord from) const {
        std::array<std::array<bool, 8>, 8> moves = {};
        if (!isInside(from.row, from.col)) return moves;

        Piece piece = board[from.row][from.col];
        if (piece == Piece::Empty) return moves;

        int r = from.row;
        int c = from.col;

        if (piece == Piece::WhitePawn) {
            if (isInside(r + 1, c) && board[r + 1][c] == Piece::Empty) moves[r + 1][c] = true;
            if (r == 1 && board[r + 1][c] == Piece::Empty && board[r + 2][c] == Piece::Empty) moves[r + 2][c] = true;
            if (isInside(r + 1, c - 1) && isBlack(board[r + 1][c - 1])) moves[r + 1][c - 1] = true;
            if (isInside(r + 1, c + 1) && isBlack(board[r + 1][c + 1])) moves[r + 1][c + 1] = true;
        }
        else if (piece == Piece::BlackPawn) {
            if (isInside(r - 1, c) && board[r - 1][c] == Piece::Empty) moves[r - 1][c] = true;
            if (r == 6 && board[r - 1][c] == Piece::Empty && board[r - 2][c] == Piece::Empty) moves[r - 2][c] = true;
            if (isInside(r - 1, c - 1) && isWhite(board[r - 1][c - 1])) moves[r - 1][c - 1] = true;
            if (isInside(r - 1, c + 1) && isWhite(board[r - 1][c + 1])) moves[r - 1][c + 1] = true;
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

};