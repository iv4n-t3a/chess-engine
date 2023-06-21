#ifndef ENGINE
#define ENGINE

#include "position.h"

typedef int64_t Evaluation;
typedef uint8_t Depth;

struct AB {
	Evaluation alpha;
	Evaluation beta;
};

const Evaluation MAX_EVALUATION = std::numeric_limits<Evaluation>::max();
const std::array<Evaluation, 2> best_ev = {
	MAX_EVALUATION,
	-MAX_EVALUATION
};


Evaluation evaluate(Position const&); // defined in evaluation.cpp
std::pair<Move, Evaluation> search(Position, Depth); // defined in search.cpp

#endif // #ifndef ENGINE
