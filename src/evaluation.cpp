#include "engine.h"

const Evaluation material_k = 1;

Evaluation material(Position const&);

Evaluation evaluate(Position const& p) {
	return
		material_k * material(p);
}


Evaluation material(Position const& p) {
	return
		( popcount(p.get_position(PAWN, WHITE)) - popcount(p.get_position(PAWN, BLACK)) ) * 100 +
		( popcount(p.get_position(KNIGHT, WHITE)) - popcount(p.get_position(KNIGHT, BLACK)) ) * 300 +
		( popcount(p.get_position(BISHOP, WHITE)) - popcount(p.get_position(BISHOP, BLACK)) ) * 300 +
		( popcount(p.get_position(ROOK, WHITE)) - popcount(p.get_position(ROOK, BLACK)) ) * 500 +
		( popcount(p.get_position(QUEEN, WHITE)) - popcount(p.get_position(QUEEN, BLACK)) ) * 800;
}
