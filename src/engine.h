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


Evaluation evaluate(Position const&); // defined in evaluation.cpp
std::pair<Move, Evaluation> search(Position, Depth soft_limit, Depth hard_limit, Depth left = 0, AB ab = AB()); // defined in search.cpp

#endif // #ifndef ENGINE
