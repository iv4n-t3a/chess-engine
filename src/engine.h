#ifndef ENGINE
#define ENGINE

#include "position.h"

typedef int64_t Evaluation;
typedef uint8_t Depth;

const Evaluation MAX_EVALUATION = std::numeric_limits<Evaluation>::max();
const std::array<Evaluation, 2> best_ev = {
	MAX_EVALUATION,
	-MAX_EVALUATION
};

struct AB {
	Evaluation alpha = best_ev[BLACK];
	Evaluation beta = best_ev[WHITE];
};

const std::array<Evaluation, 7> piece_cost = {
	1, // PAWN
	3, // BISHOP
	3, // KNIGHT
	5, // ROOK
	9, // QUEEN
	0, // KING
	0, // NONE_PIECE
};


Evaluation evaluate(Position const&); // defined in evaluation.cpp
std::pair<Move, Evaluation> search(Position, Depth soft_limit, Depth hard_limit, Depth left = 0, AB ab = AB(), bool only_capture=false); // defined in search.cpp

#endif // #ifndef ENGINE
