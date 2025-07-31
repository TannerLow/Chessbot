#pragma once
#include "NeuralNetwork.h"
#include "ChessBoard.h"
#include <random>
#include <limits>
#include <cassert>

struct ChessBot {
	NeuralNetwork chessnet;
	bool isWhite = false;

	// 770 = 768 (board state: 64 * 6 * 2) + 2 (whos turn) + 2 (%s toward 50-move rule)
	ChessBot(bool isWhite) : chessnet(772, 128, { 500, 500 }) {
		this->isWhite = isWhite;
	}

	void insertAsOneHot(bool isWhite, int index, std::vector<float>& vector) {
		if (index < 0) {
			vector.insert(vector.end(), 12, 0.f);
		}
		else if (isWhite) {
			vector.insert(vector.end(), index, 0.f);
			vector.push_back(1.f);
			vector.insert(vector.end(), (5 - index) + 6, 0.f);
		}
		else {
			index -= 6;
			vector.insert(vector.end(), 6 + index, 0.f);
			vector.push_back(1.f);
			vector.insert(vector.end(), 5 - index, 0.f);
		}
	}

	Matrix encodeBoard(const ChessBoard& board) {
		std::vector<float> encodingVector;
		encodingVector.reserve(772);

		for (auto& row : board.board) {
			for(Piece piece : row) {
				insertAsOneHot(board.isWhitePiece(piece), (int)piece - 1, encodingVector);
			}
		}

		encodingVector.push_back(isWhite ? 1.f : 0.f);
		encodingVector.push_back(!isWhite ? 1.f : 0.f);

		encodingVector.push_back(board.getPercentageToward50MoveRuleCaptures());
		encodingVector.push_back(board.getPercentageToward50MoveRulePawns());

		return Matrix::fromVector(encodingVector);
	}

	Coord getMostConfidentPosition(const Matrix& modelOutput, const std::vector<Coord>& possibleChoices, bool isDestination) {
		assert(possibleChoices.size() > 0 && "Bot has no possible choices.");
		int offset = isDestination ? 64 : 0;
		float max = std::numeric_limits<float>::lowest();
		Coord mostConfidentMove;

		for (const Coord& coord : possibleChoices) {
			float confidence = modelOutput.data[coord.row * 8 + coord.col + offset][0];
			if (confidence > max) {
				max = confidence;
				mostConfidentMove = coord;
			}
		}

		return mostConfidentMove;
	}

	Coord getMovablePieceDecision(const Matrix& modelOutput, const std::vector<Coord>& movablePieces) {
		return getMostConfidentPosition(modelOutput, movablePieces, false);
	}

	Coord getDestinationDecision(const Matrix& modelOutput, const std::vector<Coord>& movablePieces) {
		float max = std::numeric_limits<float>::lowest();
		Coord mostConfidentMove = movablePieces[0];

		for (const Coord& coord : movablePieces) {
			float confidence = modelOutput.data[coord.row * 8 + coord.col][0];
			if (confidence > max) {
				max = confidence;
				mostConfidentMove = coord;
			}
		}

		return mostConfidentMove;
	}

	Move decideMove(const ChessBoard& board) {
		Move move;

		Matrix input = encodeBoard(board);

		std::vector<Coord> movablePieces = board.getMovablePieces(isWhite);
		assert(movablePieces.size() > 0 && "Bot has ran out of possible moves. Game should have ended already.");
		Matrix rawOutput = chessnet.forward(input);
		move.from = getMostConfidentPosition(rawOutput, movablePieces, false);
		std::vector<Coord> validDestinations = board.getValidDestinations(move.from);
		move.to = getMostConfidentPosition(rawOutput, validDestinations, true);

		return move;
	}
};
